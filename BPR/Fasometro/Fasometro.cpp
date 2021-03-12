#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Fasometro\TFormFasometro.h>
#include <DLL_Inc\Fasometro.h>
#pragma argsused

#pragma comment(lib, "Busca.a")
#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Legenda.a")
#pragma comment(lib, "Rede.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormFasometro(TForm *form_owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormFasometro(TForm *form_owner)
   {
   //variáveis locais
   AnsiString class_name;
   TFormFasometro   *FormFasometro;

   //verifica se existe um FormFasometro aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormFasometro
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormFasometro") == 0)
         {
         if (Screen->Forms[n]->Owner == form_owner)
            {
            FormFasometro = (TFormFasometro*)Screen->Forms[n];
            FormFasometro->Atualiza();
            }
         }
      }
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormFasometro(TForm *form_owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormFasometro abertos
   while ((form = ExisteFormFasometro(form_owner)) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormFasometro(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TFormFasometro *FormFasometro;

   //verifica se existe algum TFormFasometro aberto
   //if (ExisteFormFasometro(form_owner)) return;
   try{//cria um novo TFormFasometro
      FormFasometro = new TFormFasometro(form_owner, apl_owner, parent);
      FormFasometro->Show();
      }catch(Exception &e)
         {//nada a fazer
         }
   return;
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormFasometro(TForm *form_owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormFasometro aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormFasometro
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormFasometro") == 0)
         {
         if (Screen->Forms[n]->Owner == form_owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

