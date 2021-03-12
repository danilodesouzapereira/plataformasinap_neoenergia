#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Explorer\Forms\TFormExplorer.h>
#include <DLL_Inc\Explorer.h>
#pragma argsused

#pragma comment(lib, "Busca.a")
#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Legenda.a")
#pragma comment(lib, "Radial.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormExplorer(TForm *form_owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_AtualizaFormExplorer(TForm *form_owner)
   {
   //variáveis locais
   AnsiString    class_name;
   TFormExplorer *form;

   //verifica se existe um FormExplorer aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um FormExplorer
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormExplorer") == 0)
         {
         if (Screen->Forms[n]->Owner == form_owner)
            {
            form = (TFormExplorer*)Screen->Forms[n];
            form->Atualiza();
            }
         }
      }
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormExplorer(TForm *form_owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormEditor abertos
   while ((form = ExisteFormExplorer(form_owner)) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormExplorer(TForm *form_owner, VTApl *apl, TWinControl *parent)
   {
   //variáveis locais
   TFormExplorer *form;

   //cria um novo FormExplorer
   if ((form = new TFormExplorer(form_owner, apl, parent)) != NULL)
      {//exibe o form
      form->Show();
      }
   }

//---------------------------------------------------------------------------
EXPORT VTExplorer* __fastcall DLL_NewObjExplorer(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormExplorer(TForm *form_owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormExplorer aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um FormExplorer
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormExplorer") == 0)
         {
         if (Screen->Forms[n]->Owner == form_owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

