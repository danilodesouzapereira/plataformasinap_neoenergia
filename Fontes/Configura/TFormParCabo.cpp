//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormParCabo.h"
#include "..\..\Util\Sisplan\Funcao.h"
#include "..\..\Util\Rede\TCabo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TformParCabo *formParCabo;
//---------------------------------------------------------------------------
__fastcall TformParCabo::TformParCabo(TComponent* Owner, TCabo *pCabo)
        : TForm(Owner)
   {
   this->pCabo = pCabo;
   loadCabo();
   }

//---------------------------------------------------------------------------
void __fastcall TformParCabo::ActionCancelaExecute(TObject *Sender)
   {

   pCabo=oldCabo;
   ModalResult = mrCancel;

   }
//---------------------------------------------------------------------------
void __fastcall TformParCabo::ActionConfirmaExecute(TObject *Sender)
   {

   if(saveCabo())
      {
      ModalResult = mrOk;
      }
   }
//---------------------------------------------------------------------------
void __fastcall TformParCabo::edtIadmKeyPress(TObject *Sender, char &Key)
   {

   switch (Key)
        {
        case 'E':
        case 'e':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
        case ',':
        case VK_DECIMAL:
        case VK_RETURN:
        case VK_BACK:
        case VK_TAB:
                break;

        default:
                Key=0;
        }

   }
//---------------------------------------------------------------------------
void __fastcall TformParCabo::loadCabo(void)
   {

   edtCabo->Text =pCabo->codigo;
   edtIadm->Text =DoubleToStr("%8.2f", pCabo->iadm);
   edtR0->Text =DoubleToStr("%8.2f", pCabo->r0);
   edtX0->Text =DoubleToStr("%8.2f", pCabo->x0);
   edtC0->Text =DoubleToStr("%8.2f", pCabo->c0);
   edtR1->Text =DoubleToStr("%8.2f", pCabo->r1);
   edtC1->Text =DoubleToStr("%8.2f", pCabo->c1);
   pnlCor->Color=pCabo->color;

   }
//---------------------------------------------------------------------------
void __fastcall TformParCabo::pnlCorClick(TObject *Sender)
   {

   //apresenta form de seleção de cor
   dlgSelCor->Color = pnlCor->Color;
   if (dlgSelCor->Execute())
      {
      pnlCor->Color = dlgSelCor->Color;
      }

   }
//---------------------------------------------------------------------------
bool __fastcall TformParCabo::saveCabo(void)
   {

   try
      {
      pCabo->codigo = edtCabo->Text;
      pCabo->iadm = StrToDouble(edtIadm->Text);
      pCabo->r0 = StrToDouble(edtR0->Text);
      pCabo->x0 = StrToDouble(edtX0->Text);
      pCabo->c0 = StrToDouble(edtC0->Text);
      pCabo->r1 = StrToDouble(edtR1->Text);
      pCabo->c1 = StrToDouble(edtC1->Text);
      pCabo->color = pnlCor->Color;
      return true;
      }
   catch (EConvertError &exception)
      {
      Erro("Erro nos valores digitados. Por Favor Corrija.");
      return false;
      }

   }
//---------------------------------------------------------------------------

