//---------------------------------------------------------------------------
#ifndef BatchProcessH
#define BatchProcessH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
 //EXPORT int __fastcall DLL_BatchProcess_Execute(AnsiString iniPath);
extern "C" __declspec(dllexport) int __stdcall DLL_BatchProcess_Execute (AnsiString iniPath, AnsiString pid);
extern "C" __declspec(dllexport) int __stdcall DLL_BatchCalc_Execute (UnicodeString dllName,TStrings *args);
extern "C" __declspec(dllexport) int __stdcall DLL_Cabal_Execute(AnsiString jsonFilepath);
//---------------------------------------------------------------------------
#endif
//eof

