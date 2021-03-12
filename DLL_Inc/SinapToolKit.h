//---------------------------------------------------------------------------
#ifndef SinapToolKitH
#define SinapToolKitH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
EXPORT bool       __fastcall DLL_AtualizaTelas(TComponent *Owner);
EXPORT int        __fastcall DLL_Empresa_ID(void);
EXPORT AnsiString __fastcall DLL_Empresa_Codigo(void);
EXPORT bool       __fastcall DLL_FormChildStart(TComponent *Owner, VTApl *apl_owner, int stk_option);
EXPORT bool       __fastcall DLL_FormChildStop(TComponent *Owner);
EXPORT bool       __fastcall DLL_FormMdiStart(TComponent *Owner, VTApl *apl_owner);
EXPORT bool       __fastcall DLL_FormMdiStop(TComponent *Owner);

//---------------------------------------------------------------------------
#endif
//eof


 