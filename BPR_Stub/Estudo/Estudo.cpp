#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Estudo.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_AtualizaFormEstudo(TComponent *Owner, bool salvando)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormEstudo(TComponent *Owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEstudo(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTAnaTec* __fastcall DLL_NewObjAnaTec(VTApl *apl_owner)
   {
   return(NULL);
   }
//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422293
EXPORT VTBaseAtivo* __fastcall DLL_NewObjBaseAtivo(void)
   {
   return(NULL);
   }
//---------------------------------------------------------------------------
//eof


