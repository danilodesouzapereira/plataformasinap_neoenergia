#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Mercado\TFormMercado.h>
#include <Fontes\Mercado\VTCrescimento.h>
#include <Fontes\Mercado\VTImportaMercado.h>
#include <Fontes\Mercado\VTMercado.h>
#include <Fontes\Mercado\VTSegmento.h>
#include <DLL_Inc\Mercado.h>
#pragma argsused

#pragma comment(lib, "BD.a")
#pragma comment(lib, "Cronometro.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Mapa.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormMercado(TComponent *Owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormMercado(TComponent *Owner)
   {
   //variáveis locais
   TForm *form;

   try{//verifica se existe um TFormMercado aberto
      while ((form = ExisteFormMercado(Owner)) != NULL) delete form;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormMercado(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   try{//verifica se existe um TFormMercado aberto
      if ((form = ExisteFormMercado(Owner)) == NULL)
         {//cria um novo TFormMercado e exibe como janela normal
         form = new TFormMercado(Owner, apl_owner, parent);
         }
      //exibe  TFormMercado
      if (form != NULL) form->Show();
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(form);
   }

//---------------------------------------------------------------------------
EXPORT VTCrescimento* __fastcall DLL_NewObjCrescimento(void)
   {
   return(NewObjCrescimento());
   }

//---------------------------------------------------------------------------
EXPORT VTImportaMercado* __fastcall DLL_NewObjImportaMercado(VTApl *apl)
   {
   return(NewObjImportaMercado(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTMercado* __fastcall DLL_NewObjMercado(VTApl *apl)
   {
   return(NewObjMercado(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTSegmento* __fastcall DLL_NewObjSegmento(void)
   {
	return(NewObjSegmento());
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormMercado(TComponent *Owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormMercadoo aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormMercadoo
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormMercado") == 0)
         {
         if (Screen->Forms[n]->Owner == Owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

