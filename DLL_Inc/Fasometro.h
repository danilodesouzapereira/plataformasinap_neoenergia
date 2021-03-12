//---------------------------------------------------------------------------
#ifndef FasometroH
#define FasometroH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormFasometro(TForm *form_owner);
EXPORT void  __fastcall DLL_CloseFormFasometro(TForm *form_owner);
EXPORT void  __fastcall DLL_NewFormFasometro(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);

//---------------------------------------------------------------------------
#endif
//eof


 