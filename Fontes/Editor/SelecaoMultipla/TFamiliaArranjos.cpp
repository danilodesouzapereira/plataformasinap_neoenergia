// ---------------------------------------------------------------------------

#pragma hdrstop

#include "TFamiliaArranjos.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTFamiliaArranjos* __fastcall NewObjFamiliaArranjos(VTApl *apl)
{
	try
	{ // cria objeto
		return (new TFamiliaArranjos(apl));
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TFamiliaArranjos::TFamiliaArranjos(VTApl *apl)
{
	this->apl = apl;

	//inicializa o vetor
	for (int na = 0; na < fa_MAX_FASE; na++)
	{
		arranjos[na] = NULL;
	}
}

// ---------------------------------------------------------------------------
__fastcall TFamiliaArranjos::~TFamiliaArranjos(void)
{

}
// ---------------------------------------------------------------------------
bool __fastcall TFamiliaArranjos::InsereArranjo(VTArranjo *arranjo)
{
	int fases;
	bool sucesso = true;

	fases = arranjo->Fases;
	if (fases >= fa_MAX_FASE)
	{
    	return false;
	}
	//verifica se
	if (arranjos[fases] == NULL)
	{
		arranjos[fases] = arranjo;
	}
	else
	{
		sucesso = false;
    }

	return sucesso;
}
// ---------------------------------------------------------------------------
VTArranjo* __fastcall TFamiliaArranjos::PM_GetArranjo(int fase)
{
	VTArranjo *arranjo;

	arranjo = arranjos[fase];

	return arranjo;
}
// ---------------------------------------------------------------------------
