//---------------------------------------------------------------------------
#ifndef OrdenaH
#define OrdenaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTData;
class VTMonta;
class VTMontaCC;
class VTOrdena;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTData*    __fastcall DLL_NewObjData(VTApl *apl);
EXPORT VTMonta*   __fastcall DLL_NewObjMonta(VTApl *apl);
EXPORT VTMontaCC* __fastcall DLL_NewObjMontaCC(VTApl *apl);
EXPORT VTOrdena*  __fastcall DLL_NewObjOrdena(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


 