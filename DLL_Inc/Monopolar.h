//---------------------------------------------------------------------------
#ifndef MonopolarH
#define MonopolarH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void   __fastcall DLL_AtualizaFormMonopolar(TForm *form_owner, int ind_pat);
EXPORT void   __fastcall DLL_CloseFormMonopolar(TForm *form_owner);
EXPORT TForm* __fastcall DLL_NewFormMonopolar(TForm *form_owner, VTApl *apl_owner,
                                              TWinControl *parent, int ind_pat);

//---------------------------------------------------------------------------
#endif
//eof


 