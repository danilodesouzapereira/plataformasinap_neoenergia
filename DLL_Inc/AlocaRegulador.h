//---------------------------------------------------------------------------
#ifndef AlocaReguladorH
#define AlocaReguladorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTAlocaRegulador;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void              __fastcall DLL_AtualizaFormAlocaRegulador(TComponent *owner);
EXPORT void              __fastcall DLL_CloseFormAlocaRegulador(TComponent *owner);
EXPORT TForm*            __fastcall DLL_ExisteFormAlocaRegulador(TComponent *owner);
EXPORT TForm*            __fastcall DLL_NewFormAlocaRegulador(TComponent *owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTAlocaRegulador* __fastcall DLL_NewObjAlocaRegulador(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof

