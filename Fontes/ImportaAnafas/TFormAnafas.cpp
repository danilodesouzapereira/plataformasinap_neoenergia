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
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destr�i lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   //destr�i objetos
   if (apl) {delete apl; apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnafas::ActionCancelaExecute(TObject *Sender)
   {
   //cancela importa��o
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
   //vari�veis locais
   VTAnafas  *anafas  = (VTAnafas*)apl->GetObject(__classid(VTAnafas));
   VTPath    *path    = (VTPath*)apl->GetObject(__classid(VTPath));
   VTLog     *plog    = (VTLog*)apl->GetObject(__classid(VTLog));

   //verifica se o RichEdit est� vazio
   if (redArq->Lines->Count == 0)
      {
      Aviso("Selecione arquivo c/ dados da rede");
      return;
      }
   //executa importa��o dos dados
   plog->Open(path->DirTmp() + "\\Anafas.log");
   ActionConfirma->Enabled = anafas->Executa(redArq->Lines);
   plog->Close();
   if (anafas->NumeroErros() > 0)
      {//erro: mostra arquivo de Log
      redArq->Lines->Clear();
      redArq->Lines->LoadFromFile(plog->ArqLog());
      //monta resumo da Rede
      MontaResumo();
      Aviso("Exist�ncia de erros na importa��o da rede.\n"
            "Verifique os problemas no relat�rio.");
      }
   else
      {//importa��o sem erros: monta resumo da Rede
      MontaResumo();
      Aviso("Importa��o da rede feita com sucesso.");
      //fecha o form
      ActionConfirmaExecute(NULL);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnafas::ActionSelArqExecute(TObject *Sender)
   {
   //vari�veis locais
//   VTPath *path = (VTPath*)apl->GetObject("TPath");

   //seleciona diret�rio inicial
//   if (OpenDialog->InitialDir.IsEmpty()) OpenDialog->InitialDir = path->DirImporta();
//   if (OpenDialog->InitialDir.IsEmpty()) OpenDialog->InitialDir = path->DirBase();
   //seleciona arquivo TXT
   if (! OpenDialog->Execute()) return;
   //usu�rio selecionou um arquivo: exibe nome do arquivo no Status bar
   StatusBar->SimpleText = OpenDialog->FileName;
   //exibe conte�do do arquivo no RichEdit
   redArq->Lines->LoadFromFile(OpenDialog->FileName);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnafas::MontaResumo(void)
   {
   //vari�veis locais
   AnsiString txt;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //apresenta resumo da Rede
   redArq->Lines->Add(AnsiString::StringOfChar('=', 80));
   redArq->Lines->Add("");
   redArq->Lines->Add("       Resumo da Rede        ");
   redArq->Lines->Add("");
   redArq->Lines->Add(AnsiString::StringOfChar('-', 30));
   redArq->Lines->Add("Equipamento      Quantidade");
   //n�mero de Barras
   lisEQP->Clear();
   redes->LisBarra(lisEQP);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Barra", lisEQP->Count));
   //n�mero de Trechos
   lisEQP->Clear();
   redes->LisLigacao(lisEQP, eqptoTRECHO);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Trecho", lisEQP->Count));
   //n�mero de M�tuas
   lisEQP->Clear();
   redes->LisEqpto(lisEQP, eqptoMUTUA);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "M�tua", lisEQP->Count));
   //n�mero de Trafos
   lisEQP->Clear();
   redes->LisLigacao(lisEQP, eqptoTRAFO);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Trafo", lisEQP->Count));
   //n�mero de Cap.S�rie
   lisEQP->Clear();
   redes->LisLigacao(lisEQP, eqptoCAPSERIE);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Cap.S�rie", lisEQP->Count));
   //n�mero de Suprimentos
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoSUPRIMENTO);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Suprimento", lisEQP->Count));
   //n�mero de Cargas
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoCARGA);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Carga", lisEQP->Count));
   //n�mero de Capacitores
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoCAPACITOR);
   redArq->Lines->Add(txt.sprintf("%-15s  %8d", "Capacitor", lisEQP->Count));
   //n�mero de Reatores
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

//---------------------------------------------------------------------------
void __fastcall TFormAnafas::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
//eof


