//---------------------------------------------------------------------------
#ifndef DistanciaH
#define DistanciaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTCaminho;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void       __fastcall DLL_AtualizaFormDistancia(TComponent *owner);
EXPORT void       __fastcall DLL_CloseFormDistancia(TComponent *owner);
EXPORT TForm*     __fastcall DLL_NewFormDistancia(TComponent *owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTCaminho* __fastcall DLL_NewObjCaminho(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 