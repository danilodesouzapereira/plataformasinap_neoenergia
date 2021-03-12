//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormLogin.h"
#include "..\Apl\VTApl.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Produto\VTProduto.h"
#include "..\Licenca\VTLicenca.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormLogin::TFormLogin(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   }

//---------------------------------------------------------------------------
__fastcall TFormLogin::~TFormLogin(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormLogin::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLogin::ActionConfirmaExecute(TObject *Sender)
   {
   //vari�veis locais
   VTProduto  *produto  = (VTProduto*)apl->GetObject(__classid(VTProduto));
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca  *licenca  = licencas->ExisteLicenca();

   //verifica se o usu�rio preencheu os dados
   if (EditLogin->Text.IsEmpty())
      {
      Aviso("Preencha o campo de identifica��o do usu�rio!");
      return;
      }
   if (EditPassword->Text.IsEmpty())
      {
      Aviso("Preencha o campo de senha de acesso!");
      return;
      }
   //verifica se existe a Licenca
   if (licenca == NULL)
      {
      Aviso("N�o existe Licen�a de Uso para o programa " + produto->Codigo);
      return;
      }
   //verifica se os dados coincidem c/ a Licenca
   if (licenca->Login.AnsiCompare(EditLogin->Text) != 0)
      {
      Aviso("Identifica��o do usu�rio incorreta.");
      return;
      }
   if (licenca->Password.AnsiCompare(EditPassword->Text) != 0)
      {
      Aviso("Senha de acesso incorreta.");
      return;
      }
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
//eof

