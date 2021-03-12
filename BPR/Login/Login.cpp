#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Login\TFormLogin.h>
#include <DLL_Inc\Login.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormLogin(TComponent *Owner, VTApl *apl)
   {
   try{//
      return(new TFormLogin(Owner, apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
//eof

