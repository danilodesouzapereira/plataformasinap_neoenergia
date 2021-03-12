// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TVisRede.h"
#include "VTLigaCrit.h"
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTVisRede* __fastcall NewObjVisRede(VTChave* pchave)
{
	return (new TVisRede(pchave));
}

// ---------------------------------------------------------------------------
__fastcall TVisRede::TVisRede(VTChave *pchave)
{
	this->pchave = pchave;
	plisBlocoChave = NULL;
	rede1 = NULL;
	rede2 = NULL;
	bloco1 = NULL;
	bloco2 = NULL;
	blocoDesl = NULL; // bloco1 ou bloco2 desligado
	blocoRadDesl = NULL;
	blocoRadSup = NULL; // blocoRad de suprimento
	pbarra1 = NULL;
	pbarra2 = NULL;

	lisLiga1 = new TList();
	lisLiga2 = new TList();

	lisBlocosDefVis = new TList();
	chvJus = NULL; // chave a justante do defeito
	lisBlocoRadIsolaJus = new TList();
	// lista de blocosRad que apontam para as chaves de isolamento da jusante
	// para a montante até chvJus

	lisChvBlocoVis = new TList();
	lisChvAlivioVis = new TList();
	lisBlocosVis = new TList();

	ligaCrit1 = NewObjLigaCrit(this); // new TLigaCrit(this);
	ligaCrit2 = NewObjLigaCrit(this); // new TLigaCrit(this);

	lisBlocosVis1 = new TList();
	lisBlocosVis2 = new TList();

}

// ---------------------------------------------------------------------------
__fastcall TVisRede::~TVisRede(void)
{
	if (lisLiga1)
	{
		delete lisLiga1;
		lisLiga1 = NULL;
	}
	if (lisLiga2)
	{
		delete lisLiga2;
		lisLiga2 = NULL;
	}
	if (lisBlocosDefVis)
	{
		delete lisBlocosDefVis;
		lisBlocosDefVis = NULL;
	}
	if (ligaCrit1)
	{
		delete ligaCrit1;
		ligaCrit1 = NULL;
	}
	if (ligaCrit2)
	{
		delete ligaCrit2;
		ligaCrit2 = NULL;
	}
	if (lisBlocosVis1)
	{
		delete lisBlocosVis1;
		lisBlocosVis1 = NULL;
	}
	if (lisBlocosVis2)
	{
		delete lisBlocosVis2;
		lisBlocosVis2 = NULL;
	}
	if (lisBlocoRadIsolaJus)
	{
		delete lisBlocoRadIsolaJus;
		lisBlocoRadIsolaJus = NULL;
	}
	if (lisChvBlocoVis)
	{
		delete lisChvBlocoVis;
		lisChvBlocoVis = NULL;
	}
	if (lisChvAlivioVis)
	{
		delete lisChvAlivioVis;
		lisChvAlivioVis = NULL;
	}
	if (lisBlocosVis)
	{
		delete lisBlocosVis;
		lisBlocosVis = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TVisRede::IniciaDadosDefeito(void)
{
	blocoDesl = NULL; // bloco ligado a chave de vis do lado do circuito em falha
	blocoRadDesl = NULL; // blocoRadDesl->bloco = blocoDesl
	lisBlocosDefVis->Clear();
	// Lista de blocos a justante do defeito até chave vis (nao inclui o bloco de defeito)
	lisBlocoRadIsolaJus->Clear();
	// lista de blocosRad que apontam para chaves de isolamento da montante, de chvJus, para a jusante
	blRadJusDef = NULL;
	// blocoRad imediatamente a justante do defeito, 1o elemento de lisBlocoRadIsolaJus
	chvJus = NULL; // chave imediatamente a jusante do blocos de defeito que liga a chave de vis
}

// ---------------------------------------------------------------------------
double __fastcall TVisRede::FolgaIA(int indPat)
{
	// VTBloco  *blocoVis;
	VTLigaCrit *ligaCrit;
	strDelta *delta;
	double folga = 0.0;

	// Verifica se a classe já foi inicializada
	// if(!flagInicia)
	// {if(!Inicia()) return(-1);}

	if ((bloco1 != blocoDesl) && (bloco2 != blocoDesl))
		return (0.0);

	if (bloco1 == blocoDesl)
	{ // blocoVis = bloco2;
		ligaCrit = ligaCrit2;
	}

	if (bloco2 == blocoDesl)
	{ // blocoVis = bloco1;
		ligaCrit = ligaCrit1;
	}

	// Busca a folga do patamar selecionado
	delta = NULL;
	for (int n = 0; n < ligaCrit->lisDelta->Count; n++)
	{
		delta = (strDelta*)ligaCrit->lisDelta->Items[n];
		if (delta->ipat == indPat)
			break;
		delta = NULL;
	}

	if (delta != NULL)
		folga = delta->dMax;

	return (folga);
}

// ---------------------------------------------------------------------------
VTEqpto* __fastcall TVisRede::LigaCrit(int indPat)
{
	// VTBloco  *blocoVis;
	VTLigaCrit *ligaCrit;
	strDelta *delta;
	VTEqpto *eqptoCrit = NULL;

	// Verifica se a classe já foi inicializada
	// if(!flagInicia)
	// {if(!Inicia()) return(-1);}

	if ((bloco1 != blocoDesl) && (bloco2 != blocoDesl))
		return (NULL);

	if (bloco1 == blocoDesl)
	{ // blocoVis = bloco2;
		ligaCrit = ligaCrit2;
	}

	if (bloco2 == blocoDesl)
	{ // blocoVis = bloco1;
		ligaCrit = ligaCrit1;
	}

	// Busca a folga do patamar selecionado
	delta = NULL;
	for (int n = 0; n < ligaCrit->lisDelta->Count; n++)
	{
		delta = (strDelta*)ligaCrit->lisDelta->Items[n];
		if (delta->ipat == indPat)
			break;
		delta = NULL;
	}

	if (delta != NULL)
		eqptoCrit = delta->pliga;

	return (eqptoCrit);
}

// ---------------------------------------------------------------------------
double __fastcall TVisRede::IA_Crit(int indPat)
{
	VTLigaCrit *ligaCrit = NULL;
	strDelta *delta;
	// VTLigacao *pliga;
	double ia_crit;

	// Verifica se a classe já foi inicializada
	// if(!flagInicia)
	// {if(!Inicia()) return(-1);}
	if ((bloco1 != blocoDesl) && (bloco2 != blocoDesl))
		return (-1.0);

	if (bloco1 == blocoDesl)
		ligaCrit = ligaCrit2;

	if (bloco2 == blocoDesl)
		ligaCrit = ligaCrit1;

	if (ligaCrit == NULL)
		return (0.0);
	// Busca a folga do patamar selecionado
	delta = NULL;
	for (int n = 0; n < ligaCrit->lisDelta->Count; n++)
	{
		delta = (strDelta*)ligaCrit->lisDelta->Items[n];
		if (delta->ipat == indPat)
			break;
		delta = NULL;
	}

	ia_crit = 9999.;
	if (delta != NULL)
		ia_crit = delta->ia_nom;

	return (ia_crit);
}
// ---------------------------------------------------------------------------
