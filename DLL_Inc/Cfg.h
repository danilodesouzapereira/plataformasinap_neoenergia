//---------------------------------------------------------------------------
#ifndef CfgH
#define CfgH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ConfiguraArranjos(TForm *form_owner, VTApl *apl_owner, VTApl *apl_child);
EXPORT bool __fastcall DLL_ConfiguraCurvaDemanda(TForm *form_owner, VTApl *apl_owner, VTApl *apl_child);
EXPORT bool __fastcall DLL_ConfiguraFlow(TForm *form_owner, VTApl *apl_owner);
EXPORT bool __fastcall DLL_ConfiguraTipoChave(TForm *form_owner, VTApl *apl_owner);
EXPORT bool __fastcall DLL_ConfiguraTipoRede(TForm *form_owner, VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


