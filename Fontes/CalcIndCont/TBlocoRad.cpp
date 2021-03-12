// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TBlocoRad.h"
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Bloco\VTBlocos.h>
#include <PlataformaSinap\Fontes\Bloco\VTElo.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// COMENTÁRIOS
// hk201610: definição de rede de bloco: inclusao de apl no construtor
// hk201610: rede de blocoRad : VTRede     *rede
// hk201610: definição de rede de bloco :rede = RedeBloco(bloco);
// hk201610: definição de rede de bloco : PM_GetRede(void)
// hk201611: criacao de ponteiro de blocoRad para plisBlocoChave a montante

// ---------------------------------------------------------------------------
VTBlocoRad* __fastcall NewObjBlocoRad(VTBloco *bloco, VTApl *apl)
	// hkv201610: definição de rede de bloco: inclusao de apl
{
	return (new TBlocoRad(bloco, apl));
}

// ---------------------------------------------------------------------------
__fastcall TBlocoRad::TBlocoRad(VTBloco *bloco, VTApl *apl)
{
	this->apl = apl;
	this->bloco = bloco;
	// Inicializacao de rede de blocoRad
	rede = RedeBloco(bloco); // hkv201610: definição de rede de bloco :rede = RedeBloco(bloco);
	pchave = NULL;
	plisBlocoChave = NULL; // hk201611
	blocoRadMon = NULL;
	lisBlocoRad = new TList();
	cargaBlRad = NULL;
	// lista de blocosRad a justante e a montante do bloco
	lisBlocoRadJus = new TList();
	// lista de blocos a justante e a montante do bloco
	lisBlocoJus = new TList();
	// listante de blocos a jusantes até próxima chave de protecao
	lisBlocoJusProt = new TList();

	lisBlocoMon = new TList();
	numPat = 0;
	flagSup = false;

    ncons = nconsJus = 0;
}

// ---------------------------------------------------------------------------
__fastcall TBlocoRad::~TBlocoRad(void)
{
	// destroi listas e objetos
	if (lisBlocoRad)
	{
		LimpaTList(lisBlocoRad);
		delete lisBlocoRad;
		lisBlocoRad = NULL;
	}
	if (lisBlocoRadJus)
	{
		delete lisBlocoRadJus;
		lisBlocoRadJus = NULL;
	}
	if (lisBlocoJus)
	{
		delete lisBlocoJus;
		lisBlocoJus = NULL;
	}
	if (lisBlocoJusProt)
	{
		delete lisBlocoJusProt;
		lisBlocoJusProt = NULL;
	}
	if (cargaBlRad)
	{
		delete[]cargaBlRad;
		cargaBlRad = NULL;
	}
	if (cargaBloco)
	{
		delete[]cargaBloco;
		cargaBloco = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TBlocoRad::IniciaCarga(int numPat)
{
	// destroi cargaBlRad
	if (cargaBlRad)
	{
		delete[]cargaBlRad;
		cargaBlRad = NULL;
	}
	if (cargaBloco)
	{
		delete[]cargaBloco;
		cargaBloco = NULL;
	}
	cargaBlRad = new double[numPat];
	cargaBloco = new double[numPat];
	for (int np = 0; np < numPat; np++)
	{
		cargaBlRad[np] = 0.0;
		cargaBloco[np] = 0.0;
	}
	this->numPat = numPat;
}

// ---------------------------------------------------------------------------
void __fastcall TBlocoRad::LisBlRadJus(TList *lisEXT)
{
	TList *lisAux;
	TBlocoRad *blRadAux;
	// Funcao que retorna blocos a jusante do blRad->bloco
	lisAux = new TList();
	lisAux->Add(this);
	while (lisAux->Count > 0)
	{
		blRadAux = (TBlocoRad*)lisAux->First();
		lisAux->Delete(0);
		lisAux->Assign(blRadAux->lisBlocoRad, laOr);
		lisEXT->Assign(blRadAux->lisBlocoRad, laOr);
	}
	delete lisAux;
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TBlocoRad::RedeBloco(VTBloco *bloco) // hkv201610: definição de rede de bloco
{
	// Funcao para definição de rede de bloco
	VTBlocos *blocos;
	TList *lisElo;
	VTElo *elo;
	VTRede *rede;
	VTChave *chave;
	if (bloco->Rede != NULL)
		return (bloco->Rede);
	// hkv201610: para bloco com 1 barra atribui rede a chave fechada ligada a barra
	blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	lisElo = new TList();
	blocos->LisElo(bloco, lisElo);
	rede = NULL;
	// Busca a primeira chave fechada ligada a bloco com somente 1 barra
	for (int nelo = 0; nelo < lisElo->Count; nelo++)
	{
		elo = (VTElo*)lisElo->Items[nelo];
		chave = elo->Chave;
		if (chave->Estado == chvABERTA)
			continue; // hk201612
		if (chave->rede == NULL)
			continue;
		rede = elo->Chave->rede;
		break;
	}
	delete lisElo;
	return (rede);
}

// ---------------------------------------------------------------------------
double __fastcall TBlocoRad::PM_GetCargaBlRad(int iPat)
{
	if (!(numPat > 0))
		return (-1.0);
	if (iPat > (numPat - 1))
		return (-1.0);

	return (cargaBlRad[iPat]);
}

// ---------------------------------------------------------------------------
double __fastcall TBlocoRad::PM_GetCargaBloco(int iPat)
{
	if (!(numPat > 0))
		return (-1.0);
	if (iPat > (numPat - 1))
		return (-1.0);

	return (cargaBloco[iPat]);
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TBlocoRad::PM_GetRede(void)
	// hkv201610: definição de rede de bloco : PM_GetRede(void)
{
	// Funcao do bloco que retorna a rede
	VTRede *rede;
	rede = RedeBloco(bloco);
	return (rede);
}

// ---------------------------------------------------------------------------
void __fastcall TBlocoRad::PM_SetCargaBlRad(int iPat, double valor)
{
	if (!(numPat > 0))
		return;
	if (iPat > (numPat - 1))
		return;
	cargaBlRad[iPat] = valor;
}

// ---------------------------------------------------------------------------
void __fastcall TBlocoRad::PM_SetCargaBloco(int iPat, double valor)
{
	if (!(numPat > 0))
		return;
	if (iPat > (numPat - 1))
		return;
	cargaBloco[iPat] = valor;
}
// ---------------------------------------------------------------------------
