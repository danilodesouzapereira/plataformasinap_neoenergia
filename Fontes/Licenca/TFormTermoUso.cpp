//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormTermoUso.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Produto\VTProduto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormTermoUso::TFormTermoUso(TComponent* Owner, VTApl *apl, int tipo_licenca)
   : TForm(Owner)
   {
   //variáveis locais
   AnsiString arq;
   VTPath     *path    = (VTPath*)apl->GetObject(__classid(VTPath));
   VTProduto  *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

   //atualiza  Caption
   Caption = produto->Codigo + ": Termos de Uso";
   //carrega arquivo c/ termo de uso
   switch (tipo_licenca)
      {
      case tipoDEMO:
         arq = path->ArqTermoUsoDemo();
         break;
      case tipoDIDATICA:
         arq = path->ArqTermoUsoDidatico();
         break;
      default:
         arq = path->ArqTermoUso();
         break;
      }
   //carrega arquivo c/ termo de uso
   if (FileExists(arq))RichEdit->Lines->LoadFromFile(arq);
   }

//---------------------------------------------------------------------------
__fastcall TFormTermoUso::~TFormTermoUso(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormTermoUso::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormTermoUso::ActionConfirmaExecute(TObject *Sender)
   {
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormTermoUso::FormCloseQuery(TObject *Sender,  bool &CanClose)
   {
   CanClose = CheckBox->Checked;
   }

//---------------------------------------------------------------------------
void __fastcall TFormTermoUso::CheckBoxClick(TObject *Sender)
   {
   ActionConfirma->Enabled = CheckBox->Checked;
   CheckBox->Font->Color   = (CheckBox->Checked) ? clGreen : clRed;
   }

//---------------------------------------------------------------------------
//eof

