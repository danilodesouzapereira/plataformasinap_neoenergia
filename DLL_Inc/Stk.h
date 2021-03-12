//---------------------------------------------------------------------------
#ifndef StkH
#define StkH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTStkCarga;
class VTStkEqv;
class VTStkPatamar;
class VTStkLicenca;
class VTStkRegulador;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTStkCarga*     __fastcall DLL_NewObjStkCarga(VTApl *apl_owner);
EXPORT VTStkEqv*       __fastcall DLL_NewObjStkEqv(VTApl *apl_owner);
EXPORT VTStkLicenca*   __fastcall DLL_NewObjStkLicenca(VTApl *apl_owner);
EXPORT VTStkPatamar*   __fastcall DLL_NewObjStkPatamar(VTApl *apl_owner);
EXPORT VTStkRegulador* __fastcall DLL_NewObjStkRegulador(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 