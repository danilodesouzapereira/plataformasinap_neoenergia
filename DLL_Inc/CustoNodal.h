//---------------------------------------------------------------------------
#ifndef CustoNodalH
#define CustoNodalH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTMainCustoNodal;
class VTFluxo;
class VTCustos;
class VTCustoEqpto;
class VTCustoNodal;
class VTParcela;

//-----------------------------------------------------------------------------
// funções da DLL
//---------------------------------------------------------------------------
EXPORT VTMainCustoNodal* __fastcall DLL_NewObjMainCustoNodal(VTApl *apl);
EXPORT VTFluxo*          __fastcall DLL_NewObjCoefNodal(VTApl* apl);
EXPORT VTCustos*         __fastcall DLL_NewObjCustos(VTApl *apl);
EXPORT VTCustoEqpto*     __fastcall DLL_NewObjCustoEqpto(void);
EXPORT VTCustoNodal*     __fastcall DLL_NewObjCustoNodal(void);
EXPORT VTParcela*        __fastcall DLL_NewObjParcela(void);

//---------------------------------------------------------------------------
#endif
//eof


 