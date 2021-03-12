#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\ImportaAnarede\TFormAnarede.h>
#include <DLL_Inc\ImportaAnarede.h>
#pragma argsused

#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Esquematico.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Importa.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "MontaZ.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormImportaAnarede(TComponent *Owner, VTApl *apl)
   {
   try{
      return(new TFormAnarede(Owner, apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ImportaAnarede_Enabled(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
//eof

