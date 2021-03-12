//---------------------------------------------------------------------------
#ifndef Geral_H
#define Geral_H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTGeral;
class VTInfoset;
class VTPadraoRede;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTGeral*      __fastcall DLL_NewObjGeral(VTApl *apl);
EXPORT VTInfoset*    __fastcall DLL_NewObjInfoset(VTApl *apl);
EXPORT VTPadraoRede* __fastcall DLL_NewObjPadraoRede(void);

#endif
//---------------------------------------------------------------------------
//eof

 