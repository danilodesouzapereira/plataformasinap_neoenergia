#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\AlocaRegulador\TFormAlocaRegulador.h>
#include <DLL_Inc\AlocaRegulador.h>
#pragma argsused

#pragma comment(lib, "Distancia.a")
#pragma comment(lib, "Edita.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Medidor.a")
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
EXPORT void  __fastcall DLL_AtualizaFormAlocaRegulador(TComponent *owner)
	{

   }
//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormAlocaRegulador(TComponent *owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormAlocaRegulador abertos
	while ((form = DLL_ExisteFormAlocaRegulador(owner)) != NULL) delete form;
	}

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_ExisteFormAlocaRegulador(TComponent *owner)
	{
   //variáveis locais
   AnsiString class_name;
	TForm      *form;

   //verifica se existe um TFormAlocaRegulador aberto
   for (int n = 0; n < Screen->FormCount; n++)
		{//verifica se o Form n é um TFormAlocaRegulador
      class_name = Screen->Forms[n]->ClassName();
		if (class_name.AnsiCompareIC("TFormAlocaRegulador") == 0)
         {
         if (Screen->Forms[n]->Owner == owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormAlocaRegulador(TComponent *owner, VTApl *apl_owner, TWinControl *parent)
	{
   //variáveis locais
   TForm *form;

	try{//verifica se existe um form aberto
		if ((form = DLL_ExisteFormAlocaRegulador(owner)) == NULL)
			{//cria um novo form e exibe como janela normal
			form = new TFormAlocaRegulador(owner, apl_owner, parent);
         }
      //exibe  TFormAlocaRegulador
      if (form != NULL) form->Show();
      }catch(Exception &e)
         {
         return(NULL);
         }

   return(form);
	}

//---------------------------------------------------------------------------
EXPORT VTAlocaRegulador*  __fastcall DLL_NewObjAlocaRegulador(VTApl *apl_owner)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

