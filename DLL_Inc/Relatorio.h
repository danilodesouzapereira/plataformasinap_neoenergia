//---------------------------------------------------------------------------
#ifndef RelatorioH
#define RelatorioH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_AtualizaRelatorio(TForm *Owner);
EXPORT void __fastcall DLL_CloseRelatorio(TForm *Owner);
EXPORT void __fastcall DLL_NewFormRelCarga(TForm *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT void __fastcall DLL_NewFormRelEqpto(TForm *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT void __fastcall DLL_NewFormRelFluxo(TForm *Owner, VTApl *apl_owner, TWinControl *parent);

//---------------------------------------------------------------------------
#endif
//eof


 