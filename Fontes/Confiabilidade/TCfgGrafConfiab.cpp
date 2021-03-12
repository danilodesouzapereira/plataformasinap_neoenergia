//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCfgGrafConfiab.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//função global para criar objeto da classe
//---------------------------------------------------------------------------
VTCfgGrafConfiab *NewObjCfgGrafConfiab(void)
	{
   return (new TCfgGrafConfiab());
	}
//---------------------------------------------------------------------------
__fastcall TCfgGrafConfiab::TCfgGrafConfiab(void)
	{
	//inicia configuração
	index = eDEC;

	//dec
	dec.limite1 =  3.0;
	dec.limite2 = 10.0;

	//fec
	fec.limite1 = 2.5;
	fec.limite2 = 8.5;

	//end
	end.limite1 = 1.;
	end.limite2 = 10.;

	//pot
	pot.limite1 = 1.;
	pot.limite2 = 10.;
	}
//---------------------------------------------------------------------------
__fastcall TCfgGrafConfiab::~TCfgGrafConfiab(void)
	{
	//nada a fazer
	}
//---------------------------------------------------------------------------
TColor __fastcall TCfgGrafConfiab::Cor(double dec, double fec, double end, double pot)
	{
	switch(index)
   	{
			case eDEC: return CorDEC(dec);
			case eFEC: return CorFEC(fec);
			case eEND: return CorEND(end);
			case ePOT: return CorPOT(pot);
			default: break;
		}
	return clGray;  //default
	}
//---------------------------------------------------------------------------
TColor __fastcall TCfgGrafConfiab::CorDEC(double dec_value)
	{
	if(dec_value < dec.limite1) return  clGreen;
	if(dec_value > dec.limite2) return  clRed;
	return clYellow;
	}
//---------------------------------------------------------------------------
TColor __fastcall TCfgGrafConfiab::CorFEC(double fec_value)
	{
	if(fec_value < fec.limite1) return  clGreen; 
	if(fec_value > fec.limite2) return  clRed;
	return clYellow;

   }
//---------------------------------------------------------------------------
TColor __fastcall TCfgGrafConfiab::CorEND(double end_value)
	{
	if(end_value < end.limite1) return  clGreen; 
	if(end_value > end.limite2) return  clRed;
	return clYellow;
	}
//---------------------------------------------------------------------------
TColor __fastcall TCfgGrafConfiab::CorPOT(double pot_value)
	{
	if(pot_value < pot.limite1) return  clGreen; 
	if(pot_value > pot.limite2) return  clRed;
	return clYellow;
	}
//---------------------------------------------------------------------------


