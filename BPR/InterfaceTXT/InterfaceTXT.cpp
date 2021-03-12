#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\InterfaceTXT\Exporta\TFormExportaTXT.h>
#include <Fontes\InterfaceTXT\Exporta\VTExportaTXT.h>
#include <Fontes\InterfaceTXT\Importa\TFormImportaTXT.h>
#include <Fontes\InterfaceTXT\Importa\VTImportaTXT.h>
#include <DLL_Inc\InterfaceTXT.h>
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
EXPORT TForm* __fastcall DLL_NewFormExportaTXT(TComponent *Owner, VTApl *apl_owner, AnsiString arq_txt)
   {
   try{
      return(new TFormExportaTXT(Owner, apl_owner, arq_txt));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormImportaTXT(TComponent *Owner, VTApl *apl_owner)
   {
   try{
      return(new TFormImportaTXT(Owner, apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//-----------------------------------------------------------------------------
EXPORT VTExportaTXT* __fastcall DLL_NewObjExportaTXT(VTApl *apl_owner)
	{
	try{
		return(NewObjExportaTXT(apl_owner));
		}catch(Exception &e)
			{
			return(NULL);
			}
	}

//-----------------------------------------------------------------------------
EXPORT VTImportaTXT* __fastcall DLL_NewObjImportaTXT(VTApl *apl_owner)
	{
	try{
		return(NewObjImportaTXT(apl_owner));
		}catch(Exception &e)
			{
			return(NULL);
			}
	}

//---------------------------------------------------------------------------
//eof

