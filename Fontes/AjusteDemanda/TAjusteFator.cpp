//---------------------------------------------------------------------------

#pragma hdrstop
#include "..\Rede\VTRede.h"
#include "..\ImportaMedicao\VTMedicao.h"
#include "TAjusteFator.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall  TAjusteFator::TAjusteFator(void)
{
   Ativo = false;
   FatorCorrecP = 1.0;
   FatorCorrecQ = 1.0;
   FatorCorrecFP = 1.0;
   TipoFatorCorrec = adtfFATOR_UNICO;
   Rede = NULL;
   Medicao = NULL;
}
//---------------------------------------------------------------------------
__fastcall TAjusteFator::~TAjusteFator(void)
{
	//faz nada
}
//---------------------------------------------------------------------------
