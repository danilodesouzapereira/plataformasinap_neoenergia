//---------------------------------------------------------------------------
#ifndef MedidorH
#define MedidorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormMedidor(TForm *form_owner);
EXPORT void  __fastcall DLL_CloseFormMedidor(TForm *form_owner);
EXPORT void  __fastcall DLL_NewFormMedidor(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);
EXPORT void  __fastcall DLL_SelEqpto(TForm *form_owner, VTApl *apl_owner, TWinControl *parent, VTEqpto *eqpto);

//---------------------------------------------------------------------------
#endif
//eof


 