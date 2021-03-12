//---------------------------------------------------------------------------
#ifndef ArranjoH
#define ArranjoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTArranjo;
class VTArranjos;
class VTCabo;
class VTEqpto;
class VTPtoFixacao;
class VTSuporte;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTArranjo*    __fastcall DLL_NewObjArranjo(VTEqpto *owner=NULL);
EXPORT VTArranjos*   __fastcall DLL_NewObjArranjos(void);
EXPORT VTCabo*       __fastcall DLL_NewObjCabo(void);
EXPORT VTPtoFixacao* __fastcall DLL_NewObjPtoFixacao(void);
EXPORT VTSuporte*    __fastcall DLL_NewObjSuporte(void);

//---------------------------------------------------------------------------
#endif
//eof


