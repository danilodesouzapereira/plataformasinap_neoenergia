// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TLisDefeitoRede.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TLisDefeitoRede::TLisDefeitoRede(VTRede *rede)
{
	this->rede = rede;
	lisDefeitos = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TLisDefeitoRede::~TLisDefeitoRede(void)
{
	if (lisDefeitos)
	{
		delete lisDefeitos;
		lisDefeitos = NULL;
	}
}
// ---------------------------------------------------------------------------
