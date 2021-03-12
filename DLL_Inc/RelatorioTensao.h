//---------------------------------------------------------------------------
#ifndef RelatorioTensaoH
#define RelatorioTensaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved);
EXPORT void        __fastcall DLL_CloseFormRelatorioTensao(TComponent *Owner);
EXPORT TForm*      __fastcall DLL_ExisteFormRelatorioTensao(TComponent *Owner);
EXPORT TForm*      __fastcall DLL_NewFormRelatorioTensao(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);

//---------------------------------------------------------------------------
#endif
//eof



