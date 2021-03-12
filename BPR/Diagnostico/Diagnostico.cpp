#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\DiagEqpto\TFormDiagEqpto.h>
#include <Fontes\DiagFlow\TFormDiagFlow.h>
#include <Fontes\DiagEqpto\VTDiagTopo.h>
#include <Fontes\DiagFlow\VTDiagFlow.h>
#include <DLL_Inc\Diagnostico.h>
#include <DLL_Inc\Funcao.h>
#pragma argsused

#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Legenda.a")
#pragma comment(lib, "Potencia.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Report.a")
#pragma comment(lib, "Zona.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_DiagEqptoEnabled(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
EXPORT bool __fastcall DLL_DiagFlowEnabled(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormDiagEqpto(TForm *form_owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormDiagEqpto abertos
   while ((form = ExisteForm("TFormDiagEqpto", form_owner)) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormDiagFlow(TForm *form_owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os TFormDiagFlow abertos
   while ((form = ExisteForm("TFormDiagFlow", form_owner)) != NULL) delete form;
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormDiagEqpto(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *FormDiagEqpto;

   try{//verifica se existe algum TFormDiagEqpto aberto
      if ((FormDiagEqpto = ExisteForm("TFormDiagEqpto", form_owner)) == NULL)
         {//cria um novo FormDiagEqpto
         FormDiagEqpto = new TFormDiagEqpto(form_owner, apl_owner, parent);
         }
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   //exibe
   FormDiagEqpto->Show();
   return(FormDiagEqpto);
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormDiagFlow(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
   {
   //variáveis locais
   TForm *FormDiagFlow;

   try{//verifica se existe algum TFormDiagFlow aberto
      if ((FormDiagFlow = ExisteForm("TFormDiagFlow", form_owner)) == NULL)
         {//cria um novo FormDiagFlow
         FormDiagFlow = new TFormDiagFlow(form_owner, apl_owner, parent);
         }
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   //exibe
   FormDiagFlow->Show();
   return(FormDiagFlow);
   }

//---------------------------------------------------------------------------
EXPORT VTDiagTopo* __fastcall DLL_NewObjDiagTopo(VTApl *apl, VTProgresso *progresso)
   {
   return(NewObjDiagTopo(apl, progresso));
   }
//---------------------------------------------------------------------------
EXPORT VTDiagFlow*  __fastcall DLL_NewObjDiagFlow(VTApl *apl)
   {
   return(NewObjDiagFlow(apl));
   }

//---------------------------------------------------------------------------
//eof

