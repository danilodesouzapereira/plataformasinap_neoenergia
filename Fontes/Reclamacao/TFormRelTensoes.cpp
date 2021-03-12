//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "TFormRelTensoes.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Report\VTReport.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Report.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFormRelTensoes::TFormRelTensoes(TComponent* Owner, VTApl *apl,
                            TLineSeries* SeriesVA, TLineSeries* SeriesVB,
                            TLineSeries* SeriesVC, bool tensao)
   : TForm(Owner)
   {
   //salva objs
   this->apl = apl;
   this->SeriesVA = SeriesVA;
   this->SeriesVB = SeriesVB;
   this->SeriesVC = SeriesVC;
   //altera título do form e colunas do LView
   if(! tensao)
      {
      this->Caption = "Relatório de correntes";
      LViewTensoes->Column[2]->Caption = "Ia";
      LViewTensoes->Column[3]->Caption = "Ib";
      LViewTensoes->Column[4]->Caption = "Ic";
      }
   //insere valores no LView
   PreencheLView();
   }

//---------------------------------------------------------------------------
__fastcall TFormRelTensoes::~TFormRelTensoes(void)
   {
   //nada
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelTensoes::ActionExportaExecute(TObject *Sender)
   {
   //variaveis locais
   AnsiString   nome_arq;
   VTReport      *report;

   //cria objeto report
   report = DLL_NewObjReport(apl);
   //esconde form
   Hide();
   Application->ModalPopupMode = pmAuto;
   if(! SaveDialogAnalise->Execute()) {Show(); return;}
   //exporta em arq. txt
   nome_arq = ExtractFileName(SaveDialogAnalise->FileName);
//   if (report->ArquivoTexto(nome_arq, LViewTensoes, "RelatorioTensoesRNT", false))
   if (report->ArquivoTexto(SaveDialogAnalise->FileName, LViewTensoes, "RelatorioTensoesRNT", false))
      Aviso("A tabela foi salva em " + SaveDialogAnalise->FileName);
   else
      Erro("Erro ao criar arquivo.");
   //destroi report
   if(report) {DLL_DeleteObjReport(report); report = NULL;}
   //reexibe form
   Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelTensoes::ActionFecharExecute(TObject *Sender)
   {
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelTensoes::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //fecha o form
   ModalResult = mrOk;
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelTensoes::FormShow(TObject *Sender)
   {
   //nada
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelTensoes::PreencheLView(void)
   {
   //variaveis locais
   TListItem    *item;
   double       va, vb, vc;
   int         total_pontos;
   TDateTime    data, hora;

   //limpa
   LViewTensoes->Clear();
   va = vb = vc = 0.0;
   //verifica quantos pontos tem as series
   total_pontos = Max(SeriesVA->Count(), SeriesVB->Count());
   total_pontos = Max(SeriesVC->Count(), total_pontos);
   //insere no LView
   for(int np = 0; np < total_pontos; np+=2)
      {//os pontos estão duplicados: insere só os np pares
      if   (SeriesVA->Visible)
         {
         data = ((TDateTime)SeriesVA->XValue[np]);
         va = SeriesVA->YValue[np];
         }
      if(SeriesVB->Visible)
         {
         data = ((TDateTime)SeriesVB->XValue[np]);
         vb = SeriesVB->YValue[np];
         }
      if(SeriesVC->Visible)
         {
         data = ((TDateTime)SeriesVC->XValue[np]);
         vc = SeriesVC->YValue[np];
         }
      item = LViewTensoes->Items->Add();
      item->Caption    =      data.DateString();
      item->SubItems->Add(data.TimeString());
      item->SubItems->Add(DoubleToStr("%3.2f",va));
      item->SubItems->Add(DoubleToStr("%3.2f",vb));
      item->SubItems->Add(DoubleToStr("%3.2f",vc));
      }
   }

//---------------------------------------------------------------------------

