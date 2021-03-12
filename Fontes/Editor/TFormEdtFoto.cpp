//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtFoto.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdtFoto::TFormEdtFoto(TComponent* Owner, VTApl *apl, VTEqpto *eqpto)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl    = apl;
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtFoto::~TFormEdtFoto(void)
   {//vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFoto::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFoto::ActionConfirmaExecute(TObject *Sender)
   {
   //atualiza base de dados
   //fecha o form
   ModalResult = mrOk;
   //
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFoto::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      geral->Infoset->LoadFromFile(this);
      }
   }

//---------------------------------------------------------------------------
//eof

