// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormMostraDefeito.h"

#include "..\VTVisRede.h"
#include "..\VTBlocoRad.h"
#include "..\VTCalcIndCont.h"
#include "..\VTDefeito.h"
#include "..\VTEstratSoc.h"
#include "..\VTVisRedeSoc.h"

#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamar.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamares.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMostraDefeito *FormMostraDefeito;

// ---------------------------------------------------------------------------
__fastcall TFormMostraDefeito::TFormMostraDefeito(TComponent* Owner, TWinControl *parent,
	VTApl *apl, VTDefeito *pdef, TList *lisChavesAuto) : TForm(Owner)
{
	// insere Form em seu Parent
	if (parent)
		Parent = parent;

	this->apl = apl;
	this->pdef = pdef;
	this->lisChavesAuto = lisChavesAuto;
	// n14 calcIndCont = (VTCalcIndCont*)apl->GetObject(__classid(VTCalcIndCont));

	IniciaTreeView();
}

// ---------------------------------------------------------------------------
__fastcall TFormMostraDefeito::~TFormMostraDefeito(void)
{
}

// ---------------------------------------------------------------------------
void __fastcall TFormMostraDefeito::IniciaTreeView(void)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	// VTRedes        *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TTreeNode *nodeRaiz, *node, *node1; // , *node2;
	VTEstratSoc *estratSoc;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	int numPat = patamares->NumPatamar();
	VTVisRedeSoc *visRedeSoc;
	AnsiString aux;
	VTVisRede *visRede;
	VTBlocoRad *blRad;
	VTChave *pchave;
	double folga, carga;

	grafico->DestacaEqpto(pdef->Bloco->LisLigacao(), clRed, 10);

	tvDefeito->Items->Clear();
	nodeRaiz = tvDefeito->Items->AddChildObject(NULL, "DEFEITO", pdef);

	// node = tvDefeito->Items->AddChildObject(nodeRaiz, "LISBLOCOSVIS", pdef);
	// node = tvDefeito->Items->AddChildObject(nodeRaiz, "LISCHVBLOCOMAN", pdef);
	// node = tvDefeito->Items->AddChildObject(nodeRaiz, "LISCHVBLOCOVIS", pdef);
	// node = tvDefeito->Items->AddChildObject(nodeRaiz, "LISCHVALIVIOVIS", pdef);
	// node = tvDefeito->Items->AddChildObject(nodeRaiz, "LISCHVJUS", pdef);
	// node = tvDefeito->Items->AddChildObject(nodeRaiz, "LISVISREDEDEF", pdef);
	// for(int nv = 0; nv < pdef->LisVisRedeDef->Count; nv++)
	// {
	// visRede = (VTVisRede*)pdef->LisVisRedeDef->Items[nv];
	// node1 = tvDefeito->Items->AddChildObject(node, visRede->pchave->Codigo, visRede);
	// tvDefeito->Items->AddChildObject(node1, "LISBLOCOSVIS", visRede);
	// tvDefeito->Items->AddChildObject(node1, "LISCHVBLOCOVIS", visRede);
	// tvDefeito->Items->AddChildObject(node1, "LISCHVALIVIOVIS", visRede);
	// }

	if (pdef->ChvReliga != NULL)
	{
		node = tvDefeito->Items->AddChildObject(nodeRaiz, "RELIGA", pdef);
		aux.sprintf("ATUA");
		aux.cat_sprintf("[%s]", pdef->ChvReliga->Codigo.c_str());
		tvDefeito->Items->AddChildObject(node, aux, pdef->ChvReliga);
	}
	node = tvDefeito->Items->AddChildObject(nodeRaiz, "PROT", pdef);

	if (pdef->ChvProt == NULL)
		return;

	aux.sprintf("ABRE");
	aux.cat_sprintf("[%s]", pdef->ChvProt->Codigo.c_str());
	tvDefeito->Items->AddChildObject(node, aux, pdef->ChvProt);

	if (pdef->ChvIsolaMont != NULL)
	{

		if (pdef->ChvIsolaAutoMont != NULL)
		{
			node = tvDefeito->Items->AddChildObject(nodeRaiz, "MONTANTE_RAPIDO", pdef);

			aux.sprintf("ABRE");
			if (ChaveAutomatica(pdef->ChvIsolaAutoMont))
				aux.cat_sprintf("[AUTO]");
			else
				aux.cat_sprintf("[MANUAL]");
			aux.cat_sprintf("[%s]", pdef->ChvIsolaAutoMont->Codigo.c_str());
			tvDefeito->Items->AddChildObject(node, aux, pdef->ChvIsolaAutoMont);

			aux.sprintf("FECHA");
			if (ChaveAutomatica(pdef->ChvProt))
				aux.cat_sprintf("[AUTO]");
			else
				aux.cat_sprintf("[MANUAL]");
			aux.cat_sprintf("[%s]", pdef->ChvProt->Codigo.c_str());
			tvDefeito->Items->AddChildObject(node, aux, pdef->ChvProt);

			if (pdef->LisBlocosAutoDef->Count > 0)
			{
				node = tvDefeito->Items->AddChildObject(nodeRaiz, "MONTANTE_LENTO", pdef);

				aux.sprintf("ABRE");
				if (ChaveAutomatica(pdef->ChvIsolaMont))
					aux.cat_sprintf("[AUTO]");
				else
					aux.cat_sprintf("[MANUAL]");
				aux.cat_sprintf("[%s]", pdef->ChvIsolaMont->Codigo.c_str());
				tvDefeito->Items->AddChildObject(node, aux, pdef->ChvIsolaMont);

				aux.sprintf("FECHA");
				if (ChaveAutomatica(pdef->ChvIsolaAutoMont))
					aux.cat_sprintf("[AUTO]");
				else
					aux.cat_sprintf("[MANUAL]");
				aux.cat_sprintf("[%s]", pdef->ChvIsolaAutoMont->Codigo.c_str());
				tvDefeito->Items->AddChildObject(node, aux, pdef->ChvIsolaAutoMont);
			}
		}
		else
		{
			if (pdef->LisBlocosProtDef->Count > 0)
			{
				node = tvDefeito->Items->AddChildObject(nodeRaiz, "MONTANTE_LENTO", pdef);

				aux.sprintf("ABRE");
				if (ChaveAutomatica(pdef->ChvIsolaMont))
					aux.cat_sprintf("[AUTO]");
				else
					aux.cat_sprintf("[MANUAL]");
				aux.cat_sprintf("[%s]", pdef->ChvIsolaMont->Codigo.c_str());
				tvDefeito->Items->AddChildObject(node, aux, pdef->ChvIsolaMont);

				aux.sprintf("FECHA");
				if (ChaveAutomatica(pdef->ChvProt))
					aux.cat_sprintf("[AUTO]");
				else
					aux.cat_sprintf("[MANUAL]");
				aux.cat_sprintf("[%s]", pdef->ChvProt->Codigo.c_str());
				tvDefeito->Items->AddChildObject(node, aux, pdef->ChvProt);
			}
		}
	}
	node = tvDefeito->Items->AddChildObject(nodeRaiz, "JUSANTE", pdef);

	for (int np = 0; np < numPat; np++)
	{
		estratSoc = pdef->EstratSoc[np];
		if (estratSoc->lisVisRedeSoc->Count == 0)
			continue;
		aux.sprintf("Patamar: [%i]", np);
		node1 = tvDefeito->Items->AddChildObject(node, aux, estratSoc);
		for (int nvis = 0; nvis < estratSoc->lisVisRedeSoc->Count; nvis++)
		{
			// operações de isolament/socorro, desligamento/volta
			visRedeSoc = (VTVisRedeSoc*)estratSoc->lisVisRedeSoc->Items[nvis];
			if (visRedeSoc->blRadRapida)
			{ // abre rápida e fecha VIS
				pchave = visRedeSoc->blRadRapida->Chave;
				aux.sprintf("ABRE");
				aux.cat_sprintf("[AUTO]");
				aux.cat_sprintf("[%s]", pchave->Codigo.c_str());
				tvDefeito->Items->AddChildObject(node1, aux, visRedeSoc->blRadRapida->Chave);
				pchave = visRedeSoc->visRede->pchave;
				aux.sprintf("FECHA");
				if (ChaveAutomatica(pchave))
					aux.cat_sprintf("[AUTO]");
				else
					aux.cat_sprintf("[MANUAL]");
				aux.cat_sprintf("[%s]", pchave->Codigo.c_str());
				tvDefeito->Items->AddChildObject(node1, aux, pchave);
			}

			if (visRedeSoc->blRadLenta)
			{ // abre lenta
				pchave = visRedeSoc->blRadLenta->Chave;
				aux.sprintf("ABRE");
				aux.cat_sprintf("[MANUAL]");
				aux.cat_sprintf("[%s]", pchave->Codigo.c_str());
				tvDefeito->Items->AddChildObject(node1, aux, pchave);
				if (visRedeSoc->blRadRapida)
				{
					// e fecha rápida
					pchave = visRedeSoc->blRadRapida->Chave;
					aux.sprintf("FECHA");
					aux.cat_sprintf("[AUTO]");
					aux.cat_sprintf("[%s]", pchave->Codigo.c_str());
					tvDefeito->Items->AddChildObject(node1, aux, visRedeSoc->blRadRapida->Chave);
				}
				else
				{
					// e fecha VIS
					pchave = visRedeSoc->visRede->pchave;
					aux.sprintf("FECHA");
					if (ChaveAutomatica(pchave))
						aux.cat_sprintf("[AUTO]");
					else
						aux.cat_sprintf("[MANUAL]");
					aux.cat_sprintf("[%s]", pchave->Codigo.c_str());
					tvDefeito->Items->AddChildObject(node1, aux, pchave);
				}
			}
		}
	}
	// Adiciona elementos a tvDefeito1
	tvDefeito1->Items->Clear();
	node = tvDefeito1->Items->AddChildObject(NULL, "M", NULL);
	//
	aux.sprintf("PROT[%s]", pdef->ChvProt->Codigo.c_str());
	node1 = tvDefeito1->Items->AddChildObject(node, aux, pdef->ChvProt);
	tvDefeito1->Items->AddChildObject(node1, "LisBlocosProtDef", pdef->LisBlocosProtDef);
	//

	aux.sprintf("CHVISOLAUTOMONT[");
	if (pdef->ChvIsolaAutoMont != NULL)
		aux.cat_sprintf("%s", pdef->ChvIsolaAutoMont->Codigo.c_str());
	aux.cat_sprintf("]");
	node1 = tvDefeito1->Items->AddChildObject(node, aux, pdef->ChvIsolaAutoMont);
	tvDefeito1->Items->AddChildObject(node1, "LisBlocosProtAuto", pdef->LisBlocosProtAuto);
	tvDefeito1->Items->AddChildObject(node1, "LisBlocosAutoDef", pdef->LisBlocosAutoDef);

	aux.sprintf("CHVISOLMONT[");
	if (pdef->ChvIsolaMont != NULL)
		aux.cat_sprintf("%s", pdef->ChvIsolaMont->Codigo.c_str());
	aux.cat_sprintf("]");
	node1 = tvDefeito1->Items->AddChildObject(node, aux, pdef->ChvIsolaMont);

	node = tvDefeito1->Items->AddChildObject(NULL, "J", NULL);
	for (int np = 0; np < numPat; np++)
	{
		estratSoc = pdef->EstratSoc[np];
		aux.sprintf("Patamar - lisBlocosReparo [%i]", np);
		node1 = tvDefeito1->Items->AddChildObject(node, aux, estratSoc->lisBlocosReparo);
		for (int nvis = 0; nvis < estratSoc->lisVisRedeSoc->Count; nvis++)
		{
			visRedeSoc = (VTVisRedeSoc*)estratSoc->lisVisRedeSoc->Items[nvis];
			aux.sprintf("visRede %i [%s]", nvis, visRedeSoc->visRede->pchave->Codigo.c_str());
			tvDefeito1->Items->AddChildObject(node1, aux, visRedeSoc->visRede->pchave);
			tvDefeito1->Items->AddChildObject(node1, "lisBlocosSoc", visRedeSoc->lisBlocosSoc);
			for (int nchv = 0; nchv < visRedeSoc->lisChvIsola->Count; nchv++)
			{
				pchave = (VTChave*)visRedeSoc->lisChvIsola->Items[nchv];
				aux.sprintf("chvIsola %i", nchv);
				tvDefeito1->Items->AddChildObject(node1, aux, pchave);
			}
		}
	}

	// Adiciona elementos a tvVisDefeito
	tvVisDefeito->Items->Clear();

	for (int nc = 0; nc < pdef->LisVisRedeDef->Count; nc++)
	{
		visRede = (VTVisRede*)pdef->LisVisRedeDef->Items[nc];
		pchave = visRede->pchave;
		// pchave = (VTChave*)pdef->LisChvVis->Items[nc];
		// visRede = calcIndCont->IdentificaVisRede(pchave);
		aux.sprintf("%s", visRede->pchave->Codigo.c_str());
		if (ChaveAutomatica(visRede->pchave))
			aux.cat_sprintf("[AUTO]");
		else
			aux.cat_sprintf("[MANUAL]");

		node = tvVisDefeito->Items->AddChildObject(NULL, aux, visRede);

		for (int nb = visRede->lisBlocoRadIsolaJus->Count - 1; nb >= 0; nb--)
		{
			blRad = (VTBlocoRad*)visRede->lisBlocoRadIsolaJus->Items[nb];
			aux.sprintf("%s", blRad->Chave->Codigo.c_str());
			if (ChaveAutomatica(blRad->Chave))
				aux.cat_sprintf("[AUTO]");
			else
				aux.cat_sprintf("[MANUAL]");
			node1 = tvVisDefeito->Items->AddChildObject(node, aux, blRad);
			for (int np = 0; np < numPat; np++)
			{
				carga = blRad->CargaBlRad[np];

				folga = visRede->FolgaIA(np) * (visRede->pchave->pbarra1->vnom * 1.73205) / 1000.0;
				// n14 folga = calcIndCont->FolgaIA(visRede, np) * (visRede->pchave->pbarra1->vnom * 1.73205) / 1000.0;
				aux.sprintf("pat[%i]: folga: %6.2f / carga: %6.2f", np, folga, carga);
				// node2 =
				tvVisDefeito->Items->AddChildObject(node1, aux, blRad);
				// aux.sprintf("ABRE");
				// if(buscaProt->ChaveAutomatica(blRad->Chave))
				// aux.cat_sprintf("[AUTO]");
				// else
				// aux.cat_sprintf("[MANUAL]");
				// aux.cat_sprintf("[%s]", blRad->Chave->Codigo);
				// tvVisDefeito->Items->AddChildObject(node2, aux, blRad);
			}
		}
	}

	nodeRaiz->Expand(true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormMostraDefeito::ChaveAutomatica(VTChave *pchave)
{
	// n14if(calcIndCont->LisChavesAuto->IndexOf(pchave) == -1)
	if (lisChavesAuto->IndexOf(pchave) == -1)
		return (false);
	else
		return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMostraDefeito::IniciaTreeView00(void)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	// VTRedes        *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TTreeNode *nodeRaiz, *node, *node1; // , *node2;
	VTEstratSoc *estratSoc;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	int numPat = patamares->NumPatamar();
	VTVisRedeSoc *visRedeSoc;
	AnsiString aux;
	VTVisRede *visRede;
	VTBlocoRad *blRad;
	VTChave *pchave;
	double folga, carga;

	grafico->DestacaEqpto(pdef->Bloco->LisLigacao(), clRed, 10);

	tvDefeito->Items->Clear();
	nodeRaiz = tvDefeito->Items->AddChildObject(NULL, "DEFEITO", pdef);

	node = tvDefeito->Items->AddChildObject(nodeRaiz, "PROT", pdef);
	aux.sprintf("ABRE");
	// if(buscaProt->ChaveAutomatica(pdef->ChvProt))
	// aux.cat_sprintf("[AUTO]");
	// else
	// aux.cat_sprintf("[MANUAL]");
	aux.cat_sprintf("[%s]", pdef->ChvProt->Codigo.c_str());
	tvDefeito->Items->AddChildObject(node, aux, pdef->ChvProt);

	if (pdef->ChvIsolaAutoMont != NULL)
	{
		node = tvDefeito->Items->AddChildObject(nodeRaiz, "MONTANTE_RAPIDO", pdef);

		aux.sprintf("ABRE");
		if (ChaveAutomatica(pdef->ChvIsolaAutoMont))
			aux.cat_sprintf("[AUTO]");
		else
			aux.cat_sprintf("[MANUAL]");
		aux.cat_sprintf("[%s]", pdef->ChvIsolaAutoMont->Codigo.c_str());
		tvDefeito->Items->AddChildObject(node, aux, pdef->ChvIsolaAutoMont);

		aux.sprintf("FECHA");
		if (ChaveAutomatica(pdef->ChvProt))
			aux.cat_sprintf("[AUTO]");
		else
			aux.cat_sprintf("[MANUAL]");
		aux.cat_sprintf("[%s]", pdef->ChvProt->Codigo.c_str());
		tvDefeito->Items->AddChildObject(node, aux, pdef->ChvProt);

		if (pdef->LisBlocosAutoDef->Count > 0)
		{
			node = tvDefeito->Items->AddChildObject(nodeRaiz, "MONTANTE_LENTO", pdef);

			aux.sprintf("ABRE");
			if (ChaveAutomatica(pdef->ChvIsolaMont))
				aux.cat_sprintf("[AUTO]");
			else
				aux.cat_sprintf("[MANUAL]");
			aux.cat_sprintf("[%s]", pdef->ChvIsolaMont->Codigo.c_str());
			tvDefeito->Items->AddChildObject(node, aux, pdef->ChvIsolaMont);

			aux.sprintf("FECHA");
			if (ChaveAutomatica(pdef->ChvIsolaAutoMont))
				aux.cat_sprintf("[AUTO]");
			else
				aux.cat_sprintf("[MANUAL]");
			aux.cat_sprintf("[%s]", pdef->ChvIsolaAutoMont->Codigo.c_str());
			tvDefeito->Items->AddChildObject(node, aux, pdef->ChvIsolaAutoMont);
		}
	}
	else
	{
		if (pdef->LisBlocosProtDef->Count > 0)
		{
			node = tvDefeito->Items->AddChildObject(nodeRaiz, "MONTANTE_LENTO", pdef);

			aux.sprintf("ABRE");
			if (ChaveAutomatica(pdef->ChvIsolaMont))
				aux.cat_sprintf("[AUTO]");
			else
				aux.cat_sprintf("[MANUAL]");
			aux.cat_sprintf("[%s]", pdef->ChvIsolaMont->Codigo.c_str());
			tvDefeito->Items->AddChildObject(node, aux, pdef->ChvIsolaMont);

			aux.sprintf("FECHA");
			if (ChaveAutomatica(pdef->ChvProt))
				aux.cat_sprintf("[AUTO]");
			else
				aux.cat_sprintf("[MANUAL]");
			aux.cat_sprintf("[%s]", pdef->ChvProt->Codigo.c_str());
			tvDefeito->Items->AddChildObject(node, aux, pdef->ChvProt);
		}
	}

	node = tvDefeito->Items->AddChildObject(nodeRaiz, "JUSANTE", pdef);

	for (int np = 0; np < numPat; np++)
	{
		estratSoc = pdef->EstratSoc[np];
		if (estratSoc->lisVisRedeSoc->Count == 0)
			continue;
		aux.sprintf("Patamar: [%i]", np);
		node1 = tvDefeito->Items->AddChildObject(node, aux, estratSoc);
		for (int nvis = 0; nvis < estratSoc->lisVisRedeSoc->Count; nvis++)
		{
			// operações de isolament/socorro, desligamento/volta
			visRedeSoc = (VTVisRedeSoc*)estratSoc->lisVisRedeSoc->Items[nvis];
			for (int nisola = 0; nisola < visRedeSoc->lisChvIsola->Count; nisola++)
			{
				pchave = (VTChave*)visRedeSoc->lisChvIsola->Items[nisola];
				aux.sprintf("ABRE");
				if (ChaveAutomatica(pchave))
					aux.cat_sprintf("[AUTO]");
				else
					aux.cat_sprintf("[MANUAL]");
				aux.cat_sprintf("[%s]", pchave->Codigo.c_str());
				tvDefeito->Items->AddChildObject(node1, aux, pchave);
			}

			pchave = visRedeSoc->visRede->pchave;
			aux.sprintf("FECHA");
			if (ChaveAutomatica(pchave))
				aux.cat_sprintf("[AUTO]");
			else
				aux.cat_sprintf("[MANUAL]");
			aux.cat_sprintf("[%s]", pchave->Codigo.c_str());
			tvDefeito->Items->AddChildObject(node1, aux, pchave);
		}
	}
	// Adiciona elementos a tvDefeito1
	tvDefeito1->Items->Clear();
	node = tvDefeito1->Items->AddChildObject(NULL, "M", NULL);
	//
	aux.sprintf("PROT[%s]", pdef->ChvProt->Codigo.c_str());
	node1 = tvDefeito1->Items->AddChildObject(node, aux, pdef->ChvProt);
	tvDefeito1->Items->AddChildObject(node1, "LisBlocosProtDef", pdef->LisBlocosProtDef);
	//

	aux.sprintf("CHVISOLAUTOMONT[");
	if (pdef->ChvIsolaAutoMont != NULL)
		aux.cat_sprintf("%s", pdef->ChvIsolaAutoMont->Codigo.c_str());
	aux.cat_sprintf("]");
	node1 = tvDefeito1->Items->AddChildObject(node, aux, pdef->ChvIsolaAutoMont);
	tvDefeito1->Items->AddChildObject(node1, "LisBlocosProtAuto", pdef->LisBlocosProtAuto);
	tvDefeito1->Items->AddChildObject(node1, "LisBlocosAutoDef", pdef->LisBlocosAutoDef);

	aux.sprintf("CHVISOLMONT[");
	if (pdef->ChvIsolaMont != NULL)
		aux.cat_sprintf("%s", pdef->ChvIsolaMont->Codigo.c_str());
	aux.cat_sprintf("]");
	node1 = tvDefeito1->Items->AddChildObject(node, aux, pdef->ChvIsolaMont);

	node = tvDefeito1->Items->AddChildObject(NULL, "J", NULL);
	for (int np = 0; np < numPat; np++)
	{
		estratSoc = pdef->EstratSoc[np];
		aux.sprintf("Patamar - lisBlocosReparo [%i]", np);
		node1 = tvDefeito1->Items->AddChildObject(node, aux, estratSoc->lisBlocosReparo);
		for (int nvis = 0; nvis < estratSoc->lisVisRedeSoc->Count; nvis++)
		{
			visRedeSoc = (VTVisRedeSoc*)estratSoc->lisVisRedeSoc->Items[nvis];
			aux.sprintf("visRede %i [%s]", nvis, visRedeSoc->visRede->pchave->Codigo.c_str());
			tvDefeito1->Items->AddChildObject(node1, aux, visRedeSoc->visRede->pchave);
			tvDefeito1->Items->AddChildObject(node1, "lisBlocosSoc", visRedeSoc->lisBlocosSoc);
			for (int nchv = 0; nchv < visRedeSoc->lisChvIsola->Count; nchv++)
			{
				pchave = (VTChave*)visRedeSoc->lisChvIsola->Items[nchv];
				aux.sprintf("chvIsola %i", nchv);
				tvDefeito1->Items->AddChildObject(node1, aux, pchave);
			}
		}
	}

	// Adiciona elementos a tvVisDefeito
	tvVisDefeito->Items->Clear();

	for (int nc = 0; nc < pdef->LisVisRedeDef->Count; nc++)
	{
		visRede = (VTVisRede*)pdef->LisVisRedeDef->Items[nc];
		pchave = visRede->pchave;
		// pchave = (VTChave*)pdef->LisChvVis->Items[nc];
		// visRede = calcIndCont->IdentificaVisRede(pchave);
		aux.sprintf("%s", visRede->pchave->Codigo.c_str());
		if (ChaveAutomatica(visRede->pchave))
			aux.cat_sprintf("[AUTO]");
		else
			aux.cat_sprintf("[MANUAL]");

		node = tvVisDefeito->Items->AddChildObject(NULL, aux, visRede);

		for (int nb = visRede->lisBlocoRadIsolaJus->Count - 1; nb >= 0; nb--)
		{
			blRad = (VTBlocoRad*)visRede->lisBlocoRadIsolaJus->Items[nb];
			aux.sprintf("%s", blRad->Chave->Codigo.c_str());
			if (ChaveAutomatica(blRad->Chave))
				aux.cat_sprintf("[AUTO]");
			else
				aux.cat_sprintf("[MANUAL]");
			node1 = tvVisDefeito->Items->AddChildObject(node, aux, blRad);
			for (int np = 0; np < numPat; np++)
			{
				carga = blRad->CargaBlRad[np];
				folga = visRede->FolgaIA(np) * (visRede->pchave->pbarra1->vnom * 1.73205) / 1000.0;
				// n14 folga = calcIndCont->FolgaIA(visRede, np) * (visRede->pchave->pbarra1->vnom * 1.73205) / 1000.0;
				aux.sprintf("pat[%i]: folga: %6.2f / carga: %6.2f", np, folga, carga);
				// node2 =
				tvVisDefeito->Items->AddChildObject(node1, aux, blRad);
				// aux.sprintf("ABRE");
				// if(buscaProt->ChaveAutomatica(blRad->Chave))
				// aux.cat_sprintf("[AUTO]");
				// else
				// aux.cat_sprintf("[MANUAL]");
				// aux.cat_sprintf("[%s]", blRad->Chave->Codigo);
				// tvVisDefeito->Items->AddChildObject(node2, aux, blRad);
			}
		}
	}

	nodeRaiz->Expand(true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMostraDefeito::ActionMostraBlocoDefeitoExecute(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	grafico->DestacaEqpto(pdef->Bloco->LisLigacao(), clRed, 10);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMostraDefeito::ActionMostraDefeito1Execute(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TTreeNode *node, *nodeAux;
	VTDefeito *pdef;
	TList *lisAux;
	VTChave *pchave;
	VTEstratSoc *estratSoc;
	VTVisRedeSoc* visRedeSoc;
	TList *lisBlocos;

	grafico->Moldura();
	if ((node = tvDefeito1->Selected) == NULL)
		return;

	nodeAux = node;
	while (nodeAux->Level != 0)
		nodeAux = nodeAux->Parent;

	if (node->Level == 0)
		return;
	if (node->Level == 1)
	{
		if (nodeAux->Text.Compare("M") == 0)
		{
			pchave = (VTChave*)node->Data;
			grafico->Moldura(pchave);
		}

		if (nodeAux->Text.Compare("J") == 0)
		{
			lisBlocos = (TList*)node->Data;
			DestacaEqptos(lisBlocos); // n14, clRed);
		}
	}

	if (node->Level == 2)
	{
		if (nodeAux->Text.Compare("M") == 0)
		{
			lisBlocos = (TList*)node->Data;
			DestacaEqptos(lisBlocos); // n14, clRed);
		}
		if (nodeAux->Text.Compare("J") == 0)
		{
			if (node->Text.SubString(1, 5).Compare("lisBl") == 0)
			{
				lisBlocos = (TList*)node->Data;
				DestacaEqptos(lisBlocos); // n14, clRed);
			}
			else
			{
				pchave = (VTChave*)node->Data;
				grafico->Moldura(pchave);
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMostraDefeito::ActionMostraDefeitoExecute(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TTreeNode *node;
	VTDefeito *pdef;
	TList *lisAux;
	VTChave *pchave;
	VTEstratSoc *estratSoc;
	VTVisRede *visRede;
	VTVisRedeSoc* visRedeSoc;

	grafico->Moldura();

	if ((node = tvDefeito->Selected) == NULL)
		return;
	if (node->Level == 0)
	{
		lisAux = new TList();
		pdef = (VTDefeito*)node->Data;
		lisAux->Add(pdef->Bloco);
		DestacaEqptos(lisAux);
	}

	if (node->Level == 1)
	{
		pdef = (VTDefeito*)node->Data;
		if (node->Text.Compare("LISBLOCOSVIS") == 0)
		{
			DestacaEqptos(pdef->LisBlocosVis, clGreen);
		}

		if (node->Text.Compare("LISCHVBLOCOVIS") == 0)
		{
			lisAux = new TList();
			lisAux->Assign(pdef->LisChvBlocoVis, laOr);
			grafico->Moldura(lisAux);
			delete lisAux;
			// DestacaEqptos(pdef->LisBlocosVis, clGreen);
		}

		if (node->Text.Compare("LISCHVBLOCOMAN") == 0)
		{
			grafico->Moldura(pdef->LisChavesBlocosMan);
			DestacaEqptos(pdef->LisBlocosMan);
		}

		if (node->Text.Compare("LISCHVALIVIOVIS") == 0)
		{
			lisAux = new TList();
			lisAux->Assign(pdef->LisChvAlivioVis, laOr);
			grafico->Moldura(lisAux);
			delete lisAux;
			// DestacaEqptos(pdef->LisBlocosVis, clGreen);
		}

		if (node->Text.Compare("LISCHVJUS") == 0)
		{
			lisAux = new TList();
			lisAux->Assign(pdef->LisChvJus, laOr);
			grafico->Moldura(lisAux);
			delete lisAux;
			// DestacaEqptos(pdef->LisBlocosVis, clGreen);
		}

		if (node->Text.Compare("LISVISREDEDEF") == 0)
		{
			lisAux = new TList();
			lisAux->Assign(pdef->LisChvVisDef, laOr);
			grafico->Moldura(lisAux);
			delete lisAux;
			// DestacaEqptos(pdef->LisBlocosVis, clGreen);
		}

		if (node->Text.Compare("RELIGA") == 0)
		{
			// DestacaEqptos(pdef->LisBlocosJusProt);//n14, clRed);
			grafico->Moldura(pdef->ChvReliga);
		}

		if (node->Text.Compare("PROT") == 0)
		{
			DestacaEqptos(pdef->LisBlocosJusProt); // n14, clRed);
			grafico->Moldura(pdef->ChvProt);
		}

		if (node->Text.Compare("MONTANTE_RAPIDO") == 0)
		{
			DestacaEqptos(pdef->LisBlocosJusAuto); // n14, clRed);
			lisAux = new TList();
			lisAux->Add(pdef->ChvIsolaAutoMont);
			lisAux->Add(pdef->ChvProt);
			grafico->Moldura(lisAux);
			delete lisAux;
		}

		if (node->Text.Compare("MONTANTE_LENTO") == 0)
		{
			lisAux = new TList();
			lisAux->Assign(pdef->LisBlocosJusDef, laOr);
			lisAux->Add(pdef->Bloco);
			DestacaEqptos(lisAux); // n14, clRed);

			lisAux->Clear();
			lisAux->Add(pdef->ChvIsolaMont);
			if (pdef->ChvIsolaAutoMont != NULL)
				lisAux->Add(pdef->ChvIsolaAutoMont);
			else
				lisAux->Add(pdef->ChvProt);
			grafico->Moldura(lisAux);
			delete lisAux;
		}

		if (node->Text.Compare("JUSANTE") == 0)
		{
			lisAux = new TList();
			lisAux->Assign(pdef->LisChvVisDef, laOr);
			grafico->Moldura(lisAux);
			delete lisAux;
		}
	}

	if (node->Level == 2)
	{
		if (node->Parent->Text.Compare("JUSANTE") != 0)
		{
			if (node->Parent->Text.Compare("LISVISREDEDEF") == 0)
			{
				visRede = (VTVisRede*)node->Data;
				lisAux = new TList();
				for (int nb = 0; nb < visRede->lisBlocoRadIsolaJus->Count; nb++)
				{
					lisAux->Add(((VTBlocoRad*)visRede->lisBlocoRadIsolaJus->Items[nb])->Bloco);
				}
				grafico->Moldura(visRede->pchave);
				DestacaEqptos(lisAux);
				delete lisAux;
			}
			else
			{
				pchave = (VTChave*)node->Data;
				grafico->Moldura(pchave);
			}
		}
		else
		{
			estratSoc = (VTEstratSoc*)node->Data;

			// CalculaEstrategiaSoc(estratSoc->Defeito, estratSoc->npat);
			DestacaEqptos(estratSoc->lisBlocosReparo); // n14, clRed);

			lisAux = new TList();
			for (int nvis = 0; nvis < estratSoc->lisVisRedeSoc->Count; nvis++)
			{
				visRedeSoc = (VTVisRedeSoc*)estratSoc->lisVisRedeSoc->Items[nvis];
				lisAux->Add(visRedeSoc->visRede->pchave);
				// operações de isolament/socorro, desligamento/volta
				for (int nisola = 0; nisola < visRedeSoc->lisChvIsola->Count; nisola++)
				{
					lisAux->Add(visRedeSoc->lisChvIsola->Items[nisola]);
				}
			}
			grafico->Moldura(lisAux);
			delete lisAux;
		}
	}
	if (node->Level == 3)
	{
		if (node->Parent->Parent->Text.Compare("LISVISREDEDEF") == 0)
		{
			visRede = (VTVisRede*)node->Data;
			if (node->Text.Compare("LISBLOCOSVIS") == 0)
			{
				lisAux = new TList();
				for (int nb = 0; nb < visRede->lisBlocosVis->Count; nb++)
				{
					lisAux->Add(visRede->lisBlocosVis->Items[nb]);
				}
				grafico->Moldura(visRede->pchave);
				DestacaEqptos(lisAux);
				delete lisAux;
			}
			if (node->Text.Compare("LISCHVBLOCOVIS") == 0)
			{
				grafico->Moldura(visRede->lisChvBlocoVis);
			}
			if (node->Text.Compare("LISCHVALIVIOVIS") == 0)
			{
				grafico->Moldura(visRede->lisChvAlivioVis);
			}
		}
		else
		{
			estratSoc = (VTEstratSoc*)node->Parent->Data;

			// CalculaEstrategiaSoc(estratSoc->Defeito, estratSoc->npat);
			DestacaEqptos(estratSoc->lisBlocosReparo); // n14, clRed);
			pchave = (VTChave*)node->Data;
			grafico->Moldura(pchave);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMostraDefeito::ActionMostraDefeitoVisExecute(TObject *Sender)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TTreeNode *node;
	VTVisRede *visRede;
	VTBlocoRad *blocoRad;
	TList *lisAux;

	grafico->Moldura();
	if ((node = tvVisDefeito->Selected) == NULL)
		return;

	if (node->Level == 0)
	{
		visRede = (VTVisRede*)node->Data;
		grafico->Moldura(visRede->pchave);
		grafico->DestacaEqpto(visRede->blocoDesl->LisLigacao(), clGreen, 10);

	}

	if (node->Level == 1)
	{
		lisAux = new TList();
		blocoRad = (VTBlocoRad*)node->Data;
		for (int nb = 0; nb < blocoRad->LisBlocoJus->Count; nb++)
		{
			lisAux->Assign(((VTBloco*)blocoRad->LisBlocoJus->Items[nb])->LisLigacao(), laOr);
		}

		lisAux->Assign(blocoRad->Bloco->LisLigacao(), laOr);
		grafico->DestacaEqpto(lisAux, clGreen, 10);

		lisAux->Clear();
		lisAux->Add(blocoRad->Chave);
		lisAux->Add(((VTVisRede*)(node->Parent->Data))->pchave);
		grafico->Moldura(lisAux);
		delete lisAux;
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormMostraDefeito::DestacaEqptos(TList *lisBlocos, TColor color)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TList *lisLigacao;
	VTLigacao *ligacao;

	lisLigacao = new TList();
	for (int n = 0; n < lisBlocos->Count; n++)
		lisLigacao->Assign(((VTBloco*)lisBlocos->Items[n])->LisLigacao(), laOr);
	if (color == clBlack)
	{
		if (lisLigacao->Count > 0)
		{
			ligacao = (VTLigacao*)lisLigacao->First();
			color = ligacao->rede->Color;
		}
	}

	grafico->DestacaEqpto(lisLigacao, color, 10);

	delete lisLigacao;
}
// ---------------------------------------------------------------------------
