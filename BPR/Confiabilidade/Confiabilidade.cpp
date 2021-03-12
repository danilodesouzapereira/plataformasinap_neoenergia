#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Confiabilidade\TFormConfiabilidade.h>
#include <Fontes\Confiabilidade\TFormParam.h>
#include <Fontes\Confiabilidade\VTConfiab.h>
#include <DLL_Inc\Confiabilidade.h>
#include <DLL_Inc\Funcao.h>
#pragma argsused

#pragma comment(lib, "Bloco.a")
#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Report.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_Confiabilidade_Enabled(void)
   {
   return(true);
   }

//-----------------------------------------------------------------------------
EXPORT int __fastcall DLL_Confiabilidade_LimiteBarras(void)
   {
   return(15000);
   }

//-----------------------------------------------------------------------------
EXPORT void  __fastcall DLL_AtualizaFormConfiabilidade(TForm *form_owner)
   {
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormConfiabilidade(TForm *form_owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormConfiabilidade abertos
   while ((form = ExisteForm("TFormConfiabilidade", form_owner)) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ModuloConfiabilidadePertencePlataformaSinap(void)
	{
	return(true);
	}

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormConfiabilidade(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   try{//verifica se existe um TFormConfiabilidade aberto
      if ((form = ExisteForm("TFormConfiabilidade", form_owner)) == NULL)
         {//cria um novo TFormConfiabilidade e exibe como janela normal
         form = new TFormConfiabilidade(form_owner, apl_owner, parent);
         }
      //exibe  TFormConfiabilidade
		if (form != NULL) form->Show();
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(form);
	}

//---------------------------------------------------------------------------
EXPORT TFormParam* __fastcall DLL_NewFormParamConfiabilidade(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
	{//variáveis locais
	TForm *form;

	try{//verifica se existe um TFormConfiabilidade aberto
		if ((form = ExisteForm("TFormParam", form_owner)) == NULL)
			{//cria um novo TFormConfiabilidade e exibe como janela normal
			form = new TFormParam(form_owner, parent, apl_owner);
			}
		//exibe  TFormParam
		//if (form != NULL) form->Show();
		}catch(Exception &e)
			{
			return(NULL);
			}
	return((TFormParam*)form);
	}

//-----------------------------------------------------------------------------
EXPORT VTConfiab* __fastcall DLL_NewObjConfiab(VTApl *apl_owner)
	{
	return(NewObjConfiab(apl_owner));
	}


//---------------------------------------------------------------------------
//eof

