//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TCurvaENERQCT.h"
#include "..\VTData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TCurvaENERQCT::TCurvaENERQCT(void)
   {
   }

//---------------------------------------------------------------------------
__fastcall TCurvaENERQCT::~TCurvaENERQCT(void)
   {
   }

//---------------------------------------------------------------------------
double __fastcall TCurvaENERQCT::AjustaTap(int    modelo,
		                                     double vf_pu,
                                           double var_unit,
                                           TBarra *bar_ref,
                                           TBarra *bar2,
                                           int    *passo_int)
{
// Variáveis locais
double tap_pu;

// Calcula tap em função do modelo de regulador ENERQCT
switch(modelo)
   {
	case regENERQCT1:
      if     (vf_pu < 0.8760) tap_pu = 1.1750;  // Tap 1:a
      else if(vf_pu < 0.9500) tap_pu = 1.0841;
      else if(vf_pu < 1.0300) tap_pu = 1.0000;
      else if(vf_pu < 1.1167) tap_pu = 0.9230;
      else                    tap_pu = 0.8510;
      if(bar_ref == bar2) tap_pu = 1. / tap_pu; // Tap a:1 = Tap 1:a'
      *passo_int = int(((tap_pu - 1.) / var_unit) + 0.0001);
      break;

   case regENERQCT2:
      if     (vf_pu < 0.870) tap_pu = 1.195;    // Tap 1:a
      else if(vf_pu < 0.910) tap_pu = 1.143;
      else if(vf_pu < 0.974) tap_pu = 1.067;
      else if(vf_pu < 1.040) tap_pu = 1.000;
      else                   tap_pu = 0.933;
      if(bar_ref == bar2) tap_pu = 1. / tap_pu; // Tap a:1 = Tap 1:a'
      *passo_int = int(((tap_pu - 1.) / var_unit) + 0.0001);
      break;

   case regENERQCT3:
      if     (vf_pu < 0.840) tap_pu = 1.210;    // Tap 1:a
      else if(vf_pu < 0.900) tap_pu = 1.133;
      else if(vf_pu < 0.958) tap_pu = 1.069;
      else if(vf_pu < 1.020) tap_pu = 1.000;
      else                   tap_pu = 0.931;
      if(bar_ref == bar2) tap_pu = 1. / tap_pu; // Tap a:1 = Tap 1:a'
      *passo_int = int(((tap_pu - 1.) / var_unit) + 0.0001);
      break;

	default:  // Erro - nada a fazer
		tap_pu     = 1.;
		*passo_int = 0;
		break;
	}

// Retorna
return(tap_pu);
}

//---------------------------------------------------------------------------

