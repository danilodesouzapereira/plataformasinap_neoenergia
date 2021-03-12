#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\MapaTematico\TFormMT.h>
#include <Fontes\MapaTematico\DensidadeCarga\TFormMT_DC.h>
#include <Fontes\MapaTematico\Celplan\TFormMT_Sinal.h>
#include <Fontes\MapaTematico\InterfaceCelplanner\TFormCelPlanner.h>
#include <Fontes\MapaTematico\InterfaceCelplanner\VTInterfaceCelPlanner.h>
#include <DLL_Inc\MapaTematico.h>
#pragma argsused

#pragma comment(lib, "Cartografia.a")
#pragma comment(lib, "Distancia.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Report.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteForm(TComponent *owner, AnsiString class_name_alvo);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_MapaTematico_Atualiza(TComponent *Owner)
   {
   //variáveis locais
   TForm *form;

   if ((form = ExisteForm(Owner, "TFormMT")) != NULL) ((TFormMT*)form)->Atualiza();
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_MapaTematico_Start(TComponent *Owner, VTApl *apl_owner)
   {
   try{//verifica se existe um TFormMapaTematico aberto
      if (ExisteForm(Owner, "TFormMT") == NULL)
         {///cria um novo TFormMT
         new TFormMT(Owner, apl_owner);
         }
	  }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_MapaTematico_Stop(TComponent *Owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os FormMT abertos
   while ((form = ExisteForm(Owner, "TFormMT")) != NULL) delete form;
   //destrói TFormMT_DC aberto
   while ((form = ExisteForm(Owner, "TFormMT_DC")) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormMT_DC(TComponent *Owner, VTApl *apl_owner,
                                          TWinControl *parent, TAction *ActionOnClose)
   {
   TForm *Form;

   //verifica se já existe um TFormMT_DC
   if ((Form = ExisteForm(Owner, "TFormMT_DC")) == NULL)
      {//cria um novo TFormMT_DC
      Form = new TFormMT_DC(Owner, apl_owner, parent, ActionOnClose);
      }
   Form->Show();
   return(Form);
   }
//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormImportCelPlan(TComponent *Owner, VTApl *apl_owner,
										  TWinControl *parent, TAction *ActionOnClose)
{
	TForm *Form;

   //verifica se já existe um TFormMT_DC
   if ((Form = ExisteForm(Owner, "TFormMT_Sinal")) == NULL)
	  {//cria um novo TFormMT_DC
	  Form = new TFormMT_Sinal(Owner, apl_owner, parent, ActionOnClose);
	  }
   Form->Show();
   return(Form);
}
//---------------------------------------------------------------------------
EXPORT VTInterfaceCelPlanner* __fastcall DLL_NewInterfaceCelPlanner(VTApl *apl_owner)
{
   return(NewObjInterfaceCelPlanner(apl_owner));
}
//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormExportCelPlan(TComponent *Owner, VTApl *apl_owner,
										  TWinControl *parent, TAction *ActionOnClose)
{
	TForm *Form;

   //verifica se já existe um TFormMT_DC
   if ((Form = ExisteForm(Owner, "TFormCelPlanner")) == NULL)
      {//cria um novo TFormMT_DC
      Form = new TFormCelPlanner(Owner, apl_owner, parent);
	  }
   Form->Show();
   return(Form);
}
//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteForm(TComponent *owner, AnsiString class_name_alvo)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormMapaTematico aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormMapaTematico
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC(class_name_alvo) == 0)
         {
         if (Screen->Forms[n]->Owner == owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

