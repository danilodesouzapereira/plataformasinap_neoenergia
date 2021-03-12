// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TLisBlocoChave.h"
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTLisBlocoChave* __fastcall NewObjLisBlocoChave(VTChave* pchave)
{
	return (new TLisBlocoChave(pchave));
}

// ---------------------------------------------------------------------------
__fastcall TLisBlocoChave::TLisBlocoChave(VTChave *pchv)
{
	this->pchv = pchv;
	// cria listas
	lisBlJ = new TList(); // Todos os blocos a justante
	lisBlReliga = new TList(); // lista de blocos de religamento, para chaves religadoras
	// flagBlocos = false;
	blJus = NULL; // BlocoRad a jusante
	blMon = NULL; // BlocoRad a montante
	blSup = NULL; // blocoRad de suprimento
	// para o caso de chave NA entre alimentadores
	blMon1 = NULL; // BlocoRad a montante
	blSup1 = NULL; // BlocoRad de suprimento em um dos circuitos de viz
}

// ---------------------------------------------------------------------------
__fastcall TLisBlocoChave::~TLisBlocoChave(void)
{
	// destroi listas e objetos
	if (lisBlJ)
	{
		delete lisBlJ;
		lisBlJ = NULL;
	}
	if (lisBlReliga)
	{
		delete lisBlReliga;
		lisBlReliga = NULL;
	}
}

// ---------------------------------------------------------------------------
