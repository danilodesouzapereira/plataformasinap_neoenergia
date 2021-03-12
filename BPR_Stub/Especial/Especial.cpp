#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Especial.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_Especial_Atualiza(TComponent *Owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_Especial_Start(TComponent *Owner, VTApl *apl_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_Especial_Stop(TComponent *Owner)
   {
   }

//---------------------------------------------------------------------------
//eof

