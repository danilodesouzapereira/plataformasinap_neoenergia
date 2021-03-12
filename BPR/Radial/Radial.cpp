#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Radial\TFormMalha.h>
#include <Fontes\Radial\VTArvore.h>
#include <Fontes\Radial\VTMalhas.h>
#include <Fontes\Radial\VTNode.h>
#include <Fontes\Radial\VTRadial.h>
#include <Fontes\Radial\VTTronco.h>
#include <DLL_Inc\Radial.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Topologia.a")

//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormMalha(TComponent *Owner);

//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
   {
   return 1;
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormMalha(TComponent *Owner)
   {
   //variáveis locais
   TForm *form;

   try{//verifica se existe um TFormMalha aberto
      while ((form = ExisteFormMalha(Owner)) != NULL) delete form;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormMalha(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
	{
   //variáveis locais
   TForm *form;

   try{//verifica se existe um TFormMalha aberto
      if ((form = ExisteFormMalha(Owner)) == NULL)
         {//cria um novo TFormMalha e exibe como janela normal
         form = new TFormMalha(Owner, apl_owner, parent);
         }
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(form);
   }

//---------------------------------------------------------------------------
EXPORT VTArvore*  __fastcall DLL_NewObjArvore(VTApl *apl)
   {
   return(NewObjArvore(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTMalhas* __fastcall DLL_NewObjMalhas(VTApl *apl)
   {
   return(NewObjMalhas(apl));
   }
//---------------------------------------------------------------------------
EXPORT VTNode* __fastcall DLL_NewObjNode(void)
   {
   return(NewObjNode());
   }

//---------------------------------------------------------------------------
EXPORT VTRadial* __fastcall DLL_NewObjRadial(VTApl *apl)
   {
   return(NewObjRadial(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTTronco*  __fastcall DLL_NewObjTronco(VTApl *apl)
   {
   return(NewObjTronco(apl));
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TForm* __fastcall ExisteFormMalha(TComponent *Owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um TFormMalha aberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um TFormMercadoo
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormMalha") == 0)
         {
         if (Screen->Forms[n]->Owner == Owner) return(Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof



