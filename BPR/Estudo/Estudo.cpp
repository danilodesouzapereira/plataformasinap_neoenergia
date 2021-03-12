#include <vcl.h>
#include <windows.h>
#pragma hdrstop
//#include <Fontes\Estudo\TFormEstudo.h>
#include <Fontes\Estudo\TFormEstudoNovo.h>
#include <Fontes\Estudo\AnaTec\VTAnaTec.h>
#include <Fontes\Estudo\Relatorio\VTBaseAtivo.h>
#include <DLL_Inc\Estudo.h>
#pragma argsused

#pragma comment(lib, "BDihm.a")
#pragma comment(lib, "CalcIndCont.a")
#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Copia.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Mercado.a")
#pragma comment(lib, "MultiObra.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Redutor.a")
#pragma comment(lib, "Report.a")
#pragma comment(lib, "Stk.a")

//---------------------------------------------------------------------------
static TFormEstudoNovo* __fastcall ExisteFormEstudo(TComponent *Owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_AtualizaFormEstudo(TComponent *Owner, bool salvando)
   {
   //variáveis locais
   TFormEstudoNovo *FormEstudoNovo;

   if ((FormEstudoNovo = ExisteFormEstudo(Owner)) != NULL) FormEstudoNovo->AtualizaEstado(salvando);
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormEstudo(TComponent *Owner)
   {
   //variáveis locais
   TFormEstudoNovo *FormEstudoNovo;

   while ((FormEstudoNovo = ExisteFormEstudo(Owner)) != NULL) delete FormEstudoNovo;
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEstudo(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   return(new TFormEstudoNovo(Owner, apl_owner, parent));
   }

//-----------------------------------------------------------------------------
EXPORT VTAnaTec* __fastcall DLL_NewObjAnaTec(VTApl *apl_owner)
   {
   return(NewObjAnaTec(apl_owner));
   }
//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422293
EXPORT VTBaseAtivo* __fastcall DLL_NewObjBaseAtivo(void)
   {
   return(NewObjBaseAtivo());
   }
//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TFormEstudoNovo* __fastcall ExisteFormEstudo(TComponent *Owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormEstudo aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormEstudo
      class_name = Screen->Forms[n]->ClassName();
	  if (class_name.AnsiCompareIC("TFormEstudoNovo") == 0)
         {
         if (Screen->Forms[n]->Owner == Owner) return((TFormEstudoNovo*)(Screen->Forms[n]));
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

