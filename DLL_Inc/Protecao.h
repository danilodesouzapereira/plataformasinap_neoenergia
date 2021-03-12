//---------------------------------------------------------------------------
#ifndef ProtecaoH
#define ProtecaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormProtecao(TForm *form_owner);
EXPORT void  __fastcall DLL_CloseFormProtecao(TForm *form_owner);
EXPORT void  __fastcall DLL_NewFormProtecao(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);

//---------------------------------------------------------------------------
#endif
//eof


 