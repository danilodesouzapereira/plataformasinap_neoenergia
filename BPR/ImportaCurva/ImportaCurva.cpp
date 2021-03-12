#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\ImportaCurva\TFormImportaCurva.h>
#include <Fontes\ImportaCurva\TFormImportaCurvas.h>
#include <DLL_Inc\ImportaCurva.h>
#pragma argsused

#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Funcao.a")


//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ImportaCurva_Enabled(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ImportaCurva(TForm *form_owner, VTApl *apl_owner,
                                        VTCarga *carga, VTCurva *curva)
   {
   //variáveis locais
   bool              curva_importada;
   TFormImportaCurva *FormImportaCurva;

   try{//cria FormImportaCurva como janela modal
      FormImportaCurva = new TFormImportaCurva(form_owner, apl_owner, carga, curva);
      curva_importada = (FormImportaCurva->ShowModal() == mrOk);
      //destrói o Form
      delete FormImportaCurva;
      }catch(Exception &e)
         {
         curva_importada = false;
         }
   return(curva_importada);
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_ImportaCurvas(TForm *form_owner, VTApl *apl_owner)
   {
   //variáveis locais
   bool               curva_importada;
/*
   TFormImportaCurvas *FormImportaCurvas;

   try{//cria FormImportaCurva como janela modal
      FormImportaCurvas = new TFormImportaCurvas(form_owner, apl_owner);
      curva_importada = (FormImportaCurvas->ShowModal() == mrOk);
      //destrói o Form
      delete FormImportaCurvas;
      }catch(Exception &e)
         {
         curva_importada = false;
         }
*/
   return(curva_importada);
   }

//---------------------------------------------------------------------------
//eof

