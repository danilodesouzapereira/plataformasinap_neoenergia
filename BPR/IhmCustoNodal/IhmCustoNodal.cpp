#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include "..\..\Fontes\IhmCustoNodal\TFormCustoNodal.h"
#include <DLL_Inc\IhmCustoNodal.h>
#pragma argsused

#pragma comment(lib, "CustoNodal.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "Zona.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormCustoNodal(TForm *form_owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_AtualizaFormCustoNodal(TForm *form_owner, int ind_pat)
   {
   //variáveis locais
   TForm *form;

   //atualiza todos os TFormCustoNodal abertos
   if ((form = ExisteFormCustoNodal(form_owner)) != NULL)
      {
      ((TFormCustoNodal*)form)->DefinePatamar(ind_pat);
      }
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormCustoNodal(TForm *form_owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormCustoNodal abertos
   while ((form = ExisteFormCustoNodal(form_owner)) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_IhmCustoNodal_Enabled(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCustoNodal(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe um TFormCustoNodal aberto
   if ((form = ExisteFormCustoNodal(form_owner)) == NULL)
      {//cria um novo TFormCustoNodal e exibe como janela normal
      form = new TFormCustoNodal(form_owner, apl_owner, parent);
      }
   //exibe  TFormCustoNodal
   if (form != NULL) form->Show();
   return(form);
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormCustoNodal(TForm *form_owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormCustoNodal aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormCustoNodal
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormCustoNodal") == 0)
         {
         if (Screen->Forms[n]->Owner == form_owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

