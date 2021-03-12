#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Perdatec.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_PerdaEnabled(void)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormPerdareg(TComponent *Owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void   __fastcall DLL_CloseFormPerdatec(TComponent *Owner)
   {
   }

   //-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_ExisteFormPerda(TComponent *Owner, AnsiString name)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCfgPerdatec(TComponent *Owner, VTApl *apl_owner)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormHistoricoPerdareg(TComponent *Owner, VTApl *apl_owner, TAction *ActionOnClose)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormHistoricoPerdatec(TComponent *Owner, VTApl *apl_owner, TAction *ActionOnClose)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormPerdareg(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormPerdatec(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTApl* __fastcall DLL_NewObjAplPerdareg(TForm *form_owner, VTApl *apl_owner)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTApl* __fastcall DLL_NewObjAplPerdatec(TForm *form_owner, VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjFluxo(VTApl *apl_owner)
   {
   return(NULL);
   }
//---------------------------------------------------------------------------
EXPORT VTResumo* __fastcall DLL_NewObjResumo(void)
   {
   return(NULL);
   }
//---------------------------------------------------------------------------
//eof

