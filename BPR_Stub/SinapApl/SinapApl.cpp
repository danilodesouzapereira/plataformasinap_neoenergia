#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <SinapApl.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTApl* __fastcall DLL_NewObjApl(TForm *FormOwner, VTApl *AplOwner)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTApl* __fastcall DLL_NewObjAplBatch(TForm *FormOwner, VTApl *AplOwner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTApl* __fastcall DLL_NewObjAplChild(TForm *FormOwner, VTApl *AplOwner, TAction *ActionInsRede)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTApl* __fastcall DLL_NewObjAplMdi(TForm *FormOwner, VTApl *AplOwner)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTApl* __fastcall DLL_NewObjAplRnt(TForm *FormOwner, VTApl *AplOwner)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
//eof


