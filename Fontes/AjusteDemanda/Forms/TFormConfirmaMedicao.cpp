//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormConfirmaMedicao.h"
#include "..\ImportaMedicao\VTMedicoes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormConfirmaMedicao::TFormConfirmaMedicao(TComponent* Owner, VTMedicoes *medicoes)
   : TForm(Owner)
	{
	//guarda ponteiro
	this->medicoes = medicoes;
   }

//---------------------------------------------------------------------------
__fastcall TFormConfirmaMedicao::~TFormConfirmaMedicao(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormConfirmaMedicao::ButNaoClick(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormConfirmaMedicao::ButSimClick(TObject *Sender)
   {
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormConfirmaMedicao::CBoxArquivoChange(TObject *Sender)
   {
   //exibe arquivo selecionado
   ExibeArquivoSelecionado();
   }

//---------------------------------------------------------------------------
void __fastcall TFormConfirmaMedicao::DefineArqMedicao(TStrings *files)
   {
   //salva ponteiro para objeto
   this->files = files;
   //inicia CBoxArquivo
   CBoxArquivo->Clear();
   for (int n = 0; n < files->Count; n++)
      {
      CBoxArquivo->Items->Add(ExtractFileName(files->Strings[n]));
      }
   //seleciona primeiro item
   if (CBoxArquivo->Items->Count > 0) CBoxArquivo->ItemIndex = 0;
   //exibe arquivo selecionado
   ExibeArquivoSelecionado();
   }

//---------------------------------------------------------------------------
void __fastcall TFormConfirmaMedicao::ExibeArquivoSelecionado(void)
   {
   //variáveis locais;
   int index;

   //reinicia RichEdit
   RichEdit->Clear();
   //proteção
   if ((index = CBoxArquivo->ItemIndex) < 0)     return;
   if (! FileExists(files->Strings[index])) return;
   //carrega o arquivo no RichEdit
   RichEdit->Lines->LoadFromFile(files->Strings[index]);
	}

//---------------------------------------------------------------------------
//eof


