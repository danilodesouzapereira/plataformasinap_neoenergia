#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Confiabilidade.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_Confiabilidade_Enabled(void)
   {
   return(false);
   }

//-----------------------------------------------------------------------------
EXPORT int __fastcall DLL_Confiabilidade_LimiteBarras(void)
   {
   return(0);
   }

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormConfiabilidade(TForm *form_owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormConfiabilidade(TForm *form_owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormConfiabilidade(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT TFormParam* __fastcall DLL_NewFormParamConfiabilidade(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
	{//variáveis locais
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTConfiab* __fastcall DLL_NewObjConfiab(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof


