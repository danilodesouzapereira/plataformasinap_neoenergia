#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Reclamacao\TFormReclamacaoBT.h>
#include <Fontes\Reclamacao\TFormReclamacaoMT.h>
#include <DLL_Inc\Reclamacao.h>
#pragma argsused

#pragma comment(lib, "BDihm.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Estudo.a")
#pragma comment(lib, "Flow.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Report.a")
#pragma comment(lib, "Zona.a")


//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormReclamacao(TComponent *Owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormReclamacao(TComponent *Owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormReclamacao abertos
   while ((form = ExisteFormReclamacao(Owner)) != NULL) delete form;
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormReclamacaoBT(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
	TForm *form;

   try{//verifica se existe um TFormReclamacao aberto
      if ((form = ExisteFormReclamacao(Owner)) == NULL)
         {//cria um novo TFormReclamacao e exibe como janela normal
			form = new TFormReclamacaoBT(Owner, apl_owner, parent);
			//DVK 2015.03.26 verifica se o form está ok
			if(! ((TFormReclamacaoBT*)form)->inicializado) {delete form; form = NULL;}
         }
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(form);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormReclamacaoMT(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
	//variáveis locais
	TForm *form;

   try{//verifica se existe um TFormReclamacao aberto
		if ((form = ExisteFormReclamacao(Owner)) == NULL)
         {//cria um novo TFormReclamacao e exibe como janela normal
			form = new TFormReclamacaoMT(Owner, apl_owner, parent);
			//DVK 2015.03.23 verifica se o form está ok
			if(! ((TFormReclamacaoMT*)form)->inicializado) {delete form; form = NULL;}
         }
      }catch(Exception &e)
			{
         return(NULL);
			}
   return(form);
	}

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormReclamacao(TComponent *Owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormReclamacao aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormReclamacao
		class_name = Screen->Forms[n]->ClassName();
//		if (class_name.AnsiCompareIC("TFormReclamacao") == 0)
		if ((class_name.AnsiCompareIC("TFormReclamacaoBT") == 0) ||
			 (class_name.AnsiCompareIC("TFormReclamacaoMT") == 0))
         {
         if (Screen->Forms[n]->Owner == Owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

