//---------------------------------------------------------------------------
#ifndef ImportaCurvaH
#define ImportaCurvaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
class VTCurva;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ImportaCurva_Enabled(void);
EXPORT bool __fastcall DLL_ImportaCurva(TForm *form_owner, VTApl *apl_owner,
                                        VTCarga *carga, VTCurva *curva);
EXPORT bool __fastcall DLL_ImportaCurvas(TForm *form_owner, VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 