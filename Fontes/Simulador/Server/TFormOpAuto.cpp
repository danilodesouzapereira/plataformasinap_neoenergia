//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include<DateUtils.hpp>
#include "TFormOpAuto.h"
#include "VTSinapServer.h"
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\Flow\VTFlow.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTChave.h"
#include "..\SInap\VTSinapChild.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormOpAuto::TFormOpAuto(TComponent* Owner, VTApl *apl_owner, TWinControl *parent, TAction *ActionOnClose)
   : TForm(Owner)
   {
   //salva ponteiro para objetos
   this->apl           = apl_owner;
   this->ActionOnClose = ActionOnClose;
   edita     = (VTEdita*)apl->GetObject(__classid(VTEdita));
   grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   server    = (VTSinapServer*)apl->GetObject(__classid(VTSinapServer));
   //desabilita o Timer
   Timer->Enabled = false;
   //cria lista de Chaves supervisionadas
   lisCHV = new TList();
   lisMAN = new TList();
   server->LisEqptoSupervisionado(lisCHV, eqptoCHAVE);
   //esconde GBoxEvento
   GBoxEvento->Visible = false;
   //simula sele��o do modo de altera��o dos patamares p/ exibir/esconder CBoxTempoPatamar
   RadioGroupPatamarClick(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TFormOpAuto::~TFormOpAuto(void)
   {
   //cancela moldura na Chave de defeito
   grafico->Moldura((VTChave*)NULL);
   //destr�i lista sem destruir seus objetos
   if (lisCHV) {delete lisCHV; lisCHV = NULL;}
   if (lisMAN) {delete lisMAN; lisMAN = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormOpAuto::ActionFechaExecute(TObject *Sender)
   {
   //fecha o form
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormOpAuto::ActionStartExecute(TObject *Sender)
   {
   //vari�veis locais
   VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

   //verifica op��es
   if (! LeDados()) return;
   //exibe GBoxEvento
   GBoxEvento->Visible = true;
   //executa fluxo de pot�ncia
   Log("Calculando fluxo de pot�ncia");
   sinap_child->ActionFlow->Execute();
   //habilita conrole de defeito
   ctrl_def.enabled = true;
   //define estado inicial da m�quina de transi��o de estados
   ctrl_def.estado = estINICIO;
   //executa MTE de controle de conting�ncia
   ExecutaTransicaoControleContingencia();
   //habilita o Timer
   Timer->Interval = 1000; //1seg
   Timer->Enabled  = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormOpAuto::ActionStopExecute(TObject *Sender)
   {
   //desabilita o Timer
   Timer->Enabled  = false;
   Timer->Interval = 0;
   //habilita conrole de defeito
   ctrl_def.enabled = false;
   //esconde GBoxEvento
   GBoxEvento->Visible = false;
   //restaura estado da rede, voltando todas chaves para seu estado normal
   RestauraEstadoNormalChaves();
   //cancela moldura na Chave de defeito
   grafico->Moldura((VTChave*)NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormOpAuto::ChaveAbre(VTChave *chave, bool lockout)
   {
   //vari�veis locais
   VTChave *clone;

   //configura estado da Chave
   clone                           = chave->Clone();
   clone->Estado                   = chvABERTA;
   clone->Status[sttCHAVE_LOCKOUT] = lockout;
   clone->Status[sttCHAVE_CBR]     = false;
   clone->Status[sttCHAVE_CBTL]    = false;
   //altera a Chave na rede
   edita->Altera(chave, clone);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormOpAuto::ChaveFecha(VTChave *chave)
   {
   //vari�veis locais
   VTChave *clone;

   //configura estado da Chave
   clone                           = chave->Clone();
   clone->Estado                   = chvFECHADA;
   clone->Status[sttCHAVE_LOCKOUT] = false;
   clone->Status[sttCHAVE_CBR]     = false;
   clone->Status[sttCHAVE_CBTL]    = false;
   //altera a Chave na rede
   edita->Altera(chave, clone);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormOpAuto::ExecutaTransicaoControleContingencia(void)
   {
   //vari�veis locais
   bool lockout;

   //verifica se transi��o est�o habilitada
   if (! ctrl_def.enabled) return(true);
   //IMPORTANTE: desabilita o timer para evitar transi��o de estado indevida
   //            porque o c�lculo do fluxo pode ser grande para algumas redes
   Timer->Enabled = false;
   try{//verifica o estado
      switch(ctrl_def.estado)
         {
         case estINICIO:
            //redefine estado: rede em estado normal
            ctrl_def.estado = estREDE_NORMAL;
            //reinicia temporiza��o
            ctrl_def.timer_count = ctrl_def.tempo.rede_normal;
            //seleciona Patamar da hora atual
            ctrl_pat.patamar = server->PatamarAtual();
            LogPatamar("Hor�rio de simula��o: " + ctrl_pat.patamar->Nome);
            //restaura estado da rede, voltando todas chaves para seu estado normal
            RestauraEstadoNormalChaves();
            //reinicia Log
            LogClear();
            Log("Rede em estado normal");
            break;
         case estREDE_NORMAL:
            //redefine estado: chave aberta - primeira abertura
            ctrl_def.estado = estCHAVE_ABERTA1;
            //reinicia temporiza��o
            ctrl_def.timer_count = ctrl_def.tempo.chave_aberta1;
            //seleciona pr�xima Chave para simular conting�ncia
            if (SelecionaChaveDefeito())
               {//abre a Chave selecionada
               ChaveAbre(ctrl_def.chave, lockout=false);
               Log("Abertura 1 da chave " + ctrl_def.chave->Codigo);
               //exibe moldura na Chave de defeito
               grafico->Moldura(ctrl_def.chave);
               }
            break;
         case estCHAVE_ABERTA1:
            //redefine estado: chave aberta - segunda abertura
            ctrl_def.estado = estCHAVE_ABERTA2;
            //reinicia temporiza��o
            ctrl_def.timer_count = ctrl_def.tempo.chave_aberta2;
            //atualiza estado da Chave - fechada
            ChaveFecha(ctrl_def.chave);
            Log("Fechamento 1 da chave " + ctrl_def.chave->Codigo);
            //atualiza estado da Chave - aberta
            ChaveAbre(ctrl_def.chave, lockout=false);
            Log("Abertura 2 da chave " + ctrl_def.chave->Codigo);
            break;
         case estCHAVE_ABERTA2:
            //redefine estado: rede em contingencia
            ctrl_def.estado = estREDE_CONTINGENCIA;
            //reinicia temporiza��o
            ctrl_def.timer_count = ctrl_def.tempo.rede_contingencia;
            //atualiza estado da Chave - fechada
            ChaveFecha(ctrl_def.chave);
            Log("Fechamento 2 da chave " + ctrl_def.chave->Codigo);
            //atualiza estado da Chave - aberta com indica��o ou n�o de lockout
            ChaveAbre(ctrl_def.chave, lockout=CheckBoxLockout->Checked);
            Log("Abertura 3 da chave " + ctrl_def.chave->Codigo);
            if (CheckBoxLockout->Checked) Log("> Chave com indica��o de Lockout");
            else                          Log("> Chave sem indica��o de Lockout");
            Log("Rede em conting�ncia");
            break;
         case estREDE_CONTINGENCIA:
            //redefine estado: rede em estado normal
            ctrl_def.estado = estREDE_NORMAL;
            //reinicia temporiza��o
            ctrl_def.timer_count = ctrl_def.tempo.rede_normal;
            //restaura estado da rede, voltando todas chaves para seu estado normal
            RestauraEstadoNormalChaves();
            LogClear();
            Log("Rede em estado normal");
            //cancela moldura na Chave de defeito
            grafico->Moldura((VTChave*)NULL);
            break;
         default:
            Aviso("Controle de conting�ncia com estado inv�lido");
         }
      }catch(Exception &)
         {//nada a fazer
         }
   //reabilita o timer
   Timer->Enabled = true;
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormOpAuto::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //sinaliza fechamento do Form
   if (ActionOnClose) ActionOnClose->Execute();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormOpAuto::LeDados(void)
   {
   //vari�veis locais
   double  valor;

   //verifica op��o de controle dos patamares
   switch(RadioGroupPatamar->ItemIndex)
      {
      case 0: //tempo real: define n�mero de segundos p/ mudan�a de patamar
         ctrl_pat.tempo = 60 * 60;
         //inicia tempo do timer para pr�xima mudan�a de patamar em fun��o da hora atual
         ctrl_pat.timer_count = ctrl_pat.tempo - ((MinuteOf(Now()) * 60) + SecondOf(Now()));
         break;
      case 1: //tempo simulado: l� tempo para mudan�a de patamar
         if (StrToDouble(CBoxTempoPatamar->Text, valor))
            {//define n�mero de segundos p/ mudan�a de patamar em segundos
            ctrl_pat.tempo = int(valor) * 60;
            //inicia tempo do timer para pr�xima mudan�a de patamar
            ctrl_pat.timer_count = ctrl_pat.tempo;
            }
         else
            {
            Aviso("Tempo para alterar patamar com valor inv�lido");
            return(false);
            }
         break;
      case 2: //ap�s simular defeito em todas as chaves: n�o utiliza temporiza��o
         ctrl_pat.tempo       = 0;
         ctrl_pat.timer_count = 0;
         break;
      default:
         Aviso("Selecione o tipo de simula��o: tempo real ou simulado");
         return(false);
      }
   //verifica op��es de controle de conting�ncia
   ctrl_def.estado  = estINICIO;
   //l� tempo de rede no estado normal
   if (StrToDouble(CBoxTempoRedeNormal->Text, valor)) ctrl_def.tempo.rede_normal = int(valor);
   else
      {
      Aviso("Per�odo p/ ocorr�ncia de defeito com valor inv�lido");
      return(false);
      }
   //l� tempo de rede em conting�ncia
   if (StrToDouble(CBoxTempoRedeContingencia->Text, valor)) ctrl_def.tempo.rede_contingencia = int(valor);
   else
      {
      Aviso("Per�odo p/ retornar ao estado normal com valor inv�lido");
      return(false);
      }
   //l� tempo do ciclo de abertura das chaves
   if (StrToDouble(CBoxTempoAbertura1->Text, valor)) ctrl_def.tempo.chave_aberta1 = int(valor);
   else
      {
      Aviso(" Abertura 1: per�odo c/ chave aberta com valor inv�lido");
      return(false);
      }
   //l� tempo do ciclo de abertura das chaves
   if (StrToDouble(CBoxTempoAbertura2->Text, valor)) ctrl_def.tempo.chave_aberta2 = int(valor);
   else
      {
      Aviso(" Abertura 2: per�odo c/ chave aberta com valor inv�lido");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormOpAuto::Log(AnsiString texto)
   {
   RichEdit->Lines->Add(texto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormOpAuto::LogChave(AnsiString texto)
   {
   LabelChave->Caption = texto;
   }

//---------------------------------------------------------------------------
void __fastcall TFormOpAuto::LogClear(void)
   {
   //limpa RichEdit
   RichEdit->Lines->Clear();
   LabelChave->Caption = "";
   }

//---------------------------------------------------------------------------
void __fastcall TFormOpAuto::LogPatamar(AnsiString texto)
   {
   LabelPatamar->Caption = texto;
   }

//---------------------------------------------------------------------------
void __fastcall TFormOpAuto::RadioGroupPatamarClick(TObject *Sender)
   {
   //exibe esconde CBoxTempoPatamar
   CBoxTempoPatamar->Visible = (RadioGroupPatamar->ItemIndex == 1);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormOpAuto::RestauraEstadoNormalChaves(void)
   {
   //vari�veis locais
   VTChave *chave;

   //inicia lisMAN com as Chaves que est�o fora do estado normal
   lisMAN->Clear();
   //copia Chaves da lista de objetos supervisionados
   for (int n = 0; n < lisCHV->Count; n++)
      {
      chave = (VTChave*)lisCHV->Items[n];
      //verifica se a Chave n�o est� no estado normal
      if (chave->Estado != chave->EstadoNormal)
         {//insere Chave em lisMAN
         chave->StatusAsUnsigned = 0;
         lisMAN->Add(chave);
         }
      }
   //verifica se h� Chaves em lisMAN
   if (lisMAN->Count > 0) edita->Manobra(lisMAN);
   //cancela Undo/Redo para evitar o ac�mulo excessivo de objetos
   edita->Clear();

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormOpAuto::SelecionaPatamar(void)
   {
   ctrl_pat.patamar = server->SelecionaProximoPatamar();
   LogPatamar("Hor�rio de simula��o: " + ctrl_pat.patamar->Nome);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormOpAuto::SelecionaChaveDefeito(void)
   {
   //vari�veis locais
   int        index, offset;
   AnsiString txt;
   VTChave    *chave;

   //verifica se j� existe uma Chave selecionada
   if (ctrl_def.chave == NULL) offset = 0;
   else                        offset = lisCHV->IndexOf(ctrl_def.chave) + 1;
   //determina pr�xima Chave no estado fechado
   for (int n = 0; n < lisCHV->Count; n++)
      {
      if ((index = (offset + n)) >= lisCHV->Count)
         {//todas as Chaves foram simuladas
         index = (offset + n) - lisCHV->Count;
         //prote��o:
         if (index > lisCHV->Count) index = 0;
         //verifica se � a primeira Chave da lista
         if (index == 0)
            {//verifica se a mudan�a de patamar deve ser feita ap�s simular defeito em todas Chaves
            if (ctrl_pat.tempo == 0)
               {//seleciona novo patamar
               SelecionaPatamar();
               }
            }
         }
      chave = (VTChave*)lisCHV->Items[index];
      if (chave->Aberta) continue;
      if (chave->EstadoNormal == chvABERTA)
         {
         Aviso("Erro: chave NA selecionada para defeito");
         continue;
         }
      //encontrou Chave fechada
      ctrl_def.chave = chave;
      LogChave(txt.sprintf("Chave %02d/%02d: %s", index+1, lisCHV->Count, chave->Codigo.c_str()));
      return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormOpAuto::TimerTimer(TObject *Sender)
   {
   //vari�veis locais
   AnsiString txt;

   //verifica se est� usando temporiza��o para altera��o de patamar
   if (ctrl_pat.tempo > 0)
      {//decrementa temporiza��o de controle de patamares
      if ((ctrl_pat.timer_count--) == 0)
         {//seleciona novo patamar
         SelecionaPatamar();
         //reinicia temporiza��o
         ctrl_pat.timer_count = ctrl_pat.tempo;
         }
      }
   //verifica se a simula��o de conting�ncia est� habilitada
   if (ctrl_def.enabled)
      {//decrementa temporiza��o atual
      ctrl_def.timer_count--;
      //atualiza LabelTimer
      txt.sprintf("%4d", ctrl_def.timer_count);
      LabelTimer->Caption = txt;
      //verifica se terminou a temporiza��o
      if (ctrl_def.timer_count == 0)
         {//verifica o estado atual
         ExecutaTransicaoControleContingencia();
         }
      }
   }

//---------------------------------------------------------------------------
//eof


