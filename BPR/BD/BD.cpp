#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\BD\VTBD.h>
#include <Fontes\BD\VTSNPDBConn.h>
#include <DLL_Inc\BD.h>
#pragma argsused

#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "Classes.a")
#pragma comment(lib, "Cronometro.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "Esquematico.a")
#pragma comment(lib, "Figura.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Geral.a")
#pragma comment(lib, "Importa.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "Mapa.a")
#pragma comment(lib, "Mercado.a")
#pragma comment(lib, "MontaZ.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "Registro.a")
//#pragma comment(lib, "doa41c102.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT VTBD* __fastcall DLL_NewObjBD(VTApl *apl)
   {
   return(NewObjBD(apl));
   }
//---------------------------------------------------------------------------
EXPORT VTSNPDBConn* __fastcall DLL_NewObjSNPDBConn(VTApl* apl, AnsiString strDBName, AnsiString strLoginName,
                                                   AnsiString strLoginPwd, int intDBType, bool bolMultiDataSet)
   {
   return(NewObjSNPDBConn(apl, strDBName, strLoginName, strLoginPwd, intDBType, bolMultiDataSet));
   }

//---------------------------------------------------------------------------
//eof

