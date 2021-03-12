#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Protecao\TFormProtecao.h>
#include <DLL_Inc\Protecao.h>
#pragma argsused

#pragma comment(lib, "Busca.a")
#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Legenda.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Report.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormProtecao(TForm *form_owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormProtecao(TForm *form_owner)
   {
   //variáveis locais
   AnsiString class_name;
   TFormProtecao   *FormProtecao;

   //verifica se existe um FormProtecao aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormProtecao
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormProtecao") == 0)
         {
         if (Screen->Forms[n]->Owner == form_owner)
            {
            FormProtecao = (TFormProtecao*)Screen->Forms[n];
            FormProtecao->Atualiza();
            }
         }
      }
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormProtecao(TForm *form_owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormProtecao abertos
   while ((form = ExisteFormProtecao(form_owner)) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormProtecao(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TFormProtecao *FormProtecao;

   //verifica se existe algum TFormProtecao aberto
   //if (ExisteFormProtecao(form_owner)) return;
   try{//cria um novo TFormProtecao
      FormProtecao = new TFormProtecao(form_owner, apl_owner, parent);
      FormProtecao->Show();
      }catch(Exception &e)
         {//nada a fazer
         }
   return;
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormProtecao(TForm *form_owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormProtecao aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormProtecao
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormProtecao") == 0)
         {
         if (Screen->Forms[n]->Owner == form_owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

