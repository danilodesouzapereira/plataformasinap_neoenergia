#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <AjusteDemanda.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_AjusteDemanda_Enabled(void)
   {
   return(false);
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormDemanda(TComponent *Owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_ExisteFormDemanda(TComponent *Owner)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCfgDemanda(TComponent *Owner, VTAjusta *ajuste_demanda)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT TForm*  __fastcall DLL_NewFormDemanda(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTAjusta* __fastcall DLL_NewObjAjustaPorFase(VTApl *apl_owner)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTAjusta* __fastcall DLL_NewObjAjustaTrifasico(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


