#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Equivalente\TFormEqv.h>
#include <Fontes\Equivalente\TFormEqvWise.h>
#include <DLL_Inc\Equivalente.h>
#include <DLL_Inc\Funcao.h>
#pragma argsused

#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Redutor.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
   {
   return 1;
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormEqv(TComponent *Owner)
   {
   //variáveis locais
   TForm *form;

   try{//destrói todos TFormEqv
      while ((form = ExisteForm("TFormEqv", Owner)) != NULL)
         {
         delete form;
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormEqvWise(TComponent *Owner)
   {
   //variáveis locais
   TForm *form;

   try{//destrói todos TFormEqvWise
      while ((form = ExisteForm("TFormEqvWise", Owner)) != NULL)
         {
         delete form;
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEqv(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   try{//verifica se existe o TFormEqv
      if ((form = ExisteForm("TFormEqv", Owner)) == NULL)
         {//cria TFormEqv
         form = new TFormEqv(Owner, apl_owner, parent);
         }
      }catch(Exception &e)
         {
         form = NULL;
         }
   return(form);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEqvWise(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   try{//verifica se existe o TFormEqvWise
      if ((form = ExisteForm("TFormEqvWise", Owner)) == NULL)
         {//cria TFormEqvWise
         form = new TFormEqvWise(Owner, apl_owner, parent);
         }
      }catch(Exception &e)
         {
         form = NULL;
         }
   return(form);
   }

//---------------------------------------------------------------------------
//eof

