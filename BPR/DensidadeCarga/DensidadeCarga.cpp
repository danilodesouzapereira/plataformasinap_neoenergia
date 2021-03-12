#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\DensidadeCarga\TFormDensidade.h>
#include <Fontes\BD\VTBD.h>
#include <Fontes\BD\Hist\VTBDhist.h>
#include <DLL_Inc\DensidadeCarga.h>
#pragma argsused

#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "BD.a")
#pragma comment(lib, "Classes.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Grafico.a")
#pragma comment(lib, "Mapa.a")
#pragma comment(lib, "MapaTematico.a")
#pragma comment(lib, "Mercado.a")
#pragma comment(lib, "MontaZ.a")
#pragma comment(lib, "Potencia.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Redegraf.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormDensidade(TComponent *Owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormDensidadeCarga(TComponent *Owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormDensidade abertos
   while ((form = ExisteFormDensidade(Owner)) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormDensidadeCarga(TComponent *Owner, VTApl *apl_owner, TAction *ActionOnClose)
   {
   //variáveis locais
   TForm *form;

   //cria um novo TFormDensidade (que é do tipo Child)
   form = new TFormDensidade(Owner, apl_owner, ActionOnClose);
   return(form);
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormDensidade(TComponent *Owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormPertec aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um FormPertec
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormDensidade") == 0)
         {
         if (Screen->Forms[n]->Owner == Owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

