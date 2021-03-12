#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Legenda.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTLegenda* __fastcall DLL_NewObjLegenda(VTApl *apl)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTVetval*  __fastcall DLL_NewObjVetval(int ind_atrib, AnsiString descricao, int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

