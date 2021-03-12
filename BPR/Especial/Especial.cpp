#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Especial\TFormEspecial.h>
#include <Fontes\Curvas\VTCurva.h>
#include <DLL_Inc\Especial.h>
#pragma argsused

#pragma comment(lib, "AjusteDemanda.a")
#pragma comment(lib, "AlocaCapacitor.a")
#pragma comment(lib, "AlocaRegulador.a")
#pragma comment(lib, "BDihm.a")
#pragma comment(lib, "Balanceamento.a")
#pragma comment(lib, "Cartografia.a")
#pragma comment(lib, "Confiabilidade.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "CustoNodal.a")
#pragma comment(lib, "Debug.a")
#pragma comment(lib, "Diagnostico.a")
#pragma comment(lib, "Distancia.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Equivalente.a")
#pragma comment(lib, "EstimaDemanda.a")
#pragma comment(lib, "EstimaEstado.a")
#pragma comment(lib, "FluxoH3.a")
#pragma comment(lib, "Figura.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Grafico.a")
#pragma comment(lib, "IhmCustoNodal.a")
#pragma comment(lib, "InterfaceNIX.a")
#pragma comment(lib, "InterfaceODSS.a")
#pragma comment(lib, "Mercado.a")
#pragma comment(lib, "Perdatec.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Protecao.a")
#pragma comment(lib, "RBAC.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Relatorio.a")
#pragma comment(lib, "RelatorioJusante.a")
#pragma comment(lib, "RelatorioTensao.a")
#pragma comment(lib, "Report.a")
#pragma comment(lib, "Simulador.a")
#pragma comment(lib, "Stk.a")

//---------------------------------------------------------------------------
static TFormEspecial* __fastcall ExisteFormEspecial(TComponent *Owner);

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_Especial_Atualiza(TComponent *Owner)
   {
   //variáveis locais
   TFormEspecial *FormEspecial;

   if ((FormEspecial = ExisteFormEspecial(Owner)) != NULL)
      {
      FormEspecial->Atualiza();
      }
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_Especial_Start(TComponent *Owner, VTApl *apl_owner)
   {
   try{//cria TFormEspecial e exibe como janela normal
      new TFormEspecial(Owner, apl_owner);
      }catch(Exception &e)
         {
         }
   }

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_Especial_Stop(TComponent *Owner)
   {
   //variáveis locais
   TForm *form;

   //destrói todos os FormEspecial abertos
   while ((form = ExisteFormEspecial(Owner)) != NULL) delete form;
   }

//---------------------------------------------------------------------------
//funções locais da DLL
//---------------------------------------------------------------------------
static TFormEspecial* __fastcall ExisteFormEspecial(TComponent *Owner)
   {
   //variáveis locais
   AnsiString class_name;
   TForm      *form;

   //verifica se existe um FormEspecialaberto
   for (int n = 0; n < Screen->FormCount; n++)
      {//verifica se o Form n é um FormEspecial
      class_name = Screen->Forms[n]->ClassName();
      if (class_name.AnsiCompareIC("TFormEspecial") == 0)
         {
         if (Screen->Forms[n]->Owner == Owner) return((TFormEspecial*)Screen->Forms[n]);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

