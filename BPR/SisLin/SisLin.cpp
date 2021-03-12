#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\SisLin\VTSisLin.h>
#include <DLL_Inc\SisLin.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
VTSisLin * DLL_NewObjSisLinC(int        num_linhas_submatriz,
                             int        num_total_linhas,
                             VTTempoCPU *tCPU,
                             AnsiString nome_matriz)
   {
   return(NewObjSisLinC(num_linhas_submatriz, num_total_linhas, tCPU, nome_matriz));
   }

//---------------------------------------------------------------------------
VTSisLin * DLL_NewObjSisLinD(int        num_linhas_submatriz,
                             int        num_total_linhas,
                             VTTempoCPU *tCPU,
                             AnsiString nome_matriz)
   {
   return(NewObjSisLinD(num_linhas_submatriz, num_total_linhas, tCPU, nome_matriz));
   }

//---------------------------------------------------------------------------
VTSisLin * DLL_NewObjSisLinLD(int        num_linhas_submatriz,
                              int        num_total_linhas,
                              VTTempoCPU *tCPU,
                              AnsiString nome_matriz)
   {
   return(NewObjSisLinLD(num_linhas_submatriz, num_total_linhas, tCPU, nome_matriz));
   }

//---------------------------------------------------------------------------
//eof

