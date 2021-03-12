//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormAnafas.h"
#include "TAplAnafas.h"
#include "VTAnafas.h"
#include "..\Apl\VTApl.h"
#include "..\Log\VTLog.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//---------------------------------------------------------------------------
__fastcall TFormAnafas::TFormAnafas(TComponent *Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
   try{//cria objeto AplAnafas
      apl    = new TAplAnafas(this, apl_owner);
      lisEQP = new TList();
      //desabilita ActionConfirma
      ActionConfirma->Enabled = false;
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TFormAnafas::~TFormAnafas(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   //destrói objetos
   if (apl) {delete apl; apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnafas::ActionCancelaExecute(TObject *Sender)
   {
   //cancela importação
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnafas::ActionConfirmaExecute(TObject *Sender)
   {
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnafas::ActionImportaExecute(TObject *Sender)
   {
   //variáveis locais
   VTAnafas  *anafas  = (VTAnafas*)apl->GetObject(__classid(VTAnafas));
   VTPath    *path    = (VTPath*)apl->GetObject(__classid(VTPath));
   VTLog     *plog    = (VTLog*)apl->GetObject(__classid(VTLog));

   //verifica se o RichEdit está vazio
   if (redArq->Lines->Count == 0)
      {
      Aviso("Selecione arquivo c/ dados da rede");
      return;
      }
   //executa importação dos dados
   plog->Open(path->DirTmp() + "\\Anafas.log");
   ActionConfirma->Enabled = anafas->Executa(redArq->Lines);
   plog->Close();
   if (anafas->NumeroErros() > 0)
      {//erro: mostra arquivo de Log
      redArq->Lines->Clear();
      redArq->Lines->LoadFromFile(plog->ArqLog());
      //monta resumo da Rede
      MontaResumo();
      Aviso("Existência de erros na importação da rede.\n"
            "Verifique os problemas no relatório.");
      }
   else
      {//importação sem erros: monta resumo da Rede
      MontaResumo();
      Aviso("Importação da rede feita com sucesso.");
      //fecha o form
      ActionConfirmaExecute(NULL);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnafas::ActionSelArqExecute(TObject *Sender)
   {
   //variáveis locais
//   VTPath *path = (VTPath*)apl->GetObject("TPath");

   //seleciona diretório inicial
//   if (OpenDialog->InitialDir.IsEmpty()) OpenDialog->InitialDir = path->DirImporta();
//   if (OpenDialog->InitialDir.IsEmpty()) OpenDialog->InitialDir = path->DirBase();
   //seleciona arquivo TXT
   if (! OpenDialog->Execute()) return;
   //usuário selecionou um arquivo: exibe nome do arquivo no Status bar
   StatusBar->SimpleText = OpenDialog->FileName;
   //exibe conteúdo do arquivo no RichEdit
   redArq->Lines->LoadFromFile(OpenDialog->FileName);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnafas::MontaResumo(void)
   {
   //variáveis locais
   AnsiString txt;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //apresenta resumo da Rede
   redArq->Lines->Add(AnsiString::StringOfChar('=', 80));
   redArq->Lines->Add("");
   redArq->Lines->Add("       Resumo da Rede        ");
   redArq->Lines->Add("");
   redArq->Lines->Add(AnsiString::StringOfChar('-', 30));
   redArq->Lines->Add("Equipamento      Quantidade");
   //número de Barras
   lisEQP->Clear();
   redes->LisBarra(lisEQP);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Barra", lisEQP->Count));
   //número de Trechos
   lisEQP->Clear();
   redes->LisLigacao(lisEQP, eqptoTRECHO);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Trecho", lisEQP->Count));
   //número de Mútuas
   lisEQP->Clear();
   redes->LisEqpto(lisEQP, eqptoMUTUA);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Mútua", lisEQP->Count));
   //número de Trafos
   lisEQP->Clear();
   redes->LisLigacao(lisEQP, eqptoTRAFO);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Trafo", lisEQP->Count));
   //número de Cap.Série
   lisEQP->Clear();
   redes->LisLigacao(lisEQP, eqptoCAPSERIE);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Cap.Série", lisEQP->Count));
   //número de Suprimentos
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoSUPRIMENTO);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Suprimento", lisEQP->Count));
   //número de Cargas
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoCARGA);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Carga", lisEQP->Count));
   //número de Capacitores
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoCAPACITOR);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Capacitor", lisEQP->Count));
   //número de Reatores
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoREATOR);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Reator", lisEQP->Count));
   redArq->Lines->Add(AnsiString::StringOfChar('-', 30));
   }
//---------------------------------------------------------------------------
void __fastcall TFormAnafas::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnafas::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   Left = 0;
   Top  = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnafas::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
//eof


