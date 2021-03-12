//---------------------------------------------------------------------------
#ifndef MapaTematicoH
#define MapaTematicoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTInterfaceCelPlanner;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void   __fastcall DLL_MapaTematico_Atualiza(TComponent *Owner);
EXPORT void   __fastcall DLL_MapaTematico_Start(TComponent *Owner, VTApl *apl_owner);
EXPORT void   __fastcall DLL_MapaTematico_Stop(TComponent *Owner);
EXPORT TForm* __fastcall DLL_NewFormMT_DC(TComponent *Owner, VTApl *apl_owner,
										  TWinControl *parent, TAction *ActionOnClose);
EXPORT TForm* __fastcall DLL_NewFormImportCelPlan(TComponent *Owner, VTApl *apl_owner,
										  TWinControl *parent, TAction *ActionOnClose);
EXPORT TForm* __fastcall DLL_NewFormExportCelPlan(TComponent *Owner, VTApl *apl_owner,
										  TWinControl *parent, TAction *ActionOnClose);

EXPORT VTInterfaceCelPlanner* __fastcall DLL_NewInterfaceCelPlanner(VTApl *apl_owner);
//---------------------------------------------------------------------------
#endif
//eof


 