#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Balanceamento\TFormBalanceamento.h>
#include <DLL_Inc\Balanceamento.h>
#pragma argsused

#pragma comment(lib, "Edita.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Report.a")
#pragma comment(lib, "Zona.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormBalanceamento(TComponent *owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormBalanceamento(TComponent *owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormBalanceamento abertos
   while ((form = DLL_ExisteFormBalanceamento(owner)) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_ExisteFormBalanceamento(TComponent *owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormBalanceamento aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormBalanceamento
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormBalanceamento") == 0)
         {
         if (Screen->Forms[n]->Owner == owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormBalanceamento(TComponent *owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe um TFormBalanceamento aberto
   if ((form = DLL_ExisteFormBalanceamento(owner)) == NULL)
      {//cria um novo TFormBalanceamento e exibe como janela normal
      form = new TFormBalanceamento(owner, apl_owner, parent);
      }
   //exibe  TFormBalanceamento
   if (form != NULL) form->Show();
   return(form);
   }

//---------------------------------------------------------------------------
//eof

