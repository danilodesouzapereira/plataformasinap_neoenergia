//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <Fontes\Radial\VTArvore.h>
#include <Fontes\Radial\VTNode.h>
#include "TFormRjChaves.h"
#include "TInfoRJChave.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormRjChaves *FormRjChaves;
//---------------------------------------------------------------------------
__fastcall TFormRjChaves::TFormRjChaves(TComponent* Owner, VTArvore* arvore)
	: TForm(Owner)
{
	arvore->TreeViewInicia(treeViewArvore,false);
}
//---------------------------------------------------------------------------
void __fastcall TFormRjChaves::treeViewArvoreClick(TObject *Sender)
{
	TTreeNode *node;
	TInfoRJChave *str;
	VTNode *nodeS;

	node = treeViewArvore->Selected;
	if (node != NULL)
	{
		nodeS = (VTNode*) node->Data;
		str = (TInfoRJChave*) nodeS->Object;
		ExibeValor(str);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormRjChaves::ExibeValor(TInfoRJChave *str)
{
	AnsiString nomeValor;

	vlistEditor->Strings->Clear();
	nomeValor = "Pacm_kW=" + DoubleToStr("%3.2f", str->demandaAtivaAcum_kw);
	vlistEditor->Strings->Add(nomeValor);
	nomeValor = "Qacm_kvar=" + DoubleToStr("%3.2f", str->demandaReativaAcum_kvar);
	vlistEditor->Strings->Add(nomeValor);
	nomeValor = "Snom_kVA=" + DoubleToStr("%3.2f", str->potenciaNomAcum_kva);
	vlistEditor->Strings->Add(nomeValor);
	nomeValor = "Consumidor BT resid=" + IntToStr(str->qntdeConsuBT.resid);
	vlistEditor->Strings->Add(nomeValor);
	nomeValor = "Consumidor BT comerc=" + IntToStr(str->qntdeConsuBT.comerc);
	vlistEditor->Strings->Add(nomeValor);
	nomeValor = "Consumidor BT indust=" + IntToStr(str->qntdeConsuBT.indust);
	vlistEditor->Strings->Add(nomeValor);
	nomeValor = "Consumo resid_kWh=" + DoubleToStr("%3.2f", str->consumo.resid_kwh);
	vlistEditor->Strings->Add(nomeValor);
	nomeValor = "Consumo comerc_kWh=" + DoubleToStr("%3.2f", str->consumo.comerc_kwh);
	vlistEditor->Strings->Add(nomeValor);
	nomeValor = "Consumo indust_kWh=" + DoubleToStr("%3.2f", str->consumo.indust_kwh);
	vlistEditor->Strings->Add(nomeValor);
	nomeValor = "Consumidor MT resid=" + IntToStr(str->qntdeConsuMT.resid);
	vlistEditor->Strings->Add(nomeValor);
	nomeValor = "Consumidor MT comerc=" + IntToStr(str->qntdeConsuMT.comerc);
	vlistEditor->Strings->Add(nomeValor);
	nomeValor = "Consumidor MT indust=" + IntToStr(str->qntdeConsuMT.indust);
	vlistEditor->Strings->Add(nomeValor);

}
//---------------------------------------------------------------------------

