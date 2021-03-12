#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <DLL_Inc\BatchProcess.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
// EXPORT int __fastcall DLL_BatchProcess_Execute(AnsiString iniPath)
extern "C" __declspec(dllexport) int __stdcall DLL_BatchProcess_Execute (AnsiString iniPath, AnsiString pid)
   {
   return(0);
   }

//-----------------------------------------------------------------------------
// EXPORT int __fastcall DLL_BatchProcess_Execute(AnsiString iniPath)
extern "C" __declspec(dllexport) int __stdcall DLL_BatchCalc_Execute
							(UnicodeString dllName, TStrings *args)
   {
   return(0);
   }
//-----------------------------------------------------------------------------
// EXPORT int __fastcall DLL_BatchProcess_Execute(AnsiString iniPath)
extern "C" __declspec(dllexport) int __stdcall DLL_Cabal_Execute(AnsiString jsonFilepath)
   {
   return(0);
   }
//---------------------------------------------------------------------------
//eof


