#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\Grafico\TFormPadrao.h>
#include <Fontes\Grafico\TFormZoom.h>
#include <Fontes\Grafico\VTQuad.h>
#include <DLL_Inc\Grafico.h>
#pragma argsused

#pragma comment(lib, "Cartografia.a")
#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Legenda.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "ResCC.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteForm(TComponent *Owner, AnsiString class_name);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormPadrao(TForm *form_owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormPadrao abertos
   while ((form = ExisteForm(form_owner, "TFormPadrao")) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormZoom(TForm *form_owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormZoom abertos
   while ((form = ExisteForm(form_owner, "TFormZoom")) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_NewFormPadrao(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe um FormPadrao aberto
   if ((form = ExisteForm(form_owner, "TFormPadrao")) == NULL)
      {//cria um FormPadrao e exibe como janela normal
      form = new TFormPadrao(form_owner, apl_owner, parent);
      }
   //exibe  FormPadrao
   if (form != NULL) form->Show();
   }

//---------------------------------------------------------------------------
EXPORT void  __fastcall DLL_NewFormZoom(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   //verifica se existe algum TFormZoom aberto
   if ((form = ExisteForm(form_owner, "TFormZoom")) != NULL)
      {
      form->Show();
      return;
      }
   try{//cria um  TFormZoom
      form = new TFormZoom(form_owner, apl_owner, parent);
      form->Show();
      }catch(Exception &e)
         {//nada a fazer
         }
   return;
   }

//---------------------------------------------------------------------------
EXPORT VTGrafico* __fastcall DLL_NewObjGrafico(VTApl *apl)
   {
   return(NewObjGrafico(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTGrafico* __fastcall DLL_NewObjGraficoAbre(VTApl *apl)
   {
   return(NewObjGraficoAbre(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTQuad* __fastcall DLL_NewObjQuad(void)
   {
   return(NewObjQuad());
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteForm(TComponent *Owner, AnsiString class_name)
   {
   //variáveis locais
   TForm      *form;

   //verifica se existe um Form aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormEditor
      if (class_name.AnsiCompareIC(Screen->Forms[n]->ClassName()) == 0)
         {
         if (Screen->Forms[n]->Owner == Owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

