#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <BDihm.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm*   __fastcall DLL_NewFormAbre(TComponent *Owner, VTApl *apl_owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormAbreEstudoRBT(TComponent *Owner, VTApl *apl_owner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormAbreEstudoRMT(TComponent *Owner, VTApl *apl_owner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTAbre*  __fastcall DLL_NewObjAbre(TComponent *Owner, VTApl *apl_owner)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTInsere*  __fastcall DLL_NewObjInsere(TComponent *Owner, VTApl *apl_owner, TAction *ActionInsRede)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTSalva* __fastcall DLL_NewObjSalva(TComponent *Owner, VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof



