#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Reclamacao.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormReclamacao(TComponent *Owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormReclamacaoBT(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormReclamacaoMT(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
	}

//---------------------------------------------------------------------------
//eof

