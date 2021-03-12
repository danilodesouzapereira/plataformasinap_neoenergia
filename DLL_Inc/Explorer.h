//---------------------------------------------------------------------------
#ifndef ExplorerH
#define ExplorerH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTExplorer;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void        __fastcall DLL_AtualizaFormExplorer(TForm *form_owner);
EXPORT void        __fastcall DLL_CloseFormExplorer(TForm *form_owner);
EXPORT void        __fastcall DLL_NewFormExplorer(TForm *form_owner, VTApl *apl, TWinControl *parent);
EXPORT VTExplorer* __fastcall DLL_NewObjExplorer(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


 