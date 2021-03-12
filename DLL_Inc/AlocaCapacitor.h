//---------------------------------------------------------------------------
#ifndef AlocaCapacitorH
#define AlocaCapacitorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTAlocaCapacitor;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void              __fastcall DLL_AtualizaFormAlocaCapacitor(TComponent *owner);
EXPORT void              __fastcall DLL_CloseFormAlocaCapacitor(TComponent *owner);
EXPORT TForm*            __fastcall DLL_ExisteFormAlocaCapacitor(TComponent *owner);
EXPORT TForm*            __fastcall DLL_NewFormAlocaCapacitor(TComponent *owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTAlocaCapacitor* __fastcall DLL_NewObjAlocaCapacitor(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof



