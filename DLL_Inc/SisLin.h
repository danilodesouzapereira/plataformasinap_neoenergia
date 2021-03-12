//---------------------------------------------------------------------------
#ifndef SisLinH
#define SisLinH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTSisLin;
class VTTempoCPU;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTSisLin * DLL_NewObjSisLinC(int        num_linhas_submatriz,
                                    int        num_total_linhas,
                                    VTTempoCPU *tCPU=NULL,
                                    AnsiString nome_matriz="MatrizC");

EXPORT VTSisLin * DLL_NewObjSisLinD(int        num_linhas_submatriz,
                                    int        num_total_linhas,
                                    VTTempoCPU *tCPU=NULL,
                                    AnsiString nome_matriz="MatrizD");

EXPORT VTSisLin * DLL_NewObjSisLinLD(int        num_linhas_submatriz,
                                     int        num_total_linhas,
                                     VTTempoCPU *tCPU=NULL,
                                     AnsiString nome_matriz="MatrizLD");

#endif
//---------------------------------------------------------------------------
//eof
