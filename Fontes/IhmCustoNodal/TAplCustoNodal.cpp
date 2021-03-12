//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAplCustoNodal.h"
#include "..\CustoNodal\VTMainCustoNodal.h"
#include "..\CustoNodal\VTCustos.h"
#include "..\Fluxo\VTFluxo.h"
#include "..\Ordena\VTOrdena.h"
#include "..\Zona\VTZonas.h"
#include "..\..\DLL_Inc\CustoNodal.h"
#include "..\..\DLL_Inc\Legenda.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\Zona.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAplCustoNodal::TAplCustoNodal(TForm *form_owner, VTApl *apl_owner)
                          :TApl(form_owner, apl_owner)
   {
   //cria objetos necessários ao Editor
  Add(DLL_NewObjOrdena(this));
  Add(DLL_NewObjZonas(this));
  Add(DLL_NewObjMainCustoNodal(this));
  Add(DLL_NewObjCustos(this));
  Add(DLL_NewObjCoefNodal(this));
  }

//---------------------------------------------------------------------------
__fastcall TAplCustoNodal::~TAplCustoNodal(void)
   {
   //OBS: a classe base destrói a lista e seus objetos
   }

//---------------------------------------------------------------------------
//eof