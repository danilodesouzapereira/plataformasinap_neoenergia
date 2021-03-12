#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Cfg.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
   {
   return 1;
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ConfiguraArranjos(TForm *form_owner, VTApl *apl_owner, VTApl *apl_child)
   {
   return(false);
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ConfiguraCurvaDemanda(TForm *form_owner, VTApl *apl_owner, VTApl *apl_child)
   {
   return(false);
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ConfiguraFlow(TForm *form_owner, VTApl *apl_owner)
   {
   return(false);
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ConfiguraTipoChave(TForm *form_owner, VTApl *apl_owner)
   {
   return(false);
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ConfiguraTipoRede(TForm *form_owner, VTApl *apl_owner)
   {
   return(false);
   }

//---------------------------------------------------------------------------
//eof


