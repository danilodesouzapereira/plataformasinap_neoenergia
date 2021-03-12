//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCelPlanner.h"
#include "VTInterfaceCelPlanner.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
//#include "..\Flow\VTFlow.h"
//#include "..\Patamar\VTPatamar.h"
//#include "..\Patamar\VTPatamares.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Progresso.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCelPlanner::TFormCelPlanner(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro dos objetos
   this->apl = apl_owner;
   if (parent) Parent = parent;
   //cria objetos locais
   celplanner = NewObjInterfaceCelPlanner(apl_owner);
   //gera dados para o CelPlanner
   //ActionGeraDados->Execute(); //FKM 2016.04.27
   //index_patamar = -1;
   //index_pat_usado = -1;
   //fdA4 = 1.;
   //fdTrafo = 1.;
   IniciaForm();
   }

//---------------------------------------------------------------------------
__fastcall TFormCelPlanner::~TFormCelPlanner(void)
   {
   //destrói objetos
   if (celplanner) {delete celplanner; celplanner = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormCelPlanner::ActionGeraDadosExecute(TObject *Sender)
   {//variaveis locais
   //VTFlow  *flow  = (VTFlow*)apl->GetObject(__classid(VTFlow));
   //VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTProgresso *progresso = DLL_NewObjProgresso(this,NULL);

   //salva qual patamar foi usado
   //index_pat_usado = index_patamar;
   if(ExisteRedeBTEquivalente())
      {
      Aviso("Existem redes SDBT não carregadas,"
                        "\nnão é possível exportá-las corretamente para o CelPlanner.");
      }
   else
      {//verifica se os inputs são validos
      //if(ValidaInputForm())
       //  {//configura o interface nix
         ConfiguraInterfaceCelPlanner();
         //prepara o richedit pra update
         SendMessage(RichEdit->Handle,WM_SETREDRAW,false,0);
         //gera dados para CelPlanner
         if(progresso)progresso->Start(progDEFAULT);
         celplanner->ExportaArqCelPlanner(RichEdit->Lines);
         //redesenha o richedit
         SendMessage(RichEdit->Handle,WM_SETREDRAW,true,0);
         RichEdit->Refresh();
         Application->ProcessMessages();
         if(progresso)progresso->Stop();
      //   }
      }
   if(progresso)delete progresso;
   }

//---------------------------------------------------------------------------
//void __fastcall TFormCelPlanner::ActionHelpExecute(TObject *Sender)
//	{
//	//variaveis locais
//	TForm *FormHelp;
//
//	FormHelp = DLL_NewFormHelp(this, apl, "Interface_CelPlanner");
//	if (FormHelp) FormHelp->Show();
//	}

//---------------------------------------------------------------------------
void __fastcall TFormCelPlanner::ActionSalvaArquivoExecute(TObject *Sender)
   {
   //varia´veis locais
   AnsiString filename;
   int        count = 1;
   VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString filepath;
   AnsiString aux = "";


   //verifica se o saveDialog tem um caminho
   if (SaveDialog->InitialDir.IsEmpty())
      {SaveDialog->InitialDir = path->DirExporta();}
   //cria um nome de arquivo
   while (FileExists(filepath = path->DirExporta() + "\\" + ExtractFileName(path->ArqRede) + "_" + IntToStr(count++) + ".txt"));
   filename = ExtractFileName(filepath);

   //seta um nome
   SaveDialog->FileName = filename;
   if(SaveDialog->Execute())
      {//salva dados do RichEdit no arquivo
      RichEdit->Lines->SaveToFile(SaveDialog->FileName);
      }
   }
//---------------------------------------------------------------------------
//void __fastcall TFormCelPlanner::cboxIAlClick(TObject *Sender)
//   {
//   if(cboxIAl->Checked)
//      {
//      clboxPatamar->Enabled = true;
//      clboxPatamarClick(Sender);
//      }
//   else
//      {
//      clboxPatamar->Enabled = false;
//      clboxPatamarClick(Sender);
//      }
//   }
//---------------------------------------------------------------------------
//void __fastcall TFormCelPlanner::cboxFDa4Click(TObject *Sender)
//   {
//   if(cboxFDa4->Checked)
//      {
//      editFDa4->Enabled = true;
//      editFDa4->Visible = true;
//      editFDa4->SetFocus();
//      }
//   else
//      {
//      editFDa4->Enabled = false;
//      editFDa4->Visible = false;
//      }
//   }
//---------------------------------------------------------------------------

//void __fastcall TFormCelPlanner::cboxFDbtClick(TObject *Sender)
//   {
//   if(cboxFDbt->Checked)
//      {
//      editFDbt->Enabled = true;
//      editFDbt->Visible = true;
//      editFDbt->SetFocus();
//      }
//   else
//      {
//      editFDbt->Enabled = false;
//      editFDbt->Visible = false;
//      }
//   }
//---------------------------------------------------------------------------
//void __fastcall TFormCelPlanner::clboxPatamarClick(TObject *Sender)
//   {
//   if(clboxPatamar->Checked)
//      {
//      if(!IniciaCBoxPatamar())
//         {
//         DesabilitaCombo();
//         }
//      }
//   else
//      {
//      DesabilitaCombo();
//      }
//   }
//---------------------------------------------------------------------------
//void __fastcall TFormCelPlanner::comboPatamarChange(TObject *Sender)
//   {//variaveis locais
//   VTPatamar *patamar;
//   int indice;
//
//   indice = comboPatamar->ItemIndex;
//   //seleciona o objeto
//   patamar = (VTPatamar*) comboPatamar->Items->Objects[indice];
//   if(patamar != NULL)
//      {
//      index_patamar = patamar->Index;
//      }
//   }
//---------------------------------------------------------------------------
void  __fastcall TFormCelPlanner::ConfiguraInterfaceCelPlanner(void)
   {
   if (celplanner != NULL)
      {
      //
      celplanner->exp_Cargas = checkCargas->Checked;
      celplanner->exp_Chaves = checkChaves->Checked;
      celplanner->exp_Geradores = checkGeradores->Checked;
      celplanner->exp_Reguladores = checkReguladores->Checked;
      celplanner->exp_Capacitores = checkCapacitores->Checked;
      celplanner->exp_Religadores = checkReligadores->Checked;
      celplanner->exp_Seccionadoras = checkSeccionadoras->Checked;
      celplanner->exp_Subestacao = checkSubestacao->Checked;
      //celplanner->exp_Trafos = checkTrafos->Checked;
      //arrrumar
      if(radioCoordenadas->ItemIndex == 0)//utm
         {
         celplanner->is_UTM = true;
         }
      if(radioCoordenadas->ItemIndex == 1)//GMS
         {
         celplanner->is_UTM = false;
         }

      }
   }
//---------------------------------------------------------------------------
//void  __fastcall TFormCelPlanner::DesabilitaCombo(void)
//   {
//   comboPatamar->Clear();
//   comboPatamar->Text = "Patamar de demanda máxima";
//   comboPatamar->Enabled = false;
//   index_patamar = -1;
//   }
//---------------------------------------------------------------------------
bool  __fastcall TFormCelPlanner::ExisteRedeBTEquivalente(void)
   {//variáveis locais
   VTRedes    *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTRede     *rede;
   VTTipoRede *tipoRede;

   //varre todas as redes à procura de redes bt reduzidas
   for(int nr =0; nr < redes->LisRede()->Count ; nr++)
      {
      rede = (VTRede*)redes->LisRede()->Items[nr];
      tipoRede = rede->TipoRede;
      if(tipoRede != NULL)
         {//se a rede for redeSEC
         if(tipoRede->Segmento == redeSEC)
            {//caso seja rede equivalente
            if(!rede->Carregada)
               {
               return (true);
               }
            }
         }
      }
   //caso tenha varrido todas e não encontrado
   return (false);
   }
//---------------------------------------------------------------------------
//bool  __fastcall TFormCelPlanner::IniciaCBoxPatamar(void)
//   {//variaveis locais
//   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
//   VTPatamar   *patamar;
//   TList       *lisPAT = new TList;
//   bool        sucesso = false;
//
//   ;
//   //preenche o combo com todos patamares disponivels
//   patamares->LisPatamarHabilitado(lisPAT);
//   //caso tenha pelo menos 1 patamar
//   if(lisPAT->Count > 0)
//      {
//      //limpa  combo
//      comboPatamar->Clear();
//      for(int np = 0; np < lisPAT->Count; np++)
//         {
//         patamar = (VTPatamar*) lisPAT->Items[np];
//         //adiciona o nome e ponteiro
//         comboPatamar->AddItem(patamar->Nome, patamar);
//         }
//      //seleciona o primeior
//      if(comboPatamar->Items->Count > 0)
//         {
//         comboPatamar->ItemIndex = 0;
//         //habilita o combo
//         comboPatamar->Enabled = true;
//         //salva o index do patamar
//         index_patamar = ((VTPatamar*) comboPatamar->Items->Objects[0])->Index;
//         sucesso = true;
//         }
//      }
//   return sucesso;
//   }

//---------------------------------------------------------------------------
bool  __fastcall TFormCelPlanner::IniciaForm(void)
   {
   if (celplanner != NULL)
      {
      //
      checkCargas->Checked = celplanner->exp_Cargas;
      checkChaves->Checked = celplanner->exp_Chaves;
      checkGeradores->Checked = celplanner->exp_Geradores;
      checkReligadores->Checked = celplanner->exp_Religadores;
      checkReguladores->Checked = celplanner->exp_Reguladores;
      checkCapacitores->Checked = celplanner->exp_Capacitores;
      checkSeccionadoras->Checked = celplanner->exp_Seccionadoras;
      checkSubestacao->Checked = celplanner->exp_Subestacao;
      //checkTrafo->Checked = celplanner->exp_Trafos;
      //arrrumar
      celplanner->is_UTM = false;
      }
   return(true);
   }
//---------------------------------------------------------------------------
void __fastcall TFormCelPlanner::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
//bool  __fastcall TFormCelPlanner::ValidaFatorDemandaA4(void)
//   {//variaveis locais
//   bool sucesso = true;
//   AnsiString  string;
//
//   string = editFDa4->Text;
//   if(!StrToDouble(string, fdA4))
//      {
//      Aviso("Valor de Fator de Demanda inválido.");
//      sucesso = false;
//      }
//   return sucesso;
//   }
//---------------------------------------------------------------------------
//bool  __fastcall TFormCelPlanner::ValidaFatorDemandaTrafo(void)
//   {//variaveis locais
//   bool sucesso = true;
//   AnsiString  string;
//
//   string = editFDbt->Text;
//   if(!StrToDouble(string, fdTrafo))
//      {
//      Aviso("Valor de Fator de Demanda inválido.");
//      sucesso = false;
//      }
//   return sucesso;
//   }
//---------------------------------------------------------------------------
//bool  __fastcall TFormCelPlanner::ValidaInputForm(void)
//   {//variaveis locais
//   bool sucesso = true;
//
//   try
//      {
//      //se for forçar FD
//      if(cboxFDbt->Checked)
//         {//valida os valores de demanda
//         if(!ValidaFatorDemandaTrafo())
//            {sucesso = false;}
//         }
//      //se for forçar FD
//      if(cboxFDa4->Checked)
//         {//valida os valores de demanda
//         if(!ValidaFatorDemandaA4())
//            {sucesso = false;}
//         }
//      }
//   catch (Exception &e)
//      {
//      sucesso = false;
//      }
//   return sucesso;
//   }
//---------------------------------------------------------------------------
//eof
//void __fastcall TFormCelPlanner::editFDbtMouseActivate(TObject *Sender, TMouseButton Button,
//          TShiftState Shift, int X, int Y, int HitTest, TMouseActivate &MouseActivate)
//
//   {
//   editFDbt->SetFocus();
//   }
//---------------------------------------------------------------------------

//void __fastcall TFormCelPlanner::editFDa4MouseActivate(TObject *Sender, TMouseButton Button,
//          TShiftState Shift, int X, int Y, int HitTest, TMouseActivate &MouseActivate)
//
//   {
//   editFDa4->SetFocus();
//   }
//---------------------------------------------------------------------------





