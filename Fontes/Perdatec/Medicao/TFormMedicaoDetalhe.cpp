//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "FileCtrl.hpp"
#include "TFormMedicaoDetalhe.h"
#include "..\Apl\VTApl.h"
#include "..\Medicao\TMedicao.h"
#include "..\Complexo\Complexo.h"
#include "..\Diretorio\VTPath.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Potencia\VTDemanda.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTBarra.h"
#include "..\Classes\VTClasse.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMedicaoDetalhe *FormMedicaoDetalhe;
//---------------------------------------------------------------------------
__fastcall TFormMedicaoDetalhe::TFormMedicaoDetalhe(TComponent* Owner, VTApl *apl,TMedicao *medicao, int ndias)
	: TForm(Owner)
{
	mApl = apl;
	mMedicao = medicao;
	mNdias = ndias;
	PreencheListViews();
}
//---------------------------------------------------------------------------
void __fastcall TFormMedicaoDetalhe::Exporta(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMedicaoDetalhe::PreencheListViews(void)
{
	PreencheListViews(lviewBTVAR,mMedicao->lisCAR_BT_AJUS);
	PreencheListViews(lviewBTFIX,mMedicao->lisCAR_BT_FIXA);
	PreencheListViews(lviewMTVAR,mMedicao->lisCAR_MT_AJUS);
	PreencheListViews(lviewMTFIX,mMedicao->lisCAR_MT_FIXA);
}
//---------------------------------------------------------------------------
void __fastcall TFormMedicaoDetalhe::PreencheListViews(TListView *lview, TList *lisCARGAS)
{
	VTCarga *carga;
	TListItem *item;

	lview->Clear();
	lview->Items->BeginUpdate();
	for (int n = 0; n < lisCARGAS->Count; n++)
	{
		carga = (VTCarga*)lisCARGAS->Items[n];
		item = lview->Items->Add();
		PreencheListItem(item, carga);
	}
	lview->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TFormMedicaoDetalhe::PreencheListItem(TListItem *item, VTCarga *carga)
{
	VTDemanda *demanda = (VTDemanda*)mApl->GetObject(__classid(VTDemanda));
	VTPatamares *patamares = (VTPatamares*)mApl->GetObject(__classid(VTPatamares));
	double edia_mwh = 0;
	double emes_mwh = 0;
	complex<double> smva = CZero();
	int num_pat = patamares->NumPatamar();

	edia_mwh = demanda->Edia_mwh[carga];
	emes_mwh = edia_mwh*mNdias;
	item->Caption = carga->Codigo;
	item->SubItems->Add(carga->classe->Codigo);
	item->SubItems->Add(carga->pbarra->Codigo);
	item->SubItems->Add(DoubleToStr("%4.3f",edia_mwh));
	item->SubItems->Add(DoubleToStr("%4.3f",emes_mwh));

	for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
	{
		smva = demanda->S_mva[carga][ind_pat];
		item->SubItems->Add(DoubleToStr("%4.3f",smva.real()*1000));
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormMedicaoDetalhe::ExportaListView(TListView *lview, AnsiString filepath)
{
	TStringList *strings;
	TListItem *item;
	AnsiString linha;

	strings = new TStringList;
	//poe header
	linha = "Codigo Carga;Classe;Barra;Edia(MWh);Emes(MWh);";
	for (int n = 0; n < 24; n++)
	{
		linha += "P[" + IntToStr(n) + "](kW);";
	}
	strings->Add(linha);
	for (int nl = 0; nl < lview->Items->Count; nl++)
	{
		item =  lview->Items->Item[nl];
		linha = "";
		linha += item->Caption + ";";
		for (int ns = 0; ns < item->SubItems->Count; ns++)
		{
			linha += item->SubItems->Strings[ns] + ";";
		}
		strings->Add(linha);
	}

	strings->SaveToFile(filepath.c_str());
	delete strings;
}
//---------------------------------------------------------------------------
void __fastcall TFormMedicaoDetalhe::ToolButton1Click(TObject *Sender)
{
	VTPath *path = (VTPath*)mApl->GetObject(__classid(VTPath));
	String filepath = "";
	String filename = "";
	const int SELDIRHELP = 1000;


	//configura  local para salvar
	filepath = path->DirExporta();
	if (!SelectDirectory(filepath, TSelectDirOpts() << sdAllowCreate << sdPerformCreate << sdPrompt,SELDIRHELP))
	{
		return;
	}

	ExportaListView(lviewBTVAR,filepath + "//btVar.txt");
	ExportaListView(lviewBTFIX,filepath + "//btFix.txt");
	ExportaListView(lviewMTVAR,filepath + "//mtVar.txt");
	ExportaListView(lviewMTFIX,filepath + "//mtFix.txt");
}
//---------------------------------------------------------------------------

