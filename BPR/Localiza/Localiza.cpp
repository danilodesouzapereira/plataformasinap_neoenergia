#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Localiza\TFormLocaliza.h>
#include <DLL_Inc\Localiza.h>
#pragma argsused

#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Legenda.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormLocaliza(TForm *form_owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormLocaliza(TForm *form_owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormLocaliza(TForm *form_owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormLocaliza abertos
   while ((form = ExisteFormLocaliza(form_owner)) != NULL) delete form;
   }


//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormLocaliza(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe um TFormLocaliza aberto
   if ((form = ExisteFormLocaliza(form_owner)) == NULL)
      {//cria um novo TFormLocaliza e exibe como janela normal
      form = new TFormLocaliza(form_owner, apl_owner, parent);
      }
   //exibe  TFormLocaliza
   if (form != NULL) form->Show();
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormLocaliza(TForm *form_owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormLocaliza aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormLocaliza
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormLocaliza") == 0)
         {
         if (Screen->Forms[n]->Owner == form_owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

