//---------------------------------------------------------------------------
#ifndef DensidadeCargaH
#define DensidadeCargaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTBDh;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void   __fastcall DLL_CloseFormDensidadeCarga(TComponent *Owner);
EXPORT TForm* __fastcall DLL_NewFormDensidadeCarga(TComponent *Owner, VTApl *apl_owner, TAction *ActionOnClose);

//---------------------------------------------------------------------------
#endif
//eof

