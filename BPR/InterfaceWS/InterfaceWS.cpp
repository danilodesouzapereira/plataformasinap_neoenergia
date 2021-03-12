#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <..\..\Fontes\InterfaceWS\VTExportaEstudo.h>
#include <..\..\Fontes\InterfaceWS\VTImportaWS.h>
#include <..\..\DLL_Inc\InterfaceWS.h>
#pragma argsused

#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Registro.a")
#pragma comment(lib, "WebService.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTExportaEstudo* __fastcall DLL_NewObjExportaEstudo(VTApl *apl_owner)
	{
	try{
		return(NewObjExportaEstudo(apl_owner));
		}catch(Exception &e)
			{
			return(NULL);
			}
	}

//-----------------------------------------------------------------------------
EXPORT VTImportaWS* __fastcall DLL_NewObjImportaWS(VTApl *apl_owner)
{
    try
    {
        return(NewObjImportaWS(apl_owner));
    }
    catch(Exception &e)
    {
        return(NULL);
    }
}

//---------------------------------------------------------------------------
//eof

