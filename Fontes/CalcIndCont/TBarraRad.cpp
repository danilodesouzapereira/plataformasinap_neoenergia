// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TBarraRad.h"
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTBarraRad* __fastcall NewObjBarraRad(VTBarra *pbarra)
{
	return (new TBarraRad(pbarra));
}

// ---------------------------------------------------------------------------
__fastcall TBarraRad::TBarraRad(VTBarra *pbarra)
{
	this->pbarra = pbarra;
	lisBarRad = new TList();
	pbarra->Obj = NULL;
	pligaPai = NULL;
	pbarPai = NULL;
	pbarRadPai = NULL;
	nconsMon = 0; // Número de consumidores a montante
	nconsJus = 0; // numero de consumidores a jusante
	dcompJus = 0.0; // Comprimento a jusante
}

// ---------------------------------------------------------------------------
__fastcall TBarraRad::~TBarraRad(void)
{
	// destroi listas e objetos
	if (lisBarRad)
	{
		LimpaTList(lisBarRad);
		delete lisBarRad;
		lisBarRad = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TBarraRad::Inicia(void)
{
	TBarraRad *barRad;
	pbarra = NULL;
	pligaPai = NULL;
	pbarPai = NULL;
	pbarRadPai = NULL;
	// Delete os descendentes e limpa a lista
	for (int nb = 0; nb < lisBarRad->Count; nb++)
	{
		barRad = (TBarraRad*)lisBarRad->Items[nb];
		delete barRad;
	}
	lisBarRad->Clear();
}
// ---------------------------------------------------------------------------
