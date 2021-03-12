//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Flow\VTFlow.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TFormServer.h"
#include "TFormCfg.h"
#include "TFormOpAuto.h"
#include "TFormOpManual.h"
#include "VTSinapServer.h"
#include "..\MultiMonitor\VTMultiPanel.h"
#include "..\Protocolo\VTProtocolo.h"
#include "..\User\VTSinapUser.h"
#include "..\User\VTSinapUser.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormServer::TFormServer(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
                       : TForm(Owner)
   {
   //insere Form em seu Parent
   if (parent != NULL) Parent = parent;
   //cria objeto Apl
   apl = NewObjApl(this, apl_owner);
   //cria objetos
   apl->Add(user   = NewObjSinapUser(apl));
   apl->Add(server = NewObjSinapServer(apl));
   //cria lista de Eqptos para supervis�o autom�tica
   supervisao.ativa  = false;
   supervisao.lisEQP = new TList();
   //habilita log do objeto SinapUser
   user->HabilitaLog(RichEditRx->Lines, RichEditTx->Lines, StatusBar->Panels->Items[1]);
   //desabilita supervis�o manual/auto enquanto n�o houver Eqptos selecionados
   ActionOpManual->Enabled = false;
   ActionOpAuto->Enabled   = false;
   //inicia Socket
   ActionServerStart->Execute();
   //configura op��es do fluxo de pot�ncia
   ConfiguraFlow();
   }

//---------------------------------------------------------------------------
__fastcall TFormServer::~TFormServer(void)
   {
   //destr�i o FormOpAuto e FormOpManual
   if (FormCfg) {delete FormCfg; FormCfg = NULL;}
   if (FormOpManual) {delete FormOpManual; FormOpManual  = NULL;}
   //destr�i objetos
   if (apl) {delete apl; apl = NULL;}
   //destr�i lista sem destruir seus objetos
   if (supervisao.lisEQP) {delete supervisao.lisEQP; supervisao.lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormServer::ActionClearLogExecute(TObject *Sender)
   {
   RichEditRx->Lines->Clear();
   RichEditTx->Lines->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TFormServer::ActionListaIPExecute(TObject *Sender)
   {
   char               szHostName[1024] = "";
   struct sockaddr_in SocketAddress;
   struct hostent     *pHost        = 0;

   WSADATA            WSAData;

   // Initialize winsock dll
   if (WSAStartup(MAKEWORD(1, 0), &WSAData) != 0)
      {//erro
      return;
      }
   if (::gethostname(szHostName, sizeof(szHostName)) != 0)
      {//erro
      return;
      }
   // Get local IP addresses
   if ((pHost = ::gethostbyname(szHostName)) == NULL)
      {//erro
      return;
      }
   for(int iCnt = 0; ((pHost->h_addr_list[iCnt]) && (iCnt < 10)); ++iCnt)
      {
      memcpy(&SocketAddress.sin_addr, pHost->h_addr_list[iCnt], pHost->h_length);
      user->LogTx(inet_ntoa(SocketAddress.sin_addr));
      }
   user->LogTx("Endere�os de IP dispon�veis:");
   // Cleanup
   WSACleanup();
   }

//---------------------------------------------------------------------------
void __fastcall TFormServer::ActionOpManualExecute(TObject *Sender)
   {
   //vari�veis locais
   VTMultiPanel *MultiPanel = (VTMultiPanel*)apl->GetObject(__classid(VTMultiPanel));

   //esconde este Form
   Hide();
   //verifica se j� existe o FormOpManual
   if (FormOpManual == NULL)
      {//cria TFormCfgEqpto como janela normal
      FormOpManual = new TFormOpManual(this, apl, MultiPanel->PanelAtivo, ActionShow);
      }
   //exibe o FormOpAuto como janela normal
   FormOpManual->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormServer::ActionOpAutoExecute(TObject *Sender)
   {
   //vari�veis locais
   VTMultiPanel *MultiPanel = (VTMultiPanel*)apl->GetObject(__classid(VTMultiPanel));

   //esconde este Form
   Hide();
   //verifica se j� existe o FormOpManual
   if (FormOpAuto == NULL)
      {//cria TFormCfgEqpto como janela normal
      FormOpAuto = new TFormOpAuto(this, apl, MultiPanel->PanelAtivo, ActionShow);
      }
   //exibe o FormOpAuto como janela normal
   FormOpAuto->Show();
   }

/*
//---------------------------------------------------------------------------
void __fastcall TFormServer::ActionOpAutoExecute(TObject *Sender)
   {
   //vari�veis locais
   double      dt_min;
   TFormEdtTxt *Form;

   //cria TFormEdtTxt e exibe como janela modal
   Form = new TFormEdtTxt(this, "Simula��o de Tempo Real");
   Form->Label->Caption = "Tempo p/ alterar patamar (min)";
   Form->Texto          = "5";
   if (Form->ShowModal() == mrOk)
      {//determina intervalo de tempo para alterar patamar
      if (! StrToDouble(Form->Texto, dt_min))
         {
         Aviso("Intervalo de tempo incorreto");
         }
      else
         {//inicia Timer
         Timer->Interval = dt_min * 60 * 1000;
         Timer->Enabled = true;
         }
      }
   //destr�i FormTempo
   delete Form;
   }
*/
/*
//---------------------------------------------------------------------------
void __fastcall TFormServer::ActionOpAuto_StopExecute(TObject *Sender)
   {
   //desabilita o Timer
   Timer->Enabled = false;
   //mat�m StatusBar com a �ltima informa��o de patamar
   //StatusBar->Panels->Items[1]->Text = "";
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormServer::ActionServerStartExecute(TObject *Sender)
   {
   try{//ativa o Servidor
      server->Conecta(SERVER_PORT);
      //verifica se o Socket est� ativo
      if (server->Ativo)
         {//indica que o Server est� ativo
         //StatusBar->SimpleText = "IP: " + ServerSocket->Socket->LocalAddress +
         //                      "   -  PORT = " + IntToStr(ServerSocket->Socket->LocalPort);
         //StatusBar->SimpleText = "Conectado: PORT = " + IntToStr(SERVER_PORT);
         StatusBar->Panels->Items[0]->Text = "Conectado: PORT = " + IntToStr(SERVER_PORT);
         }
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormServer::ActionServerStopExecute(TObject *Sender)
   {
   //cancela conex�o
   server->Desconecta();
   //indica que o Server est� inativo
   StatusBar->Panels->Items[0]->Text = "Inativo";
   StatusBar->Panels->Items[1]->Text = "";
   }

//---------------------------------------------------------------------------
void __fastcall TFormServer::ActionShowExecute(TObject *Sender)
   {
   //vari�veis locais
   VTSinapServer *server = (VTSinapServer*)apl->GetObject(__classid(VTSinapServer));

   //torna a exibir este Form
   Show();
   //verifica se h� equipamentos selecionados para supervis�o
   supervisao.lisEQP->Clear();
   server->LisEqptoSupervisionado(supervisao.lisEQP, eqptoCHAVE);
   ActionOpManual->Enabled = (supervisao.lisEQP->Count > 0);
   ActionOpAuto->Enabled   = (supervisao.lisEQP->Count > 0);
   }

//---------------------------------------------------------------------------
void __fastcall TFormServer::ActionCfgSupExecute(TObject *Sender)
   {
   //vari�veis locais
   VTMultiPanel *MultiPanel = (VTMultiPanel*)apl->GetObject(__classid(VTMultiPanel));

   //esconde este Form
   Hide();
   //verifica se j� existe o  FormCfgEqpto
   if (FormCfg == NULL)
      {//cria TFormCfgEqpto como janela normal
      FormCfg = new TFormCfg(this, apl, MultiPanel->PanelAtivo, ActionShow);
      }
   //exibe o FormCfg como janela normal
   FormCfg->Show();
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormServer::ActionCfgSup_StopExecute(TObject *Sender)
   {
   //finaliza supervis�o autom�tica
   if (FormCfg) FormCfg->Stop();
   //troca Action associado ao ButSupervisaoAuto
   ButSupervisaoAuto->Action = ActionSupAutomatica_Start;
   //atualiza StatusBar
   StatusBar->Panels->Items[1]->Text = "";
   }
*/

//---------------------------------------------------------------------------
void __fastcall TFormServer::ConfiguraFlow(void)
   {
   //vari�veis locais
   VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));

   flow->ProgressoEnabled = false;
   flow->AvisoEnabled     = false;
   flow->TipoRelatorio    = relNENHUM;
   }


//---------------------------------------------------------------------------
void __fastcall TFormServer::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destr�i o Form
   Action = caFree;
   }

/*
//IMPORTANTE: FUN��O BLOQUEADA pois geral->Infoset n�o funciona devido op��o de compila��o:
Data Alignment => Byte
//---------------------------------------------------------------------------
void __fastcall TFormServer::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   Left = 0;
   Top  = 0;
   }
*/

//---------------------------------------------------------------------------
//eof



