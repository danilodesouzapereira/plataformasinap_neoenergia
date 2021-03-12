#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\AlocaCapacitor\TFormAlocaCapacitor.h>
#include <DLL_Inc\AlocaCapacitor.h>
#pragma argsused

#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Edita.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Report.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormAlocaCapacitor(TComponent *owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormAlocaCapacitor(TComponent *owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormAlocaCapacitor abertos
   while ((form = DLL_ExisteFormAlocaCapacitor(owner)) != NULL) delete form;
   }


//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_ExisteFormAlocaCapacitor(TComponent *owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormAlocaCapacitor aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormAlocaCapacitor
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormAlocaCapacitor") == 0)
         {
         if (Screen->Forms[n]->Owner == owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormAlocaCapacitor(TComponent *owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   try{//verifica se existe um TFormAlocaCapacitor aberto
      if ((form = DLL_ExisteFormAlocaCapacitor(owner)) == NULL)
         {//cria um novo TFormAlocaCapacitor e exibe como janela normal
         form = new TFormAlocaCapacitor(owner, apl_owner, parent);
         }
      //exibe  TFormAlocaCapacitor
      if (form != NULL) form->Show();
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(form);
   }

//---------------------------------------------------------------------------
//eof

