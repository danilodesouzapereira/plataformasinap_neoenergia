//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "TFormMedicao.h"
#include <Fontes\Apl\VTApl.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMedicao *FormMedicao;
//---------------------------------------------------------------------------
__fastcall TFormMedicao::TFormMedicao(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiros
   this->apl = apl;
   Inicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormMedicao::~TFormMedicao(void)
   {
   //
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::ActionConfirmaExecute(TObject *Sender)
   {
   //
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::ActionCancelaExecute(TObject *Sender)
   {
   //
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::ActionSelArqExecute(TObject *Sender)
   {
   //variaveis locais
   AnsiString       codigo;
   TStringList    *lines  = new TStringList();

   //abre arquivo de medição
   if(! OpenDialog->Execute())  return;
   //carrega dados do arquivo txt
   lines->LoadFromFile(OpenDialog->FileName);
   LBoxLines->Clear();
   LBoxLines->Items->AddStrings(lines);

   if (lines)  {delete lines;  lines  = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::Inicia(void)
   {
   //preenche CboxTipos com os tipos de medição
   CBoxTipos->Clear();
   CBoxTipos->AddItem("Landis+Gyr", NULL);
   CBoxTipos->AddItem("SAGA4000", NULL);
   CBoxTipos->AddItem("Outro", NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::FormShow(TObject *Sender)
   {
   //
   }

//---------------------------------------------------------------------------

