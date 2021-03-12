#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Simulador\Client\TFormClient.h>
#include <PlataformaSinap\Fontes\Simulador\Client\VTSinapClient.h>
#include <PlataformaSinap\Fontes\Simulador\Server\TFormServer.h>
#include <PlataformaSinap\Fontes\Simulador\Server\VTSinapServer.h>
#include <PlataformaSinap\Fontes\Simulador\User\VTSinapUser.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Simulador.h>
#pragma argsused

#pragma comment(lib, "CurtoCircuito.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "CurtoCircuito.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Stk.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormClient(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   try{//verifica se existe um TFormClient aberto
      if ((form = ExisteForm("TFormClient", Owner)) == NULL)
			{//cria um novo TFormClient
			form = new TFormClient(Owner, apl_owner, parent);
         }
      }catch(Exception &e)
         {
			return(NULL);
			}
	return(form);
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormServer(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   try{//verifica se existe um TFormServer aberto
      if ((form = ExisteForm("TFormServer", Owner)) == NULL)
			{//cria um novo TFormServer
			form = new TFormServer(Owner, apl_owner, parent);
         }
      }catch(Exception &e)
         {
			return(NULL);
			}
	return(form);
	}

//-----------------------------------------------------------------------------
EXPORT VTSinapClient* __fastcall DLL_NewObjSinapClient(VTApl *apl_owner)
   {
   return(NewObjSinapClient(apl_owner));
   }

//-----------------------------------------------------------------------------
EXPORT VTSinapServer* __fastcall DLL_NewObjSinapServer(VTApl *apl_owner)
   {
   return(NewObjSinapServer(apl_owner));
   }

//-----------------------------------------------------------------------------
EXPORT VTSinapUser* __fastcall DLL_NewObjSinapUser(VTApl *apl_owner)
   {
   return(NewObjSinapUser(apl_owner));
   }

//---------------------------------------------------------------------------
//eof


