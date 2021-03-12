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
   //variáveis locais
   VTProduto  *produto  = (VTProduto*)apl->GetObject(__classid(VTProduto));
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca  *licenca  = licencas->ExisteLicenca();

   //verifica se o usuário preencheu os dados
   if (EditLogin->Text.IsEmpty())
      {
      Aviso("Preencha o campo de identificação do usuário!");
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
      Aviso("Não existe Licença de Uso para o programa " + produto->Codigo);
      return;
      }
   //verifica se os dados coincidem c/ a Licenca
   if (licenca->Login.AnsiCompare(EditLogin->Text) != 0)
      {
      Aviso("Identificação do usuário incorreta.");
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

