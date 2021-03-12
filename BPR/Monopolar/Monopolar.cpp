#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Monopolar\TFormMonopolar.h>
#include <DLL_Inc\Monopolar.h>
#pragma argsused

#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Especial.a")
#pragma comment(lib, "Fasometro.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Medidor.a")
#pragma comment(lib, "MontaZ.a")
#pragma comment(lib, "Rede.a")

//---------------------------------------------------------------------------
//funcões usadas pela DLL
static TForm* __fastcall ExisteFormMonopolar(TForm *form_owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_AtualizaFormMonopolar(TForm *form_owner, int ind_pat)
   {
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormMonopolar(TForm *form_owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormMonopolar abertos
   while ((form = ExisteFormMonopolar(form_owner)) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormMonopolar(TForm *form_owner, VTApl *apl_owner,
                                              TWinControl *parent, int ind_pat)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe um TFormMonopolar aberto
   if ((form = ExisteFormMonopolar(form_owner)) == NULL)
      {//cria um novo TFormMonopolar e exibe como janela normal
      form = new TFormMonopolar(form_owner, apl_owner, parent, ind_pat);
      }
   //exibe  TFormMonopolar
   if (form != NULL) form->Show();
   return(form);
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormMonopolar(TForm *form_owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormMonopolar aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormMonopolar
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormMonopolar") == 0)
         {
         if (Screen->Forms[n]->Owner == form_owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

