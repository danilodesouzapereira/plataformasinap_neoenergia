// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
// #include <Math.hpp>
// #include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "..\Complexo\Complexo.h"
#include "..\Constante\Const.h"
#include "..\Constante\Fases.h"
#include "..\Rede\VTCarga.h"
#include "TGrupo.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTGrupo * __fastcall NewObjGrupo(void)
{
	try
	{
		return (new TGrupo());
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TGrupo::TGrupo(void)
{
	lisCargas = new TList;
	lisCargasAjus = new TList;
	categoria = catAD_INV;
	peso = 1;

	for (int nf = indFASE_A; nf < MAX_FASE; nf++)
	{
		proporcaoP[nf] = 1.;
		proporcaoQ[nf] = 1.;
		p_car_ajus_mw[nf] = 0.;
		q_car_ajus_mvar[nf] = 0.;
		fatcorP[nf] = 1.0;
		fatcorQ[nf] = 1.0;
		s_car_ajus_mva[nf] = CZero();

	}
}

// ---------------------------------------------------------------------------
__fastcall TGrupo::~TGrupo(void)
{
	// destroi a lista sem destruir os objetos
	if (lisCargas)
	{
		delete lisCargas;
		lisCargas = NULL;
	}
	if (lisCargasAjus)
	{
		delete lisCargasAjus;
		lisCargasAjus = NULL;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TGrupo::Inicia(void)
{
	VTCarga *carga;

	lisCargasAjus->Clear();
	//percorre todas cargas
	for (int nc = 0; nc < lisCargas->Count; nc++)
	{
		carga = (VTCarga*) lisCargas->Items[nc];
		if (carga->Ajustavel)
		{
            lisCargasAjus->Add(carga);
		}
	}
}
// ---------------------------------------------------------------------------
// eof
