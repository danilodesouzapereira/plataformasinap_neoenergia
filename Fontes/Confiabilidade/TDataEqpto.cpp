//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDataEqpto.h"
#include "..\Rede\VTEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTDataEqpto* __fastcall NewObjDataEqpto(VTEqpto* eqpto)
	{
	return(new TDataEqpto(eqpto));
	}
//---------------------------------------------------------------------------
__fastcall TDataEqpto::TDataEqpto(VTEqpto* eqpto)
	{
	this->eqpto = eqpto;
	itipo = -1;
	if(eqpto) itipo = eqpto->Tipo();

   eqpto_aux = NULL;

	//Inicia parametros
	pconf.fat_isola = 1.0;
	pconf.comp      = 1.0;
	//dataIni = NULL;
	}
//---------------------------------------------------------------------------
__fastcall TDataEqpto::~TDataEqpto(void)
	{
	}
//---------------------------------------------------------------------------
void __fastcall TDataEqpto::CopiaDados(VTDataEqpto* dataExt)
	{
	this->eqpto     = dataExt->eqpto;
	this->eqpto_aux = dataExt->eqpto_aux;//Classificacao auxiliar p.ex tipo de cabo
	this->itipo     = dataExt->itipo;    //tipo do equipamento
	this->itipo_aux = dataExt->itipo_aux;//tipo auxiliar de equipamento
	this->codigo    = dataExt->codigo;//Código do tipo
	this->pconf     = dataExt->pconf;
	this->vmin      = dataExt->vmin;
	this->vmax      = dataExt->vmax;
	}
//---------------------------------------------------------------------------

