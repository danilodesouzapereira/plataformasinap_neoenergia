//---------------------------------------------------------------------------
#ifndef EspecialH
#define EspecialH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_Especial_Atualiza(TComponent *Owner);
EXPORT void __fastcall DLL_Especial_Start(TComponent *Owner, VTApl *apl_owner);
EXPORT void __fastcall DLL_Especial_Stop(TComponent *Owner);

//---------------------------------------------------------------------------
#endif
//eof


 