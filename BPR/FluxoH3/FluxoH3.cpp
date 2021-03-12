#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Fluxo\VTFluxo.h>
#include <Fontes\FluxoH3\Main\TFormFluxoH3.h>
#include <DLL_Inc\FluxoH3.h>
#pragma argsused

#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "BD.a")
#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Esquematico.a")
#pragma comment(lib, "Flow.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Gauss3.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Legenda.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "RedeFH.a")
#pragma comment(lib, "SisLin.a")

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormFluxoH3(TForm *form_owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormFluxoH3(TForm *form_owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormFluxoH3 abertos
   while ((form = ExisteFormFluxoH3(form_owner)) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormFluxoH3(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe um TFormFluxoH3 aberto
   if ((form = ExisteFormFluxoH3(form_owner)) == NULL)
      {//cria um novo TFormFluxoH3 e exibe como janela normal
      form = new TFormFluxoH3(form_owner, apl_owner, parent);
      }
   //exibe  TFormFluxoH3
   if (form != NULL) form->Show();
   return(form);
   }

//---------------------------------------------------------------------------
EXPORT VTFluxo* __fastcall DLL_NewObjFluxoH3(VTApl *apl)
   {
   return(NewObjFluxoH3(apl));
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormFluxoH3(TForm *form_owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormFluxoH3 aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormFluxoH3
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormFluxoH3") == 0)
         {
         if (Screen->Forms[n]->Owner == form_owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

