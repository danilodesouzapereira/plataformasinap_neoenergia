#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Radial.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormMalha(TComponent *Owner)
   {
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormMalha(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTArvore*  __fastcall DLL_NewObjArvore(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTMalhas* __fastcall DLL_NewObjMalhas(VTApl *apl)
   {
   return(NULL);
   }
//---------------------------------------------------------------------------
EXPORT VTNode* __fastcall DLL_NewObjNode(void)
   {
   return(NULL);
   }
//-----------------------------------------------------------------------------
EXPORT VTRadial* __fastcall DLL_NewObjRadial(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTTronco* __fastcall DLL_NewObjTronco(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

