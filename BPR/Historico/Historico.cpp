#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Historico\BatchDC\TFormBatchDC.h>
#include <Fontes\Historico\BatchIG\TFormBatchIG.h>
#include <Fontes\Historico\BatchPR\TFormBatchPR.h>
#include <Fontes\Historico\BatchPT\TFormBatchPT.h>
#include <DLL_Inc\Historico.h>
#pragma argsused

#pragma comment(lib, "AjusteDemanda.a")
#pragma comment(lib, "BD.a")
#pragma comment(lib, "BDihm.a")
#pragma comment(lib, "Cronometro.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Edita.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "ImportaMedicao.a")
#pragma comment(lib, "ImportaRedeGIS.a")
#pragma comment(lib, "InterfaceTXT.a")
#pragma comment(lib, "Mercado.a")
#pragma comment(lib, "Patamar.a")
#pragma comment(lib, "Perdatec.a")
#pragma comment(lib, "Potencia.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Redutor.a")
#pragma comment(lib, "SinapToolKit.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormBatchDC(TComponent *Owner, VTApl *apl_owner)
   {
   //variáveis locais
   TForm *form;

   try{//cria um novo TFormHistorico que será exibido como janela normal
      form = new TFormBatchDC(Owner, apl_owner);
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(form);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormBatchIG(TComponent *Owner, VTApl *apl_owner)
   {
   //variáveis locais
   TForm *form;

   try{//cria um novo TFormHistorico que será exibido como janela normal
      form = new TFormBatchIG(Owner, apl_owner);
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(form);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormBatchPR(TComponent *Owner, VTApl *apl_owner)
   {
   //variáveis locais
   TForm *form;

   try{//cria um novo TFormBatchPR que será exibido como janela normal
      form = new TFormBatchPR(Owner, apl_owner);
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(form);
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormBatchPT(TComponent *Owner, VTApl *apl_owner)
   {
   //variáveis locais
   TForm *form;

   try{//cria um novo TFormBatchPT que será exibido como janela normal
      form = new TFormBatchPT(Owner, apl_owner);
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(form);
   }

//---------------------------------------------------------------------------
//eof

