#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Produto\TFormLogo.h>
#include <Fontes\Produto\TFormSobre.h>
#include <Fontes\Produto\VTProduto.h>
#include <DLL_Inc\Produto.h>
#include <DLL_Inc\Funcao.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormLogo(TComponent *Owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *Form;

   try{//cria novo TFormLogo
      Form = new TFormLogo(Owner, parent);
      }catch(Exception &e)
         {
         Form = NULL;
         }
   return(Form);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormSobre(TComponent *Owner, VTApl *apl_owner)
   {
   //variáveis locais
   TForm *Form;

   try{//verifica se já existe o Form
      if ((Form = ExisteForm("TFormSobre", Owner)) == NULL)
         {//cria novo TFormSobre
         Form = new TFormSobre(Owner, apl_owner);
         }
      }catch(Exception &e)
         {
         Form = NULL;
         }
   return(Form);
   }

//---------------------------------------------------------------------------
EXPORT VTProduto* __fastcall DLL_NewObjProduto(void)
   {
   return(NewObjProduto());
   }


//---------------------------------------------------------------------------
//eof

