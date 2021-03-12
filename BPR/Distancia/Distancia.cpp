#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Distancia\TFormDistancia.h>
#include <Fontes\Distancia\VTCaminho.h>
#include <DLL_Inc\Distancia.h>
#pragma argsused

#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Report.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormDistancia(TComponent *owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormDistancia(TComponent *owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormDistancia(TComponent *owner)
   {
   //vari�veis locais
   TForm *form;

   //destr�i todos os TFormDistancia abertos
   while ((form = ExisteFormDistancia(owner)) != NULL) delete form;
   }


//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormDistancia(TComponent *owner, VTApl *apl_owner, TWinControl *parent)
   {
   //vari�veis locais
   TForm *form;

   //verifica se existe um TFormDistancia aberto
   if ((form = ExisteFormDistancia(owner)) == NULL)
      {//cria um novo TFormDistancia e exibe como janela normal
      form = new TFormDistancia(owner, apl_owner, parent);
      }
   //exibe  TFormDistancia
   if (form != NULL) form->Show();
   return(form);
   }

//---------------------------------------------------------------------------
EXPORT VTCaminho* __fastcall DLL_NewObjCaminho(VTApl *apl_owner)
   {
   return(NewObjCaminho(apl_owner));
   }

//---------------------------------------------------------------------------
//fun��es locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormDistancia(TComponent *owner)
   {
   //vari�veis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormDistancia aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n � um TFormDistancia
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormDistancia") == 0)
         {
         if (Screen->Forms[n]->Owner == owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

