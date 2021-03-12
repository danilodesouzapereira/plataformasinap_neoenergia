//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormSobre.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Produto\VTProduto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormSobre::TFormSobre(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //variáveis locais
   //VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));
   VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

   //atualiza Caption
   Caption               = "Sobre o programa " + produto->Codigo;
   LabelRevisao->Caption = "Revisão: " + produto->CodRevSeqData;
   }

//---------------------------------------------------------------------------
__fastcall TFormSobre::~TFormSobre(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormSobre::butFechaClick(TObject *Sender)
   {
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSobre::ButSiteSinapsisClick(TObject *Sender)
   {
   //inicia Broswser p/ acessar o site da SInapsis
   }

//---------------------------------------------------------------------------
void __fastcall TFormSobre::FormClose(TObject *Sender, TCloseAction &Action)   {
   //apenas esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
//eof

