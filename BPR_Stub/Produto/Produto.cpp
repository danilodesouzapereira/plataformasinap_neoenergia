#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Produto.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormLogo(TComponent *Owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormSobre(TComponent *Owner, VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTProduto* __fastcall DLL_NewObjProduto(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

