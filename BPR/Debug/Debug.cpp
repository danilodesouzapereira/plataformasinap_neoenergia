#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Debug\TFormDebug.h>
#include <DLL_Inc\Debug.h>
#pragma argsused

#pragma comment(lib, "Bloco.a")
#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Rede.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormDebug(TForm *form_owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormDebug(TForm *form_owner)
   {
   //variáveis locais
	TForm *form;

   //destrói todos os TFormEditor abertos
	while ((form = ExisteFormDebug(form_owner)) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_Debug_Enabled(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormDebug(TForm *form_owner, VTApl *apl, TWinControl *parent)
   {
   //variáveis locais
   TFormDebug *form;

   //cria um novo FormDebug
   if ((form = new TFormDebug(form_owner, apl, parent)) != NULL)
      {//exibe o form
      form->Show();
      }
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormDebug(TForm *form_owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormDebug aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um FormDebug
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormDebug") == 0)
         {
         if (Screen->Forms[n]->Owner == form_owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

