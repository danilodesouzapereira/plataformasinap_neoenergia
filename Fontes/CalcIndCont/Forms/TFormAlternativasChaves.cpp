// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TFormAlternativasChaves.h"
#include "..\VTCalcIndCont.h"
#include "..\VTBuscaProt.h"
#include "..\VTVisRede.h"
#include "..\VTResCont.h"
#include "..\VTMostraGrafIndic.h"

#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Bloco\VTBlocos.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTTipos.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>

#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormAlternativasChaves::TFormAlternativasChaves(TComponent* Owner, VTApl* apl_owner,
	TWinControl *parent) : TForm(Owner)
{
	this->apl = NewObjApl(NULL, apl_owner);
	calcIndCont = (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));
	if (calcIndCont == NULL)
	{
		calcIndCont = NewObjCalcIndCont(apl, NULL, true);
		apl->Add(calcIndCont);
		apl_owner->Add(calcIndCont->Blocos);
		calcIndCont->LeArquivosMdb("");//hk201808: reformula: calcIndCont->LeArquivos("");
	}
	lisAlternativas = new TList();
	TvChavesInicia();
	CbRedesInicia();
	TvAlternativasInicia();
}

// ---------------------------------------------------------------------------
__fastcall TFormAlternativasChaves::~TFormAlternativasChaves(void)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	grafico->DestacaEqpto(NULL, clBlue, 10);
	grafico->Moldura();
	grafico->Show(grafTOPO);

	if (lisAlternativas)
	{
		for (int n = 0; n < lisAlternativas->Count; n++)
		{
			delete((strAlt*)lisAlternativas->Items[n])->lisChavesAuto;
			delete((strAlt*)lisAlternativas->Items[n])->resCont;
		}
		LimpaTList(lisAlternativas);
		delete lisAlternativas;
		lisAlternativas = NULL;
	}
	// Reinicia chaves com status inicial
	calcIndCont->ReiniciaChavesAuto();
	// Deleta apl do form
	if (apl)
		apl->Owner->Remove(calcIndCont->Blocos); // graf
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::ActionNovaAlternativaExecute(TObject *Sender)

{
	TTreeNode *node;
	node = NovaAlternativa();
	CalculaAlternativa((strAlt*)node->Data);
}

// ---------------------------------------------------------------------------
TTreeNode* __fastcall TFormAlternativasChaves::NovaAlternativa(void)
{
	TTreeNode *node;
	strAlt *alt;
	lisAlternativas->Add(alt = new strAlt);
	alt->resCont = NULL;
	alt->lisChavesAuto = new TList();
	alt->Codigo = IntToStr(lisAlternativas->Count);
	node = tvAlternativas->Items->AddChildObject(NULL, IntToStr(lisAlternativas->Count), alt);
	return (node);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::tvAlternativasDragDrop(TObject *Sender, TObject *Source,
	int X, int Y)
{
	// variáveis locais
	TTreeView *tvOri;
	TTreeNode *node;
	TTreeNode *nodeOri;
	AnsiString aux;
	TMetaClass *obj_class;
	AnsiString obj_name;
	strAlt *alt, *altIni;
	bool flagAlt = false;
	bool flagMult = true;
	int iconta;

	obj_name = Source->ClassName();
	if (obj_name.AnsiCompareIC("TTreeView") != 0)
		return;
	tvOri = ((TTreeView*)Source);
	if (tvOri->Name.Compare("tvChaves") != 0)
		return;

	if (flagMult)
	{
		// determina TreeNode nas coordenadas X,Y
		altIni = NULL;
		if ((node = tvAlternativas->GetNodeAt(X, Y)) != NULL)
		{
			// busca no´ da alternativa
			while (node->Level > 0)
				node = node->Parent;
			altIni = (strAlt*)node->Data;
		}

		iconta = 0;
		for (int n = 0; n < tvOri->Items->Count; n++)
		{
			if (!(nodeOri = tvOri->Items->Item[n])->Selected)
				continue;
			if (nodeOri->Data == NULL)
				continue;
			if (altIni != NULL)
			{
				if (altIni->lisChavesAuto->IndexOf(nodeOri->Data) != -1)
					continue;
			}
			node = NovaAlternativa();
			alt = (strAlt*)node->Data;
			if (altIni != NULL)
				alt->lisChavesAuto->Assign(altIni->lisChavesAuto, laOr);
			alt->lisChavesAuto->Add(nodeOri->Data);
			iconta++;
			node->Text = IntToStr(iconta);
			if (altIni != NULL)
			{
				node->Text += ":";
				node->Text += altIni->Codigo;
			}
			node->Text += ":";
			node->Text += ((VTChave*)nodeOri->Data)->Codigo;
			alt->Codigo = node->Text;
			CalculaAlternativa(alt);
		}

	}
	else
	{
		// determina TreeNode nas coordenadas X,Y
		if ((node = tvAlternativas->GetNodeAt(X, Y)) == NULL)
		{
			node = NovaAlternativa();
			flagAlt = true;
		}
		// if((nodeOri = tvOri->Selected) == NULL) return;
		// busca no´ da alternativa
		while (node->Level > 0)
			node = node->Parent;
		alt = (strAlt*)node->Data;

		for (int n = 0; n < tvOri->Items->Count; n++)
		{
			if (!(nodeOri = tvOri->Items->Item[n])->Selected)
				continue;
			if (nodeOri->Data == NULL)
				continue;
			if (alt->lisChavesAuto->IndexOf(nodeOri->Data) != -1)
				continue;
			tvAlternativas->Items->AddChildObject(node, nodeOri->Text, nodeOri->Data);
			alt->lisChavesAuto->Add(nodeOri->Data);
			flagAlt = true;
		}
		if (flagAlt)
			CalculaAlternativa(alt);
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::CalculaAlternativa(strAlt *alt)
{
	VTChave *pchave;
	// buscaProt->LisChvAuto->Clear();
	calcIndCont->ReiniciaChavesAuto();

	for (int n = 0; n < alt->lisChavesAuto->Count; n++)
	{
		pchave = (VTChave*)alt->lisChavesAuto->Items[n];
		// buscaProt->LisChvAuto->Add(pchave);
		// gh201809
		calcIndCont->InsereChaveAuto(pchave, false, false);
	}

	// Executa: definição de estratégias e cálculo de indicadores
	calcIndCont->Executa();

	if (alt->resCont != NULL)
		delete alt->resCont;
	alt->resCont = calcIndCont->ResCont->ClonaResCont();

	/*
	 alt->dec      = calcIndCont->ResGeral->tdeslN /60.;
	 alt->fec      = calcIndCont->ResGeral->ndeslN;
	 alt->end      = calcIndCont->ResGeral->end;
	 if(calcIndCont->ResGeral->ncons > 0)
	 {
	 alt->dec /= calcIndCont->ResGeral->ncons;
	 alt->fec /= calcIndCont->ResGeral->ncons;
	 } else {alt->dec = alt->fec = 0.0;}
	 */
	LvResAltReinicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::tvAlternativasDragOver(TObject *Sender, TObject *Source,
	int X, int Y, TDragState State, bool &Accept)
{
	Accept = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::CbRedesInicia(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	TList *lisRede = redes->LisRede();
	cbRedes->Clear();
	cbRedes->AddItem("Geral", NULL);
	for (int n = 0; n < lisRede->Count; n++)
	{
		rede = (VTRede*)lisRede->Items[n];
		cbRedes->AddItem(rede->Codigo, rede);
	}
	cbRedes->ItemIndex = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::TvChavesInicia(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	VTVisRede *visRede;
	TList *lisChaves;
	TList *lisRede = redes->LisRede();
	VTChave *pchave;
	TTreeNode *nodeRede, *nodeChave;
	AnsiString aux;
	VTTipoChave *tipoChave;
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList *lisTipoChaves;
	int nconsProt;
	double cargaProt, distProt;

	// Reinicia as listboxes
	tvChaves->Items->Clear();

	lisTipoChaves = tipos->LisTipoChave();
	lisChaves = new TList();
   calcIndCont->BuscaProt->LisChaveOrd(lisChaves);//hk201808: reformula sai: calcIndCont->LisChavesOrd(lisChaves);
	// for(int nv = 0 ; nv < calcIndCont->LisVisRede->Count; nv++)
	// {
	// lisChaves->Add(((VTVisRede*)calcIndCont->LisVisRede->Items[nv])->pchave);
	// }
	for (int nr = 0; nr < lisRede->Count; nr++)
	{
		rede = (VTRede*)lisRede->Items[nr];
		if (!rede->Carregada)
			continue;
		nodeRede = tvChaves->Items->AddChildObject(NULL, rede->Codigo, NULL);
		nodeRede->ImageIndex = 24;
		nodeRede->SelectedIndex = nodeRede->ImageIndex;
		// Chaves NA
		for (int nv = 0; nv < calcIndCont->LisVisRede->Count; nv++)
		{
			visRede = (VTVisRede*)calcIndCont->LisVisRede->Items[nv];
			if ((visRede->rede1 != rede) && (visRede->rede2 != rede))
				continue;
			pchave = visRede->pchave;
			aux.sprintf("(%s)", pchave->TipoChave->Codigo.c_str());
			aux.cat_sprintf("(%s)", rede->Codigo.c_str());
			// aux.cat_sprintf("(%s)", (pchave->Estado == chvABERTA) ? "NA: " : "NF: ");
			aux.cat_sprintf("(%s)", (pchave->Aberta) ? "NA: " : "NF: ");
			nodeChave = tvChaves->Items->AddChildObject(nodeRede, aux + pchave->Codigo, pchave);
			nodeChave->ImageIndex = 7;
			nodeChave->SelectedIndex = nodeChave->ImageIndex;
		}
		for (int n = 0; n < lisTipoChaves->Count; n++)
		{
			tipoChave = (VTTipoChave*)lisTipoChaves->Items[n];
			for (int nc = 0; nc < lisChaves->Count; nc++)
			{
				pchave = (VTChave*)lisChaves->Items[nc];
				// if(pchave->Estado == chvABERTA) continue;
				if (pchave->Aberta)
					continue;
				if (pchave->TipoChave != tipoChave)
					continue;
				if (pchave->rede != rede)
					continue;
				lisChaves->Delete(nc);
				aux.sprintf("(%s)", tipoChave->Codigo.c_str());
				aux.cat_sprintf("(%s)", rede->Codigo.c_str());
				// aux.cat_sprintf("(%s)", (pchave->Estado == chvABERTA) ? "NA: " : "NF: ");
				aux.cat_sprintf("(%s)", (pchave->Aberta) ? "NA: " : "NF: ");
				nodeChave = tvChaves->Items->AddChildObject(nodeRede, aux + pchave->Codigo, pchave);
				nodeChave->ImageIndex = 7;
				nodeChave->SelectedIndex = nodeChave->ImageIndex;
			}
		}
	}
	delete lisChaves;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::TvAlternativasInicia(void)
{
	strAlt *alt;
	TTreeNode *node;
	VTChave *chave;
	AnsiString aux;
	node = NovaAlternativa();
	alt = (strAlt*)node->Data;
	alt->lisChavesAuto->Assign(calcIndCont->LisChavesAuto, laOr);
	for (int nc = 0; nc < alt->lisChavesAuto->Count; nc++)
	{
		chave = (VTChave*)alt->lisChavesAuto->Items[nc];
		aux.sprintf("(%s)", chave->TipoChave->Codigo.c_str());
		aux.cat_sprintf("(%s)", chave->rede->Codigo.c_str());
		// aux.cat_sprintf("(%s)", (chave->Estado == chvABERTA) ? "NA: " : "NF: ");
		aux.cat_sprintf("(%s)", (chave->Aberta) ? "NA: " : "NF: ");
		tvAlternativas->Items->AddChildObject(node, aux + chave->Codigo, chave);
	}

	CalculaAlternativa(alt);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::tvAlternativasKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift)
{
	strAlt *alt;
	TTreeNode *node, *nodeAlt;

	if (Key == VK_DELETE)
	{
		if ((node = tvAlternativas->Selected) == NULL)
			return;
		if (node->Level == 0)
		{
			nodeAlt = node;
			alt = (strAlt*)nodeAlt->Data;
			lisAlternativas->Remove(alt);
			delete alt->lisChavesAuto;
			delete alt;
			nodeAlt->Delete();
			LvResAltReinicia();
		}
		else
		{
			nodeAlt = node;
			while (nodeAlt->Level > 0)
				nodeAlt = nodeAlt->Parent;
			alt = (strAlt*)nodeAlt->Data;
			alt->lisChavesAuto->Remove(node->Data);
			node->Delete();
			CalculaAlternativa(alt);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::tvChavesMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	TTreeView* tv = tvChaves;
	tv->BeginDrag(true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::tvAlternativasEdited(TObject *Sender, TTreeNode *Node,
	UnicodeString &S)
{
	TTreeNode *node;
	strAlt *alt;

	if ((node = tvAlternativas->Selected) == NULL)
		return;
	if (node->Level != 0)
		return;
	alt = (strAlt*)node->Data;

	alt->Codigo = S;

	LvResAltReinicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::LvResAltReinicia(void)
{
	strAlt *alt;
	VTRede *rede;
	TListItem *item;
	AnsiString aux;
	double dec, fec, end;
	VTResCont *resCont;

	if (cbRedes->ItemIndex == 0)
		rede = NULL;
	else
	{
		rede = (VTRede*)cbRedes->Items->Objects[cbRedes->ItemIndex];
	}

	lvResAlt->Clear();
	for (int n = 0; n < lisAlternativas->Count; n++)
	{
		alt = (strAlt*)lisAlternativas->Items[n];
		if (rede == NULL)
		{
			dec = alt->resCont->ResGeral->tdeslN / 60.;
			fec = alt->resCont->ResGeral->ndeslN;
			end = alt->resCont->ResGeral->end;
			if (alt->resCont->ResGeral->ncons > 0)
			{
				dec /= alt->resCont->ResGeral->ncons;
				fec /= alt->resCont->ResGeral->ncons;
			}
			else
			{
				dec = fec = 0.0;
			}
		}
		else
		{
			dec = alt->resCont->ResRede[rede]->tdeslN / 60.;
			fec = alt->resCont->ResRede[rede]->ndeslN;
			end = alt->resCont->ResRede[rede]->end;
			if (alt->resCont->ResGeral->ncons > 0)
			{
				dec /= alt->resCont->ResRede[rede]->ncons;
				fec /= alt->resCont->ResRede[rede]->ncons;
			}
			else
			{
				dec = fec = 0.0;
			}
		}

		item = lvResAlt->Items->Add();
		item->Caption = alt->Codigo;
		item->Data = alt;
		aux = DoubleToStr("%6.1f", dec);
		item->SubItems->Add(aux);
		aux = DoubleToStr("%6.1f", fec);
		item->SubItems->Add(aux);
		aux = DoubleToStr("%6.1f", end);
		item->SubItems->Add(aux);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::tvAlternativasClick(TObject *Sender)
{
	TTreeNode *node;
	TList *lisAux;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	strAlt *alt;

	lisAux = new TList();
	for (int n = 0; n < tvAlternativas->Items->Count; n++)
	{
		node = tvAlternativas->Items->Item[n];
		if (!node->Selected)
			continue;
		if (!node->Data)
			continue;
		if (node->Level == 0)
		{
			alt = (strAlt*)node->Data;
			lisAux->Assign(alt->lisChavesAuto, laOr);
			continue;
		}
		if (node->Level == 1)
		{
			lisAux->Add(node->Data);
			continue;
		}
	}

	grafico->Moldura(lisAux);
	delete lisAux;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::ActionViewDECExecute(TObject *Sender)
{
	strAlt *altRef, *alt;
	TTreeNode *node;

	DefineViewMenu(PopupMenuView, 0, 14);

	altRef = NULL;
	alt = NULL;

	node = NULL;
	for (int n = 0; n < tvAlternativas->Items->Count; n++)
	{
		node = tvAlternativas->Items->Item[n];
		if ((node->Level == 0) && (node->Selected))
			break;
		node = NULL;
	}
	if (node == NULL)
		return;
	altRef = (strAlt*)node->Data;

	TListItem *item = NULL;
	for (int n = 0; n < lvResAlt->Items->Count; n++)
	{
		if ((item = lvResAlt->Items->Item[n])->Selected)
			break;
		item = NULL;
	}
	if (item != NULL)
		alt = (strAlt*)item->Data;

	if ((altRef == NULL) && (alt == NULL))
		return;

}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::ActionViewFECExecute(TObject *Sender)
{
	DefineViewMenu(PopupMenuView, 1, 14);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::ActionViewENDExecute(TObject *Sender)
{
	DefineViewMenu(PopupMenuView, 2, 14);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::ActionViewTOPOExecute(TObject *Sender)
{
	VTGrafico *graf = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
	DefineViewMenu(PopupMenuView, 3, 14);
	if (graf)
		graf->Show(grafTOPO);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::DefineViewMenu(TPopupMenu *menu, int index, int imageIndex)
{
	// var local
	TMenuItem *item;

	for (int n = 0; n < menu->Items->Count; n++)
	{
		item = menu->Items->Items[n];
		item->ImageIndex = (index == n) ? imageIndex : -1;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::ActionMostraChavesExecute(TObject *Sender)

{
	TTreeNode *node;
	TList *lisAux;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	lisAux = new TList();
	for (int n = 0; n < tvChaves->Items->Count; n++)
	{
		node = tvChaves->Items->Item[n];
		if (!node->Selected)
			continue;
		if (!node->Data)
			continue;
		lisAux->Add(node->Data);
	}

	grafico->Moldura(lisAux);
	delete lisAux;
}
// ---------------------------------------------------------------------------

void __fastcall TFormAlternativasChaves::tvChavesChange(TObject *Sender, TTreeNode *Node)

{
	ActionMostraChavesExecute(NULL);
}
// ---------------------------------------------------------------------------

void __fastcall TFormAlternativasChaves::cbRedesSelect(TObject *Sender)
{
	LvResAltReinicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::lvResAltClick(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TListItem *item;
	strAlt *alt;

	int index;
	if ((index = lvResAlt->ItemIndex) < 0)
		return;

	item = lvResAlt->Items->Item[index];
	alt = (strAlt*)item->Data;
	grafico->Moldura(alt->lisChavesAuto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlternativasChaves::ActionMostraGrafExecute(TObject *Sender)

{
	VTGrafico *graf = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
	VTMostraGrafIndic *mostraGraf;

	mostraGraf = NewObjMostraGrafIndic(apl);

	switch (igraf)
	{
	case 0:
		// MostraDec;
		break;
	case 1:
		// MostraFec;
		break;
	case 2:
		// MostraEnd;
		break;
	case 3:
		graf->Show(grafTOPO);
		break;
	default: ;
	}
	delete mostraGraf;
}
// ---------------------------------------------------------------------------
