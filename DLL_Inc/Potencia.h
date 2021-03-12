//---------------------------------------------------------------------------
#ifndef PotenciaH
#define PotenciaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTDemanda;
class VTFornecimento;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTDemanda*      __fastcall DLL_NewObjDemanda(VTApl *apl);
EXPORT VTFornecimento* __fastcall DLL_NewObjFornecimento(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


 