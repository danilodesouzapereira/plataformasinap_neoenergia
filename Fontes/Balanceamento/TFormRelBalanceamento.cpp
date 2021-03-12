//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormRelBalanceamento.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFormRelBalanceamento::TFormRelBalanceamento(TComponent* Owner, TStringList* relat)
	: TForm(Owner)
{
	if(relat) MontaRelatorio(relat);
}
//---------------------------------------------------------------------------
__fastcall TFormRelBalanceamento::~TFormRelBalanceamento(void)
{
	//nada a fazer

}
//---------------------------------------------------------------------------
void __fastcall TFormRelBalanceamento::MontaRelatorio(TStringList* relat)
{
   RichEdit->Lines->Clear();
	RichEdit->Lines->AddStrings(relat);
}
//---------------------------------------------------------------------------
void __fastcall TFormRelBalanceamento::ActionImprimirExecute(TObject *Sender)
{
	Hide();
	//print
	PrintDialog->Execute();
	Show();

}
//---------------------------------------------------------------------------
void __fastcall TFormRelBalanceamento::ActionSairExecute(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormRelBalanceamento::ActionSalvarExecute(TObject *Sender)
{
	Hide();
	//salvar relatório
	if(SaveDialog->Execute()){
		RichEdit->Lines->SaveToFile(SaveDialog->FileName);
	}
	Show();
}

//---------------------------------------------------------------------------
