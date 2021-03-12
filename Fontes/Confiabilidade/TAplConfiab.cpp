//---------------------------------------------------------------------------
#pragma hdrstop
#include "TAplConfiab.h"
#include "TConfiab.h"
#include "VTCfgGrafConfiab.h"
#include "VTDadoConfiab.h"
#include "GeraCorte\TGeraCorte.h"
#include "CalcIndices\VTCalcIndCont.h"
#include "..\Bloco\VTBlocos.h"
#include "..\Consulta\VTConsulta.h"
#include "..\GeraPath\VTGeraPath.h"
#include "..\GeraRede\VTGeraRede.h"
#include "..\..\DLL_Inc\Bloco.h"
#include "..\..\DLL_Inc\Consulta.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TAplConfiab::TAplConfiab(TForm *form_owner, VTApl *apl_owner)
	: TApl(form_owner, apl_owner)
	{
	//cria objetos para módulo de confiabilidade
	//via DLL
	this->Add(DLL_NewObjBlocos());
	this->Add(DLL_NewObjGeraPath(this));
	this->Add(DLL_NewObjGeraRede(this));
	//via local
	this->Add(NewObjDadoConfiab());
	this->Add(NewObjGeraCorte(this));
	this->Add(NewObjCalcIndice(this));
	}
//---------------------------------------------------------------------------
__fastcall TAplConfiab::~TAplConfiab(void)
	{
	//nada a fazer
	}
//---------------------------------------------------------------------------
