#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include "..\..\Dll_Inc\Compensacoes.h"
#include "..\..\Fontes\Compensacoes\TFormCompensacoes.h"
#include "..\..\Fontes\Compensacoes\VTCompensacoes.h"
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Radial.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteForm(TComponent *Owner, AnsiString form_name);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormCompensacoes(TComponent *Owner)
	{
	TForm *form;
	while ((form = ExisteForm(Owner, "TFormCompensacoes"))  != NULL) delete form;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCompensacoes(TComponent *Owner, VTApl *apl_owner, TWinControl *parent, AnsiString empresa)
	{
	TForm *form;
	try{
		if((form = ExisteForm(Owner, "TFormCompensacoes")) == NULL)
			{
			form = new TFormCompensacoes(Owner, apl_owner, parent, empresa);
			}
		}
	catch(Exception &e)
		{
		return(NULL);
		}
	return(form);
	}

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteForm(TComponent *Owner, AnsiString form_name)
	{
	AnsiString class_name;
	TForm      *form;
	for (int n = 0; n < Screen->FormCount; n++)
		{
		class_name = Screen->Forms[n]->ClassName();
		if (class_name.AnsiCompareIC(form_name) == 0)
			{
			if (Screen->Forms[n]->Owner == Owner) return(Screen->Forms[n]);
			}
		}
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTCompensacoes* __fastcall DLL_NewObjCompensacoes(TComponent *Owner, VTApl *apl_owner, AnsiString empresa)
	{
	VTCompensacoes *compensacoes;
	compensacoes = NewObjCompensacoes(Owner, apl_owner, empresa);
	return(compensacoes);
	}

//---------------------------------------------------------------------------
//eof

