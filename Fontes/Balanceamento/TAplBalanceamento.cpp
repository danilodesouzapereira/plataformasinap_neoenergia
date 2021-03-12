//---------------------------------------------------------------------------
#pragma hdrstop
#include "TAplBalanceamento.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\Zona.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TAplBalanceamento::TAplBalanceamento(TForm *form_owner, VTApl *apl_owner)
	:TApl(form_owner, apl_owner)
	{
	//cria objetos necessários
	//VTZonas
	this->lisOBJ->Add(DLL_NewObjZonas(this));
	//Ordena
	this->lisOBJ->Add(DLL_NewObjOrdena(this));
	}

//---------------------------------------------------------------------------
__fastcall TAplBalanceamento::~TAplBalanceamento(void)
   {
	//nada a fazer: classe básica libera memoria
   }
//---------------------------------------------------------------------------
