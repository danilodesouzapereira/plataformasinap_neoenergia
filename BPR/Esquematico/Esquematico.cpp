#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Esquematico\TFormEsquematico.h>
#include <Fontes\Esquematico\VTEsquematico.h>
#include <DLL_Inc\Esquematico.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Rede.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteForm(AnsiString form_name, TForm *form_owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormEsquematico(TForm *form_owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormSpy abertos
   while ((form = ExisteForm("TFormEsquematico", form_owner)) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormEsquematico(TForm *form_owner, VTApl *apl, TWinControl *parent)
   {
   //variáveis locais
   TFormEsquematico *FormEsquematico;

   //verifica se existe algum TFormSpy aberto
   if ((FormEsquematico = (TFormEsquematico*)(ExisteForm("TFormEsquematico", form_owner))) != NULL)
      {
      FormEsquematico->Show();
      return;
      }

   try{//cria um novo TFormSpy
      FormEsquematico = new TFormEsquematico(form_owner, apl, parent);
      FormEsquematico->Show();
      }catch(Exception &e)
         {//nada a fazer
         }
   return;
   }

//---------------------------------------------------------------------------
EXPORT VTEsquematico* __fastcall DLL_NewObjEsquematico(VTApl *apl)
   {
   return(NewObjEsquematico(apl));
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteForm(AnsiString form_name, TForm *form_owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormSpy aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormSpy
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC(form_name) == 0)
         {
         if (Screen->Forms[n]->Owner == form_owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

