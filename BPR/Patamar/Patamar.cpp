#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Patamar\TFormPatamar.h>
#include <Fontes\Patamar\VTPatamar.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <DLL_Inc\Patamar.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ConfiguraPatamares(VTApl *apl)
   {
   //variáveis locais
   bool         alterado = false;
   TFormPatamar *FormPatamar;

   try{//cria TFormPatamar como janela modal
      FormPatamar = new TFormPatamar(NULL, apl);
      alterado = (FormPatamar->ShowModal() == mrOk);
      //destrói FormPatamar
      delete FormPatamar;
      }catch(Exception &e)
         {
         return(false);
         }
   return(alterado);
   }

//-----------------------------------------------------------------------------
EXPORT VTPatamar* __fastcall DLL_NewObjPatamar(int index, int hora, int minuto)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
EXPORT VTPatamares* __fastcall DLL_NewObjPatamares(VTApl *apl)
   {
   return(NewObjPatamares(apl));
   }

//---------------------------------------------------------------------------
//eof

