#include <vcl.h>
#include <tchar.h>




extern "C" __declspec(dllexport) int __stdcall DLL_BatchProcess_Execute (AnsiString iniPath, AnsiString pid);
extern "C" __declspec(dllexport) int __stdcall DLL_BatchCalc_Execute (UnicodeString dllName, LPWSTR *args[]);
