//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormLicenca.h"
#include "..\Apl\VTApl.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormLicenca::TFormLicenca(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro para Apl
   this->apl = apl;
   //exibe dados da Licen�a
   DadosDaLicenca();
   }

//---------------------------------------------------------------------------
__fastcall TFormLicenca::~TFormLicenca(void)
   {
   //
   }

//---------------------------------------------------------------------------
void __fastcall TFormLicenca::ButFechaClick(TObject *Sender)
   {
   ModalResult = mrOk;
   }

//-----------------------------------------------------------------------------
void __fastcall TFormLicenca::DadosDaLicenca(void)
   {
   //vari�veis locais
   VTLicenca  *licenca;
   TStrings   *lines  = RichEdit->Lines;
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   AnsiString status[]  = {"livre", "alocada", "expirada", "bloqueada", "inexistente"};

   //verifica se existe uma Licenca p/ o produto
   if ((licenca = licencas->ExisteLicenca()) == NULL)
      {
      lines->Add("N�o existe Licen�a de Uso alocada");
      return;
      }
   //insere dados da Licenca
   lines->Add("Empresa:       " + licenca->EmpresaStr);
   lines->Add("Usu�rio:       " + licenca->Login);
   lines->Add("Senha:         " + AnsiString::StringOfChar('*', licenca->Password.Length()));
   lines->Add("Data aloca��o: " + DateToStr(licenca->DH_alocada));
   if (licenca->Validade < 0)
      {lines->Add("Validade:      n�o limitada");}
   else
      {lines->Add("Validade:      " + IntToStr(licenca->Validade) + " dias");}
   lines->Add("Status:        " + status[licenca->Status]);
   lines->Add("Google Maps:   " + ((licenca->Google) ? AnsiString("habilitado") : AnsiString("desabilitado")));
   }

//---------------------------------------------------------------------------
//eof

