#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Editor.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormEditor(TForm *form_owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEditor(TForm *form_owner, VTApl *apl_owner,
                                           TWinControl *parent, bool planejamento)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTDragdrop* __fastcall DLL_NewObjDragdrop(void)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTEditor* __fastcall DLL_NewObjEditor(VTApl *apl)
	{
	return(NULL);
	}
//-----------------------------------------------------------------------------
EXPORT VTEdt* __fastcall DLL_NewObjEdtAltCargaNL(VTApl *AplOwner, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	{
	return(NULL);
	}
//-----------------------------------------------------------------------------
EXPORT VTEdt* __fastcall DLL_NewObjEdtArea(VTApl *AplOwner, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	{
	return(NULL);
	}

//-----------------------------------------------------------------------------
EXPORT VTEdt* __fastcall DLL_NewObjEdtBusca(VTApl *AplOwner, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	{
	return(NULL);
	}
//-----------------------------------------------------------------------------
EXPORT VTEdt* __fastcall DLL_NewObjEdtCargaNL(VTApl *AplOwner, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	{
	return(NULL);
	}
//-----------------------------------------------------------------------------
EXPORT VTEdt* __fastcall DLL_NewObjEdtCurto(VTApl *AplOwner, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	{
	return(NULL);
	}
//-----------------------------------------------------------------------------
EXPORT VTEdt* __fastcall DLL_NewObjEdtPoint(VTApl *AplOwner, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
//eof


