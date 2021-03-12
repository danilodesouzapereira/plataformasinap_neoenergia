#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\LocalDB\SQLite\VTSQLite.h>
#include <DLL_Inc\LocalDB.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTSQLiteDB* __fastcall DLL_NewObjSQLiteDB(void)
   {
   return(NewObjSQLiteDB());
   }
/*
//---------------------------------------------------------------------------
EXPORT VTSQLiteTable* __fastcall DLL_NewObjSQLiteTable(void)
   {
   return(NewObjSQLiteTable());
   }
*/
//---------------------------------------------------------------------------
//eof

