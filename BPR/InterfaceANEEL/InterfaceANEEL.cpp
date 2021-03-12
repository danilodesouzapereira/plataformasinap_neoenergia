#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\InterfaceANEEL\Exporta\TFormExportaANEEL.h>
#include <Fontes\InterfaceANEEL\Exporta\VTExportaANEEL.h>
#include <Fontes\InterfaceANEEL\Importa\TFormImportaANEEL.h>
#include <Fontes\InterfaceANEEL\Importa\VTImportaANEEL.h>
#include <DLL_Inc\InterfaceANEEL.h>
#pragma argsused

#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "Classes.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Esquematico.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Importa.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "MontaZ.a")
#pragma comment(lib, "Potencia.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormExportaANEEL(TComponent *Owner, VTApl *apl_owner, AnsiString filename)
   {
   try{
      return(new TFormExportaANEEL(Owner, apl_owner, filename));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormImportaANEEL(TComponent *Owner, VTApl *apl_owner)
   {
   try{
      return(new TFormImportaANEEL(Owner, apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//-----------------------------------------------------------------------------
EXPORT VTExportaANEEL* __fastcall DLL_NewObjExportaANEEL(VTApl *apl_owner)
	{
	try{
		return(NewObjExportaANEEL(apl_owner));
		}catch(Exception &e)
			{
			return(NULL);
			}
	}

//-----------------------------------------------------------------------------
EXPORT VTImportaANEEL* __fastcall DLL_NewObjImportaANEEL(VTApl *apl_owner)
	{
	try{
		return(NewObjImportaANEEL(apl_owner));
		}catch(Exception &e)
			{
			return(NULL);
			}
	}

//---------------------------------------------------------------------------
//eof

