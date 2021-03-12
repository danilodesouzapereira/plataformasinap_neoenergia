//---------------------------------------------------------------------------
#ifndef ImportaH
#define ImportaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTExportaSec;
class VTIntegraEtd;
class VTMonta;
class VTParametros;
class VTRetiraEtd;
class VTValCabo;
class VTValEqpto;
class VTValFases;
class VTValMalhas;
class VTValPerdafe;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT TForm*         __fastcall DLL_NewFormValEqpto(TComponent *owner, VTApl *apl_owner);
EXPORT VTExportaSec*  __fastcall DLL_NewObjExportaSec(VTApl *apl_owner);
EXPORT VTIntegraEtd*  __fastcall DLL_NewObjIntegraEtd(VTApl *apl_owner);
EXPORT VTMonta*       __fastcall DLL_NewObjMonta(VTApl *apl_owner);
EXPORT VTMonta*       __fastcall DLL_NewObjMontaEstudo(VTApl *apl_owner);
EXPORT VTMonta*       __fastcall DLL_NewObjMontaRede(VTApl *apl_owner);
EXPORT VTParametros*  __fastcall DLL_NewObjParametros(VTApl *apl_owner);
EXPORT VTRetiraEtd*   __fastcall DLL_NewObjRetiraEtd(VTApl *apl_owner);
EXPORT VTValCabo*     __fastcall DLL_NewObjValCabo(VTApl *apl_owner);
EXPORT VTValEqpto*    __fastcall DLL_NewObjValEqpto(VTApl *apl_owner);
EXPORT VTValFases*    __fastcall DLL_NewObjValFases(VTApl *apl_owner);
EXPORT VTValFases*    __fastcall DLL_NewObjValFasesMalha(VTApl *apl_owner);
EXPORT VTValMalhas*   __fastcall DLL_NewObjValMalhas(VTApl *apl_owner);
EXPORT VTValPerdafe*  __fastcall DLL_NewObjValPerdafe(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 