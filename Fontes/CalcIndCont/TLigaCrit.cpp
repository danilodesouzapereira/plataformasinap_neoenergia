// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TLigaCrit.h"
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTLigaCrit* __fastcall NewObjLigaCrit(VTVisRede* visRede)
{
	return (new TLigaCrit(visRede));
}

// ---------------------------------------------------------------------------
__fastcall TLigaCrit::TLigaCrit(VTVisRede* visRede)
{
	this->visRede = visRede;
	lisDelta = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TLigaCrit::~TLigaCrit(void)
{
	if (lisDelta)
	{
		LimpaTList(lisDelta);
		delete lisDelta;
		lisDelta = NULL;
	}
}

// ---------------------------------------------------------------------------
