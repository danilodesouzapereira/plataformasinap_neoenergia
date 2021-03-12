//---------------------------------------------------------------------------
#ifndef GestaoLicencaH
#define GestaoLicencaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTLicencaBO;
class TFormCFGProxy;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void   __fastcall DLL_CloseFormAlocaLicenca(TComponent *Owner);
EXPORT TForm* __fastcall DLL_NewFormAlocaLicenca(TComponent *Owner, VTApl *apl_owner);
EXPORT void   __fastcall DLL_CloseFormDadosLicenca(TComponent *Owner);
EXPORT TForm* __fastcall DLL_NewFormDadosLicenca(TComponent *Owner, VTApl *apl_owner);
EXPORT void   __fastcall DLL_CloseFormCFGProxy(TComponent *Owner);
EXPORT TFormCFGProxy* __fastcall DLL_NewFormCFGProxy(TComponent *Owner);
EXPORT VTLicencaBO*   __fastcall DLL_NewObjLicencaBO(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof


