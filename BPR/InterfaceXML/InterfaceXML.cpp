#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\InterfaceXML\TFormExportaXML.h>
#include <Fontes\InterfaceXML\TFormImportaXML.h>
#include <Fontes\InterfaceXML\VTExportaXML.h>
#include <Fontes\InterfaceXML\VTImportaXML.h>
#include <DLL_Inc\InterfaceXML.h>
#pragma argsused

#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Esquematico.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Importa.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "MontaZ.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Registro.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ExportaXML_Enabled(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ImportaXML_Enabled(void)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormExportaXML(TComponent *Owner, VTApl *apl_owner, AnsiString arq_xml)
   {
   try{
      return(new TFormExportaXML(Owner, apl_owner, arq_xml));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormImportaXML(TComponent *Owner, VTApl *apl_owner, AnsiString arq_xml)
   {
   try{
      return(new TFormImportaXML(Owner, apl_owner, arq_xml));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//-----------------------------------------------------------------------------
EXPORT VTExportaXML* __fastcall DLL_NewObjExportaXML(VTApl *apl_owner)
	{
	try{
		return(NewObjExportaXML(apl_owner));
		}catch(Exception &e)
			{
			return(NULL);
			}
	}

//-----------------------------------------------------------------------------
EXPORT VTImportaXML* __fastcall DLL_NewObjImportaXML(VTApl *apl_owner)
	{
	try{
		return(NewObjImportaXML(apl_owner));
		}catch(Exception &e)
			{
			return(NULL);
			}
	}

//---------------------------------------------------------------------------
//eof

