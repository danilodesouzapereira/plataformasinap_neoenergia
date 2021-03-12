//---------------------------------------------------------------------------
#ifndef EsquematicoH
#define EsquematicoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTEsquematico;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void           __fastcall DLL_CloseFormEsquematico(TForm *form_owner);
EXPORT void           __fastcall DLL_NewFormEsquematico(TForm *form_owner, VTApl *apl, TWinControl *parent);
EXPORT VTEsquematico* __fastcall DLL_NewObjEsquematico(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


 