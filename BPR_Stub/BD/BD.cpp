#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <BD.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTBD* __fastcall DLL_NewObjBD(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTSNPDBConn* __fastcall DLL_NewObjSNPDBConn(VTApl* apl, AnsiString strDBName, AnsiString strLoginName,
                                                   AnsiString strLoginPwd, int intDBType, bool bolMultiDataSet)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

