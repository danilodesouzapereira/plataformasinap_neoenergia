#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <TAplBatch.h>
#include <TAplChild.h>
#include <TAplMdi.h>
#include <TAplRnt.h>
#include <Fontes\Apl\VTApl.h>
#include <DLL_Inc\SinapApl.h>
#pragma argsused

#pragma comment(lib, "Alerta.a")
#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "BD.a")
#pragma comment(lib, "BDihm.a")
#pragma comment(lib, "Busca.a")
#pragma comment(lib, "Classes.a")
#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Cronometro.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Diretorio.a")
#pragma comment(lib, "Edita.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Figura.a")
#pragma comment(lib, "Flow.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Geral.a")
#pragma comment(lib, "Grafico.a")
#pragma comment(lib, "Licenca.a")
#pragma comment(lib, "Manobra.a")
#pragma comment(lib, "Mapa.a")
#pragma comment(lib, "Mercado.a")
#pragma comment(lib, "MontaZ.a")
#pragma comment(lib, "Patamar.a")
#pragma comment(lib, "Planejamento.a")
#pragma comment(lib, "Potencia.a")
#pragma comment(lib, "Produto.a")
#pragma comment(lib, "Protesw.a")
#pragma comment(lib, "RBAC.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Redegraf.a")
#pragma comment(lib, "Topologia.a")
#pragma comment(lib, "Wizard.a")
#pragma comment(lib, "Zona.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTApl* __fastcall DLL_NewObjApl(TForm *FormOwner, VTApl *AplOwner)
   {
   return(NewObjApl(FormOwner, AplOwner));
   }

//-----------------------------------------------------------------------------
EXPORT VTApl* __fastcall DLL_NewObjAplBatch(TForm *FormOwner, VTApl *AplOwner)
	{
	return(new TAplBatch(FormOwner, AplOwner));
	}

//-----------------------------------------------------------------------------
EXPORT VTApl* __fastcall DLL_NewObjAplChild(TForm *FormOwner, VTApl *AplOwner, TAction *ActionInsRede)
   {
   return(new TAplChild(FormOwner, AplOwner, ActionInsRede));
   }

//-----------------------------------------------------------------------------
EXPORT VTApl* __fastcall DLL_NewObjAplMdi(TForm *FormOwner, VTApl *AplOwner)
	{
	return(new TAplMdi(FormOwner, AplOwner));
	}

//-----------------------------------------------------------------------------
EXPORT VTApl* __fastcall DLL_NewObjAplRnt(TForm *FormOwner, VTApl *AplOwner)
	{
	return(new TAplRnt(FormOwner, AplOwner));
	}

//---------------------------------------------------------------------------
//eof

