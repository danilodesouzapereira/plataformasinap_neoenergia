//---------------------------------------------------------------------------
#ifndef LocalizaH
#define LocalizaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormLocaliza(TForm *form_owner);
EXPORT void  __fastcall DLL_CloseFormLocaliza(TForm *form_owner);
EXPORT void  __fastcall DLL_NewFormLocaliza(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);

//---------------------------------------------------------------------------
#endif
//eof


 