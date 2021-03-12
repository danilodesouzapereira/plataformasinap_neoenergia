//---------------------------------------------------------------------------
#ifndef CartografiaH
#define CartografiaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTScg;
class VTCartografia;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void           __fastcall DLL_CloseFormGoogle(TComponent *Owner);
EXPORT TForm*         __fastcall DLL_NewFormGoogle(TComponent *Owner, VTApl *apl, TWinControl *parent);
EXPORT VTCartografia* __fastcall DLL_NewObjCartografia(VTApl *apl, TWinControl *parent, TAction *ActionMapaDisp);
EXPORT VTScg*         __fastcall DLL_NewObjScg(VTApl *apl, int empresa_id, AnsiString empresa_nome);

//---------------------------------------------------------------------------
#endif
//eof


 