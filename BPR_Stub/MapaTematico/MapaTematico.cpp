#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <MapaTematico.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_MapaTematico_Atualiza(TComponent *Owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_MapaTematico_Start(TComponent *Owner, VTApl *apl_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_MapaTematico_Stop(TComponent *Owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormMT_DC(TComponent *Owner, VTApl *apl_owner,
                                          TWinControl *parent, TAction *ActionOnClose)
   {
   return(NULL);
   }
//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormImportCelPlan(TComponent *Owner, VTApl *apl_owner,
										  TWinControl *parent, TAction *ActionOnClose)
{
	return(NULL);
}
//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormExportCelPlan(TComponent *Owner, VTApl *apl_owner,
										  TWinControl *parent, TAction *ActionOnClose)
{
	return(NULL);
}

//---------------------------------------------------------------------------
EXPORT VTInterfaceCelPlanner* __fastcall DLL_NewInterfaceCelPlanner(VTApl *apl_owner)
{
	return(NULL);
}
//---------------------------------------------------------------------------
//eof


