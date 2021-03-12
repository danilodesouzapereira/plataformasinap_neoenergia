//---------------------------------------------------------------------------
#ifndef SimuladorH
#define SimuladorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//-----------------------------------------------------------------------------
class VTApl;
class VTSinapClient;
class VTSinapServer;
class VTSinapUser;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT TForm*         __fastcall DLL_NewFormClient(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT TForm*         __fastcall DLL_NewFormServer(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTSinapClient* __fastcall DLL_NewObjSinapClient(VTApl *apl_owner);
EXPORT VTSinapServer* __fastcall DLL_NewObjSinapServer(VTApl *apl_owner);
EXPORT VTSinapUser*   __fastcall DLL_NewObjSinapUser(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof

 