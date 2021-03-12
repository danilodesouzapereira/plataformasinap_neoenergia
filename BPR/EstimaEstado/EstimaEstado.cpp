#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\EstimaEstado\VTEstimaEstado.h>
#include <Fontes\EstimaEstado\TFormCfgEstimaEstado.h>
#include <Fontes\EstimaEstado\TFormEstimaEstado.h>
#include <DLL_Inc\EstimaEstado.h>
#include <DLL_Inc\Funcao.h>
#pragma argsused

#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Equivalente.a")
#pragma comment(lib, "Estimador1.a")
#pragma comment(lib, "Estimador3.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormEstimaEstado(TComponent *Owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os FormEstimaEstado abertos
   while ((form = ExisteForm("TFormEstimaEstado", Owner)) != NULL) delete form;
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCfgEstimaEstado(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   try{//verifica se existe um FormCfgEstimaEstado aberto
      if ((form = ExisteForm("TFormCfgEstimaEstado", Owner)) == NULL)
         {//cria um novo FormCfgEstimaEstado
         form = new TFormCfgEstimaEstado(Owner, apl_owner, parent);
         }
      }catch(Exception &e)
         {
			return(NULL);
			}
	return(form);
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEstimaEstado(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *form;

   try{//verifica se existe um TFormEstimaEstado aberto
      if ((form = ExisteForm("TFormEstimaEstado", Owner)) == NULL)
         {//cria um novo TFormEstimaEstado
         form = new TFormEstimaEstado(Owner, apl_owner, parent);
         }
      }catch(Exception &e)
         {
			return(NULL);
			}
	return(form);
	}

//-----------------------------------------------------------------------------
EXPORT VTEstimaEstado* __fastcall DLL_NewObjEstimaEstado(VTApl *apl_owner)
	{
	return(NewObjEstimaEstado(apl_owner));
	}

//---------------------------------------------------------------------------
//eof

