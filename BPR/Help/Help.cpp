#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Help\TFormHelp.h>
#include <Fontes\Help\TFormSobreSinapsis.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Help.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void   __fastcall DLL_CloseFormHelp(TComponent *Owner)
   {
   //variáveis locais
   TForm *form;

   while ((form = ExisteForm("TFormHelp", NULL)) != NULL)
      {
      delete form;
      }
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormHelp(TComponent *Owner, VTApl *apl_owner, AnsiString topico)
	{
   //variáveis locais
   TForm *form;

   try{//verifica se já existe o FormHelp
      if ((form = ExisteForm("TFormHelp", Owner)) == NULL)
         {//não define owner do FormHelp
         form = new TFormHelp(Owner, apl_owner, topico);
         }
      }catch(Exception &e)
         {
         form = NULL;
         }
	return(form);
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormSobreSinapsis(TComponent *Owner, VTApl *apl_owner)
	{
   //variáveis locais
   TForm *form;

   try{//cria TFormSobreSinapsis
      form = new TFormSobreSinapsis(Owner, apl_owner);
      }catch(Exception &e)
         {
         form = NULL;
         }
	return(form);
	}

//---------------------------------------------------------------------------
//eof

