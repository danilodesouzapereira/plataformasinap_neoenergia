//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "..\Rede\VTEqpto.h"
#include "TFormEdtEqptosCodigo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"
TFormEdtEqptosCodigo *FormEdtEqptosCodigo;
//---------------------------------------------------------------------------
__fastcall TFormEdtEqptosCodigo::TFormEdtEqptosCodigo(TComponent* Owner, TWinControl *parent, TList *lisEqptos)
	: TFormEdt(Owner,parent)
{
	mListEqptos = lisEqptos;
	PreencheListView();
}
//---------------------------------------------------------------------------
__fastcall TFormEdtEqptosCodigo::~TFormEdtEqptosCodigo(void)
{

}
//---------------------------------------------------------------------------
void __fastcall TFormEdtEqptosCodigo::PreencheListView(void)
{
    VTEqpto *eqpto;
	TListItem *item;

	lviewEqptos->Clear();
	lviewEqptos->Items->BeginUpdate();
	for (int ne = 0; ne < mListEqptos->Count; ne++)
	{
		eqpto = (VTEqpto*)mListEqptos->Items[ne];
		item = lviewEqptos->Items->Add();
		item->Data = eqpto;
		item->Caption = eqpto->Codigo;
		item->SubItems->Add(eqpto->TipoAsString());
	}
	lviewEqptos->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TFormEdtEqptosCodigo::ToolButton1Click(TObject *Sender)
{
  this->ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormEdtEqptosCodigo::ActionOnDoneExecute(TObject *Sender)
{
	//teste
}
//---------------------------------------------------------------------------
