//---------------------------------------------------------------------------
#ifndef BDihm_H
#define BDihm_H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTAbre;
class VTInsere;
class VTSalva;

//-----------------------------------------------------------------------------
EXPORT TForm*     __fastcall DLL_NewFormAbre(TComponent *Owner, VTApl *apl_owner);
EXPORT TForm*     __fastcall DLL_NewFormAbreEstudoRBT(TComponent *Owner, VTApl *apl_owner);
EXPORT TForm*     __fastcall DLL_NewFormAbreEstudoRMT(TComponent *Owner, VTApl *apl_owner);
EXPORT VTAbre*    __fastcall DLL_NewObjAbre(TComponent *Owner, VTApl *apl_owner);
EXPORT VTInsere*  __fastcall DLL_NewObjInsere(TComponent *Owner, VTApl *apl_owner, TAction *ActionInsRede);
EXPORT VTSalva*   __fastcall DLL_NewObjSalva(TComponent *Owner, VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof

 