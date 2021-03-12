#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Curtocircuito\TFormCurto.h>
#include <Fontes\Curtocircuito\TFormCurtoSimultaneo.h>
#include <Fontes\Curtocircuito\VTCurtocircuito.h>
#include <DLL_Inc\CurtoCircuito.h>
#include <DLL_Inc\Funcao.h>
#pragma argsused

#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Curto.a")
#pragma comment(lib, "Distancia.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Flow.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Protecao.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "RedeCC.a")
#pragma comment(lib, "Report.a")
#pragma comment(lib, "ResCC.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_AtualizaFormCurto(TComponent *Owner)
   {
   //variáveis locais
    TForm *form;

   //atualiza TFormCurto aberto
   if ((form = ExisteForm("TFormCurto", Owner)) != NULL)
      {
      ((TFormCurto*)form)->AtualizaCalculo();
      }
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormCurto(TComponent *Owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormCurto abertos
   if ((form = ExisteForm("TFormCurto", Owner)) != NULL)
      {
      if ((form->Visible)&&(form->Enabled)) delete form;
      }
   //destrói todos os TFormCurtoSimultaneo abertos
   if ((form = ExisteForm("TFormCurtoSimultaneo", Owner)) != NULL)
      {
      if ((form->Visible)&&(form->Enabled)) delete form;
	  }
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_ExisteFormCurto(TComponent *Owner)
   {
   //verifica se existe um TFormCurto aberto
   return(ExisteForm("TFormCurto", Owner));
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCurto(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe um TFormCurto aberto
   if ((form = ExisteForm("TFormCurto", Owner)) == NULL)
      {//cria um novo TFormCurto e exibe como janela normal
      form = new TFormCurto(Owner, apl_owner, parent);
      }
   //exibe  TFormCurto
   if (form != NULL) form->Show();
   return(form);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCurtoSimultaneo(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe um TFormCurtoSimultaneo aberto
   if ((form = ExisteForm("TFormCurtoSimultaneo", Owner)) == NULL)
      {//cria um novo TFormCurtoSimultaneo e exibe como janela normal
      form = new TFormCurtoSimultaneo(Owner, apl_owner, parent);
      }
   //exibe  TFormCurto
   if (form != NULL) form->Show();
   return(form);
   }

//---------------------------------------------------------------------------
EXPORT VTCurtocircuito* __fastcall DLL_NewObjCurtocircuito(VTApl *apl_owner)
   {
   return(NewObjCurtocircuito(apl_owner));
   }

//---------------------------------------------------------------------------
//eof

