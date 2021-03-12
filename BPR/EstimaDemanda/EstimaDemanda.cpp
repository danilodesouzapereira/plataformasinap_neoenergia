#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\EstimaDemanda\TFormEstimaDemanda.h>
#include <Fontes\EstimaDemanda\VTEstimaDemanda.h>
#include <DLL_Inc\EstimaDemanda.h>
#include <DLL_Inc\Funcao.h>
#pragma argsused

#pragma comment(lib, "Copia.a")
#pragma comment(lib, "Cronometro.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Equivalente.a")
#pragma comment(lib, "Estimador1.a")
#pragma comment(lib, "Estimador3.a")
#pragma comment(lib, "Flow.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "ImportaMedicao.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "Patamar.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Redutor.a")
#pragma comment(lib, "Stk.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormEstimaDemanda(TComponent *Owner)
   {
   //variáveis locais
   TForm *Form;

   //verifica se jé existe um FormEstimaDemanda aberto
   while ((Form = ExisteForm("TFormEstimaDemanda", Owner)) != NULL) delete Form;
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormCfgEstimaDemanda(TComponent *Owner, VTEstimaDemanda *estima_demanda)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormEstimaDemanda(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *Form;

   //verifica se jé existe um FormEstimaDemanda aberto
   if ((Form = ExisteForm("TFormEstimaDemanda", Owner)) == NULL)
      {//cria um novo TFormEstimaDemanda
      Form = new TFormEstimaDemanda(Owner, apl_owner, parent);
      }
   return(Form);
   }

//---------------------------------------------------------------------------
EXPORT VTEstimaDemanda* __fastcall DLL_NewObjEstimaDemanda(VTApl *apl_owner)
   {
   return(NewObjEstimaDemanda(apl_owner));
   }

//---------------------------------------------------------------------------
//eof

