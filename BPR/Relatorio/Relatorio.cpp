#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Relatorio\TFormRelCarga.h>
#include <Fontes\Relatorio\TFormRelEqpto.h>
#include <Fontes\Relatorio\TFormRelFluxo.h>
#include <DLL_Inc\Relatorio.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Report.a")


//---------------------------------------------------------------------------
static TForm* __fastcall ExisteRelatorio(TForm *Owner, AnsiString form_name);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_AtualizaRelatorio(TForm *Owner)
	{
	//variáveis locais
	TForm *form;

	//atualiza todos os relatórios abertos
	if ((form = ExisteRelatorio(Owner, "TFormRelEqpto")) != NULL)
   	{
		((TFormRelEqpto*)form)->Atualiza();
      }
	if ((form = ExisteRelatorio(Owner, "TFormRelCarga")) != NULL)
   	{
		((TFormRelCarga*)form)->Atualiza();
      }
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseRelatorio(TForm *Owner)
	{
	//variáveis locais
	TForm *form;

	//destrói todos os relatórios abertos
	while ((form = ExisteRelatorio(Owner, "TFormRelCarga")) != NULL) delete form;
	while ((form = ExisteRelatorio(Owner, "TFormRelEqpto")) != NULL) delete form;
	while ((form = ExisteRelatorio(Owner, "TFormRelFluxo")) != NULL) delete form;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormRelCarga(TForm *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe algum TFormRelCarga aberto
   if ((form = ExisteRelatorio(Owner, "TFormRelCarga")) != NULL)
      {//exibe TFormRelCarga existente
      form->Show();
      return;
      }
   try{//cria um novo TFormRelCarga
      form = new TFormRelCarga(Owner, apl_owner, parent);
      form->Show();
      }catch(Exception &e)
         {//nada a fazer
         return;
         }
   return;
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormRelEqpto(TForm *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe algum TFormRelEqpto aberto
   if ((form = ExisteRelatorio(Owner, "TFormRelEqpto")) != NULL)
      {//exibe TFormRelEqpto existente
      form->Show();
      return;
      }
   try{//cria um novo TFormRelEqpto
      form = new TFormRelEqpto(Owner, apl_owner, parent);
      form->Show();
      }catch(Exception &e)
         {//nada a fazer
         return;
         }
   return;
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormRelFluxo(TForm *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe algum TFormRelEqpto aberto
   if ((form = ExisteRelatorio(Owner, "TFormRelFluxo")) != NULL)
      {//exibe TFormRelFluxo existente
      form->Show();
      return;
      }
   try{//cria um novo TFormRelFluxo
      form = new TFormRelFluxo(Owner, apl_owner, parent);
      form->Show();
      }catch(Exception &e)
         {//nada a fazer
         return;
         }
   return;
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteRelatorio(TForm *Owner, AnsiString form_name)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormRelatorio aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormRelatorio
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC(form_name) == 0)
         {
         if (Screen->Forms[n]->Owner == Owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

