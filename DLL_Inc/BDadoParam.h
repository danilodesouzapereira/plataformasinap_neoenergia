//---------------------------------------------------------------------------
#ifndef BDadoParamH
#define BDadoParamH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
#include "Export.h"
//#define EXPORT extern "C" __declspec(dllexport)

//---------------------------------------------------------------------------
class VTApl;
class VTBDadoParam;
class VTParamConf;
class VTGeraLisParamEqpto;
//---------------------------------------------------------------------------
// funções da DLL
//---------------------------------------------------------------------------
EXPORT VTBDadoParam* __fastcall DLL_NewObjBDadoParam(VTApl *apl);
EXPORT VTParamConf*  __fastcall DLL_NewObjParamConf(void);
EXPORT VTGeraLisParamEqpto* __fastcall DLL_NewObjGeraLisParamEqpto(VTApl *apl);
//---------------------------------------------------------------------------
#endif
//eof

