//---------------------------------------------------------------------------
#ifndef RelatorioJusanteH
#define RelatorioJusanteH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//-----------------------------------------------------------------------------
class VTApl;
class VTRelatorioJusante;
class VTRelJusChaves;
//-----------------------------------------------------------------------------
 //EXPORT int __fastcall DLL_BatchProcess_Execute(AnsiString iniPath);
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved);
EXPORT void __fastcall DLL_CloseFormRelatorioJusante(TForm *Owner);
EXPORT TForm* __fastcall DLL_NewFormRelatorioJusante(TForm *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT VTRelatorioJusante* __fastcall DLL_NewRelatorioJusante(VTApl *apl_owner);
EXPORT VTRelJusChaves* __fastcall DLL_NewRelJusChaves(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof

 