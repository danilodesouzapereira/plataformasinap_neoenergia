#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\MapaTematico\TFormMT.h>
#include <Fontes\MapaTematico\DensidadeCarga\TFormMT_DC.h>
#include <DLL_Inc\MapaTematico.h>
#pragma argsused

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

