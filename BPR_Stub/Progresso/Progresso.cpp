#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Progresso.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTProgresso* __fastcall DLL_NewObjProgresso(TComponent *Owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

