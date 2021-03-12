//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Constante\Const.h>
#include <Fontes\Planejamento\VTAlternativa.h>
#include <Fontes\Planejamento\VTEstudo.h>
#include <Fontes\Planejamento\VTPlanejamento.h>
#include <DLL_Inc\Funcao.h>
#include "TFormResumoTec.h"
#include "VTResumoTec.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormResumoTec::TFormResumoTec(TComponent* Owner, VTEstudo *estudo)
   : TForm(Owner)
   {
   //inicia LView
   LViewInicia(estudo);
   }

//---------------------------------------------------------------------------
__fastcall TFormResumoTec::~TFormResumoTec(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormResumoTec::ActionImprimeExecute(TObject *Sender)
   {
   //
   }

//---------------------------------------------------------------------------
void __fastcall TFormResumoTec::ActionFechaExecute(TObject *Sender)
   {
   //fecha o Form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormResumoTec::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormResumoTec::LViewInicia(VTEstudo *estudo)
   {
   //variáveis locais
   TList          *lisALTERNATIVA;
   TList          *lisOBRA;
   TListItem      *Item;
   VTAlternativa  *alternativa;
   VTResumoTec    *resumo;
   TListView      *LView  = LViewAnaTec;

   //reinicia LView
   LView->Items->Clear();
   //proteção
   if (estudo == NULL) return;
   //loop p/ todas Alternativas do Estudo
   lisALTERNATIVA = estudo->LisAlternativa();
   for (int n = 0; n < lisALTERNATIVA->Count; n++)
      {
      alternativa = (VTAlternativa*)lisALTERNATIVA->Items[n];
      resumo      = (VTResumoTec*)alternativa->Apl->GetObject(__classid(VTResumoTec));
    for (int ano = resumo->AnoInicial; ano < resumo->AnoFinal; ano++)
         {//insere ListItem
         Item = LView->Items->Add();
         Item->Data = alternativa;
         Item->Checked = alternativa->Valida;
         Item->Caption = alternativa->Codigo;
		 Item->SubItems->Add(IntToStr(ano));
		 //barras c/ tensão precaria
		 Item->SubItems->Add(DoubleToStr("%3.2f", resumo->DiagBarra[ano][diagREGULAR]));
         //barras c/ tensão crítica
		 Item->SubItems->Add(DoubleToStr("%3.2f", resumo->DiagBarra[ano][diagRUIM]));
		 //trafos c/ cargto crítico
		 Item->SubItems->Add(DoubleToStr("%3.2f", resumo->DiagTrafo[ano][diagREGULAR]));
         //trafos c/ cargto crítico
		 Item->SubItems->Add(DoubleToStr("%3.2f", resumo->DiagTrafo[ano][diagRUIM]));
		 //trechos  / cargto crítico
		 Item->SubItems->Add(DoubleToStr("%3.2f", resumo->DiagTrechoKm[ano][diagREGULAR]));
		 //trechos  / cargto crítico
		 Item->SubItems->Add(DoubleToStr("%3.2f", resumo->DiagTrechoKm[ano][diagRUIM]));
         //energia recebida
         Item->SubItems->Add(DoubleToStr("%3.2f", resumo->Entrada_mwhm[ano]));
         //energia gerada
         Item->SubItems->Add(DoubleToStr("%3.2f", resumo->Gerada_mwhm[ano]));
         //energia consumida
         Item->SubItems->Add(DoubleToStr("%3.2f", resumo->Entregue_mwhm[ano]));
         //perda
         Item->SubItems->Add(DoubleToStr("%3.2f", resumo->Perda_mwhm[ano]));
			//DEC
			Item->SubItems->Add(DoubleToStr("%3.2f", resumo->DEC[ano])); //DVK 2015.08.12
			//FEC
			Item->SubItems->Add(DoubleToStr("%3.2f", resumo->FEC[ano])); //DVK 2015.08.12
			//END
			Item->SubItems->Add(DoubleToStr("%3.2f", resumo->END[ano]));
         }
      }
   }

//---------------------------------------------------------------------------
//eof

