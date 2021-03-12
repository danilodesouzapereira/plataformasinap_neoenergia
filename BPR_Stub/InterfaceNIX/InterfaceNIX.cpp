#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <InterfaceNIX.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormNIX(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTInterfaceNIX* __fastcall DLL_NewObjInterfaceNIX(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

