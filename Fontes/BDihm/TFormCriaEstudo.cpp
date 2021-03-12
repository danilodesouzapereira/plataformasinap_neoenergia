//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCriaEstudo.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\BD\VTBD.h>
#include <Fontes\BD\Rede\VTBDeqpto.h>
#include <Fontes\Planejamento\VTEstudo.h>
#include <Fontes\Planejamento\VTPlanejamento.h>

#include <Fontes\Sinap\VTSinapMdi.h>
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCriaEstudo::TFormCriaEstudo(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
	{
	//variáveis locais
	VTSinapMdi *sinap_mdi = (VTSinapMdi*)apl->GetObject(__classid(VTSinapMdi));

	//salva ponteiro p/ objeto
   this->apl = apl;
   //inicia autor
	EditAutor->Text = sinap_mdi->UsuarioNome();
   //inicia DateTimePicker
   DateTimePicker->Date = Now();
   //define nome para o estudo
   EditCodigo->Text = DefineCodigoEstudo();
   }

//---------------------------------------------------------------------------
__fastcall TFormCriaEstudo::~TFormCriaEstudo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormCriaEstudo::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCriaEstudo::ActionConfirmaExecute(TObject *Sender)
   {
   //valida entrada de dado
   if ((Trim(EditCodigo->Text)).IsEmpty())
      {
      Aviso("Defina o código do estudo");
      return;
      }
   if ((Trim(EditAutor->Text)).IsEmpty())
      {
      Aviso("Defina o autor do estudo");
      return;
      }
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormCriaEstudo::DefineCodigoEstudo(void)
   {
   //variáveis locais
   int            count;
   AnsiString     codigo;
   VTBD           *bd           = (VTBD*)apl->GetObject(__classid(VTBD));
   VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

   //verifica se existe a base de dados
   if (bd == NULL) return("Estudo Novo");
   //obtém lista de Estudos existentes na base de dados
   bd->Eqpto->LeListaEstudo();
   //define código diferente dos Estudo existentes
   count = (planejamento->LisEstudo())->Count;
   do {
      codigo = "Estudo " + IntToStr(count++);
      } while (planejamento->ExisteEstudo(codigo) != NULL);
   return(codigo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCriaEstudo::EditCodigoKeyPress(TObject *Sender, char &Key)
   {
   //verifica se foi digitada a tecla Enter
   if (Key == VK_RETURN) ActionConfirma->Execute();
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormCriaEstudo::PM_GetAutor(void)
   {
   return(EditAutor->Text);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormCriaEstudo::PM_GetCodigo(void)
   {
   return(EditCodigo->Text);
   }

//---------------------------------------------------------------------------
TDateTime __fastcall TFormCriaEstudo::PM_GetData(void)
   {
   return(DateTimePicker->Date);
   }

//---------------------------------------------------------------------------
//eof

