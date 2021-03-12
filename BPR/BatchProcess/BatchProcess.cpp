// Important note about DLL memory management when your DLL uses the
// static version of the RunTime Library:
//
// If your DLL exports any functions that pass String objects (or structs/
// classes containing nested Strings) as parameter or function results,
// you will need to add the library MEMMGR.LIB to both the DLL project and
// any other projects that use the DLL.  You will also need to use MEMMGR.LIB
// if any other projects which use the DLL will be performing new or delete
// operations on any non-TObject-derived classes which are exported from the
// DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
// EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
// the file BORLNDMM.DLL should be deployed along with your DLL.
//
// To avoid using BORLNDMM.DLL, pass string information using "char *" or
// ShortString parameters.
//
// If your DLL uses the dynamic version of the RTL, you do not need to
// explicitly add MEMMGR.LIB as this will be done implicitly for you

#include <vcl.h>
#include <windows.h>
#include <inifiles.hpp>
#pragma hdrstop
#pragma argsused
// -----------------------------------------------------------------------------
// Importação das libs
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "Registro.a")
#pragma comment(lib, "SinapApl.a")
// -----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason,
    void* lpReserved) {

   return 1;

}

// -----------------------------------------------------------------------------
// EXPORT int __fastcall DLL_BatchProcess_Execute(AnsiString iniPath) {
extern "C" __declspec(dllexport) int __stdcall DLL_BatchProcess_Execute
	(AnsiString iniPath, AnsiString pid) {

   // retorno
   int retorno = 99; // erro desconhecido

   // carrega arquivo .ini
   TIniFile *ini = new TIniFile(iniPath);

   // carrega a DLL(que contem o metodo batchTask)
   UnicodeString dllName = ini->ReadString("TASK", "DLL", " ");

   // Dynamically loads the library
   HINSTANCE load = LoadLibrary(dllName.w_str()); // DLL

   // remove
   delete ini;

   if (load) {
	  typedef int(*IntReturnFunc)(AnsiString, AnsiString);

	  IntReturnFunc batchTaskExecute;

      // GetProcAddress returns a pointer to the loaded method(DLL_BatchTask_Execute)
	  batchTaskExecute = (IntReturnFunc)GetProcAddress(load,
		  "DLL_BatchTask_Execute");

      // Invoke DLL_BatchTask_Execute method
	  retorno = batchTaskExecute(iniPath, pid);
   }
   else {
      retorno = 1; // DLL nao encontrada
   }

   return (retorno);
}
// -----------------------------------------------------------------------------
// EXPORT int __fastcall DLL_BatchProcess_Execute(AnsiString iniPath) {
extern "C" __declspec(dllexport) int __stdcall DLL_BatchCalc_Execute
	(UnicodeString dllName, TStrings *args) {

   // retorno
   int retorno = 99; // erro desconhecido

   // carrega arquivo .ini
   //TIniFile *ini = new TIniFile(iniPath);

   // carrega a DLL(que contem o metodo batchTask)
   //UnicodeString dllName = dllName;

   // Dynamically loads the library
   HINSTANCE load = LoadLibrary(dllName.w_str()); // DLL

   // remove
   //delete ini;

   if (load) {
	  typedef int(*IntReturnFunc)(TStrings *args);

	  IntReturnFunc batchCalc_Execute;

      // GetProcAddress returns a pointer to the loaded method(DLL_BatchCalc_Execute)
	  batchCalc_Execute = (IntReturnFunc)GetProcAddress(load,
		  "DLL_BatchCalc_Execute");

      // Invoke DLL_BatchTask_Execute method
	  retorno = batchCalc_Execute(args);
   }
   else {
      retorno = 1; // DLL nao encontrada
   }

   return (retorno);
}
// -----------------------------------------------------------------------------
// EXPORT int __fastcall DLL_BatchProcess_Execute(AnsiString iniPath) {
extern "C" __declspec(dllexport) int __stdcall DLL_Cabal_Execute
	(AnsiString jsonFilepath) {

   // retorno
   int retorno = 99; // erro desconhecido



   // carrega a DLL(que contem o metodo batchTask)
   UnicodeString dllName = "BatchCabal.dll";

   // Dynamically loads the library
   HINSTANCE load = LoadLibrary(dllName.w_str()); // DLL


   if (load) {
	  typedef int(*IntReturnFunc)(AnsiString);

	  IntReturnFunc batchTaskExecute;

      // GetProcAddress returns a pointer to the loaded method(DLL_BatchTask_Execute)
	  batchTaskExecute = (IntReturnFunc)GetProcAddress(load,
		  "DLL_BatchTask_Execute");

      // Invoke DLL_BatchTask_Execute method
	  retorno = batchTaskExecute(jsonFilepath);
   }
   else {
      retorno = 1; // DLL nao encontrada
   }

   return (retorno);
}
// -----------------------------------------------------------------------------
