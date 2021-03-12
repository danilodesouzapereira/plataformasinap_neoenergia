//---------------------------------------------------------------------------
#ifndef PerdatecH
#define PerdatecH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTFluxo;
class VTResumo;
//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT bool      __fastcall DLL_PerdaEnabled(void);
EXPORT void      __fastcall DLL_CloseFormPerdareg(TComponent *Owner);
EXPORT void      __fastcall DLL_CloseFormPerdatec(TComponent *Owner);
EXPORT TForm*    __fastcall DLL_ExisteFormPerda(TComponent *Owner, AnsiString name);
EXPORT TForm*    __fastcall DLL_NewFormCfgPerdatec(TComponent *Owner, VTApl *apl_owner);
EXPORT TForm*    __fastcall DLL_NewFormHistoricoPerdareg(TComponent *Owner, VTApl *apl_owner, TAction *ActionOnClose);
EXPORT TForm*    __fastcall DLL_NewFormHistoricoPerdatec(TComponent *Owner, VTApl *apl_owner, TAction *ActionOnClose);
EXPORT TForm*    __fastcall DLL_NewFormPerdareg(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT TForm*    __fastcall DLL_NewFormPerdatec(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTApl*    __fastcall DLL_NewObjAplPerdareg(TForm *form_owner, VTApl *apl_owner);
EXPORT VTApl*    __fastcall DLL_NewObjAplPerdatec(TForm *form_owner, VTApl *apl_owner);
EXPORT VTFluxo*  __fastcall DLL_NewObjFluxo(VTApl *apl_owner);
EXPORT VTResumo* __fastcall DLL_NewObjResumo(void);

//---------------------------------------------------------------------------
#endif
//eof

