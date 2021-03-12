#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <LocalDB.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTSQLiteDB* __fastcall DLL_NewObjSQLiteDB(void)
   {
   return(NULL);
   }
 /*
//---------------------------------------------------------------------------
EXPORT VTSQLiteTable* __fastcall DLL_NewObjSQLiteTable(void)
   {
   return(NULL);
   }
  */
//---------------------------------------------------------------------------
//eof

