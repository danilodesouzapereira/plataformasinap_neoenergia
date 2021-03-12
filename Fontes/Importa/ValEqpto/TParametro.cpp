//---------------------------------------------------------------------------
#pragma hdrstop
#include "TParametro.h"
#include "VTParametro.h"
#include "VTParametros.h"
#include <Fontes\Apl\VTApl.h>
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTParametro* NewObjParametro(VTApl *apl)
	{
	return(new TParametro(apl));
	}
//---------------------------------------------------------------------------
__fastcall TParametro::TParametro(VTApl *apl)
	{
   //salva ponteiros p/ objetos
	this->apl = apl;
	//inicia zerado
	PD.tipo_eqpto = tipoINDEF;
	PD.atributo   = "";
	PD.valor      = 0;
	PD.opcoes     = new TStringList();
	}

//---------------------------------------------------------------------------
__fastcall TParametro::~TParametro(void)
	{
	//destroi stringlist
	PD.opcoes->Clear();
	if(PD.opcoes) {delete PD.opcoes; PD.opcoes = NULL;}
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TParametro::PM_Tipo(void)
	{
	switch(PD.tipo_eqpto)
		{
		case 	tipoARRANJO: 		return("Arranjo");
		case	tipoCABO:			return("Cabo");
		case	tipoCAPACITOR: 	return("Capacitor");
		case	tipoCARGA_A: 		return("Carga_A");
		case	tipoCARGA_B: 		return("Carga_B");
		case  tipoCHAVE: 			return("Chave");
		case  tipoGERADOR: 		return("Gerador");
		case  tipoREGULADOR: 	return("Regulador");
		case  tipoSUPORTE: 		return("Suporte");
		case  tipoSUPRIMENTO: 	return("Suprimento");
		case  tipoTRAFOSE: 		return("Trafo_SED");
		case  tipoTRAFOMT: 		return("Trafo_MT_BT");
		case  tipoTRECHOMT: 		return("Trecho_MT");
		case  tipoTRECHOBT: 		return("Trecho_BT");
		default:						return("");
      }
	}

//---------------------------------------------------------------------------
