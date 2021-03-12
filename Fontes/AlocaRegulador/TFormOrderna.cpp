//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "TFormOrderna.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFormOrderna::TFormOrderna(TComponent* Owner, VTApl *apl, TList *lisEqp)
	: TForm(Owner)
{
	this->apl = apl;
	IniciaLisEqpto(lisEqp);

}
//---------------------------------------------------------------------------
void __fastcall TFormOrderna::ListBoxClick(TObject *Sender)
{
	//var local
	VTEqpto *eqpto;
	if(ListBox->ItemIndex < 0) return;
	eqpto = (VTEqpto *) ListBox->Items->Objects[ListBox->ItemIndex];
	//desenha a moldura
	VTGrafico *graf   = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
	graf->Moldura(eqpto);
}
//---------------------------------------------------------------------------
void __fastcall TFormOrderna::IniciaLisEqpto(TList *lisEqp)
{
	//var local
	VTEqpto *eqpto;
	ListBox->Items->Clear();
	for(int n = 0; n < lisEqp->Count; n++)
		{
		eqpto = (VTEqpto *) lisEqp->Items[n];
		ListBox->Items->AddObject(eqpto->Codigo, eqpto);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormOrderna::ListBoxDblClick(TObject *Sender)
{
	//var local
	VTEqpto *eqpto;
	if(ListBox->ItemIndex < 0) return;
	eqpto = (VTEqpto *) ListBox->Items->Objects[ListBox->ItemIndex];
	//desenha a moldura
	VTGrafico *graf   = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
	graf->Moldura(eqpto);

}
//---------------------------------------------------------------------------
