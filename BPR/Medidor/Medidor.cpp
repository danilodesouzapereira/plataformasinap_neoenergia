#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Medidor\TFormMedidor.h>
#include <DLL_Inc\Medidor.h>
#pragma argsused

#pragma comment(lib, "Busca.a")
#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Legenda.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormMedidor(TForm *form_owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormMedidor(TForm *form_owner)
   {
   //variáveis locais
   AnsiString class_name;
   TFormMedidor   *FormMedidor;

   //verifica se existe um FormMedidor aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormMedidor
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormMedidor") == 0)
         {
         if (Screen->Forms[n]->Owner == form_owner)
            {
            FormMedidor = (TFormMedidor*)Screen->Forms[n];
            FormMedidor->Atualiza();
            }
         }
      }
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormMedidor(TForm *form_owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormMedidor abertos
   while ((form = ExisteFormMedidor(form_owner)) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormMedidor(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TFormMedidor *FormMedidor;

   //verifica se existe algum TFormMedidor aberto
   //if (ExisteFormMedidor(form_owner)) return;
   try{//cria um novo TFormMedidor
      FormMedidor = new TFormMedidor(form_owner, apl_owner, parent);
      FormMedidor->Show();
      }catch(Exception &e)
         {//nada a fazer
         }
   return;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_SelEqpto(TForm *form_owner, VTApl *apl_owner, TWinControl *parent, VTEqpto *eqpto)
	{
	//variáveis locais
	TFormMedidor *FormMedidor;

	try{//verifica se existe algum TFormMedidor aberto
		if ((FormMedidor = (TFormMedidor*)ExisteFormMedidor(form_owner)) == NULL)
			{//cria um novo TFormMedidor
			FormMedidor = new TFormMedidor(form_owner, apl_owner, parent);
			FormMedidor->Show();
         }
		//seleciona eqpto
//		FormMedidor->SelEqpto(eqpto);
        FormMedidor->DefineEqptoSelecionado(eqpto);
		}catch(Exception &e)
			{//nada a fazer
         }
   return;
	}

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormMedidor(TForm *form_owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormMedidor aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormMedidor
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormMedidor") == 0)
         {
         if (Screen->Forms[n]->Owner == form_owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

