//---------------------------------------------------------------------------
#ifndef EquivalenteH
#define EquivalenteH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void   __fastcall DLL_CloseFormEqv(TComponent *Owner);
EXPORT void   __fastcall DLL_CloseFormEqvWise(TComponent *Owner);
EXPORT TForm* __fastcall DLL_NewFormEqv(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT TForm* __fastcall DLL_NewFormEqvWise(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);

//---------------------------------------------------------------------------
#endif
//eof


 