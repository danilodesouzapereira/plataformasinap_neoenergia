#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <SisLin.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT VTSisLin * DLL_NewObjSisLinC(int        num_linhas_submatriz,
                                    int        num_total_linhas,
                                    VTTempoCPU *tCPU,
                                    AnsiString nome_matriz)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTSisLin * DLL_NewObjSisLinD(int        num_linhas_submatriz,
                                    int        num_total_linhas,
                                    VTTempoCPU *tCPU,
                                    AnsiString nome_matriz)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
EXPORT VTSisLin * DLL_NewObjSisLinLD(int        num_linhas_submatriz,
                                     int        num_total_linhas,
                                     VTTempoCPU *tCPU,
                                     AnsiString nome_matriz)
	{
	return(NULL);
	}

//---------------------------------------------------------------------------
//eof
