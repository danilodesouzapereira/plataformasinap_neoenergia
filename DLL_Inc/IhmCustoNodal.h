//---------------------------------------------------------------------------
#ifndef IhmCustoNodalH
#define IhmCustoNodalH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void     __fastcall DLL_AtualizaFormCustoNodal(TForm *form_owner, int ind_pat);
EXPORT void     __fastcall DLL_CloseFormCustoNodal(TForm *form_owner);
EXPORT bool     __fastcall DLL_IhmCustoNodal_Enabled(void);
EXPORT TForm*   __fastcall DLL_NewFormCustoNodal(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);

//---------------------------------------------------------------------------
#endif
//eof


 