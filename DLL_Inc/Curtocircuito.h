//---------------------------------------------------------------------------
#ifndef CurtocircuitoH
#define CurtocircuitoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTCurtocircuito;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void             __fastcall DLL_AtualizaFormCurto(TComponent *Owner);
EXPORT void             __fastcall DLL_CloseFormCurto(TComponent *Owner);
EXPORT TForm*           __fastcall DLL_ExisteFormCurto(TComponent *Owner);
EXPORT TForm*           __fastcall DLL_NewFormCurto(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT TForm*           __fastcall DLL_NewFormCurtoSimultaneo(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTCurtocircuito* __fastcall DLL_NewObjCurtocircuito(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 