#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Editor\TFormEditor.h>
#include <Fontes\Editor\VTDragdrop.h>
#include <Fontes\Editor\VTEditor.h>
#include <Fontes\Editor\VTEdt.h>
#include <DLL_Inc\Editor.h>
#pragma argsused

#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "AutoTag.a")
#pragma comment(lib, "BDihm.a")
#pragma comment(lib, "Busca.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Figura.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Importa.a")
#pragma comment(lib, "ImportaCurva.a")
#pragma comment(lib, "InterfaceTXT.a")
#pragma comment(lib, "InterfaceXML.a")
#pragma comment(lib, "Legenda.a")
#pragma comment(lib, "MontaZ.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "Patamar.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Redutor.a")
#pragma comment(lib, "Topologia.a")
#pragma comment(lib, "Zona.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormEditor(TForm *form_owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormEditor(TForm *form_owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormEditor abertos
   if ((form = ExisteFormEditor(form_owner)) != NULL) delete form;
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEditor(TForm *form_owner, VTApl *apl_owner,
														 TWinControl *parent, bool planejamento)
	{
	//variáveis locais
	TForm *form;

	//verifica se existe um TFormEditor aberto
	if ((form = ExisteFormEditor(form_owner)) == NULL)
		{//cria um novo TFormEditor e exibe como janela normal
		form = new TFormEditor(form_owner, apl_owner, parent, planejamento);
		}
	//exibe  TFormEditor
	if (form != NULL) form->Show();
	//OBS JCG 2010.01.21 - na nova versão, NÃO exibe o FormEditor
	//if (form != NULL) form->Show();
	return(form);
	}

//-----------------------------------------------------------------------------
EXPORT VTDragdrop* __fastcall DLL_NewObjDragdrop(void)
   {
   return(NewObjDragdrop());
   }

//-----------------------------------------------------------------------------
EXPORT VTEditor* __fastcall DLL_NewObjEditor(VTApl *apl)
	{
	return(NewObjEditor(apl));
   }
//-----------------------------------------------------------------------------
EXPORT VTEdt* __fastcall DLL_NewObjEdtAltCargaNL(VTApl *AplOwner, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	{
	return(NewObjEdtAltCargaNL(AplOwner, ResetFocus, SetFocus, OnDone));
	}
//-----------------------------------------------------------------------------
EXPORT VTEdt* __fastcall DLL_NewObjEdtArea(VTApl *AplOwner, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	{
	return(NewObjEdtArea(AplOwner, ResetFocus, SetFocus, OnDone));
	}

//-----------------------------------------------------------------------------
EXPORT VTEdt* __fastcall DLL_NewObjEdtBusca(VTApl *AplOwner, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	{
	return(NewObjEdtBusca(AplOwner, ResetFocus, SetFocus, OnDone));
	}
//-----------------------------------------------------------------------------
EXPORT VTEdt* __fastcall DLL_NewObjEdtCargaNL(VTApl *AplOwner, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	{
	return(NewObjEdtCargaNL(AplOwner, ResetFocus, SetFocus, OnDone));
	}

//-----------------------------------------------------------------------------
EXPORT VTEdt* __fastcall DLL_NewObjEdtCurto(VTApl *AplOwner, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	{
	return(NewObjEdtCurto(AplOwner, ResetFocus, SetFocus, OnDone));
	}

//-----------------------------------------------------------------------------
EXPORT VTEdt* __fastcall DLL_NewObjEdtPoint(VTApl *AplOwner, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
	return(NewObjEdtPoint(AplOwner, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormEditor(TForm *form_owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormEditor aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormEditor
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormEditor") == 0)
         {
         if ((form_owner == NULL)||(Screen->Forms[n]->Owner == form_owner)) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

