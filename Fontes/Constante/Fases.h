//---------------------------------------------------------------------------
#ifndef FasesH
#define FasesH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//enumerador p/ ID númerico das fases
enum enumFASE  {faseA     = 0x01,
					 faseB     = 0x02,
					 faseC     = 0x04,
					 faseN     = 0x08,
					 faseT     = 0x10,
					 faseAN    = faseA + faseN,
					 faseBN    = faseB + faseN,
					 faseCN    = faseC + faseN,
					 faseAT    = faseA + faseT,
					 faseBT    = faseB + faseT,
					 faseCT    = faseC + faseT,
					 faseNT    = faseN + faseT,
					 faseANT   = faseA + faseN + faseT,
					 faseBNT   = faseB + faseN + faseT,
					 faseCNT   = faseC + faseN + faseT,
					 faseAB    = faseA + faseB,
					 faseBC    = faseB + faseC,
					 faseCA    = faseC + faseA,
					 faseABN   = faseA + faseB + faseN,
					 faseBCN   = faseB + faseC + faseN,
					 faseCAN   = faseC + faseA + faseN,
					 faseABT   = faseA + faseB + faseT,
					 faseBCT   = faseB + faseC + faseT,
					 faseCAT   = faseC + faseA + faseT,
					 faseABNT  = faseA + faseB + faseN + faseT,
					 faseBCNT  = faseB + faseC + faseN + faseT,
					 faseCANT  = faseC + faseA + faseN + faseT,
					 faseABC   = faseA + faseB + faseC,
					 faseABCN  = faseA + faseB + faseC + faseN,
					 faseABCT  = faseA + faseB + faseC + faseT,
					 faseABCNT = faseA + faseB + faseC + faseN + faseT,
					 faseINV   = 0
					 };

//---------------------------------------------------------------------------
enum enumIND_FASE {//índices p/ as fases A, B, C, N
						indFASE_A=0, indFASE_B, indFASE_C, indNEUTRO, MAX_FASE
						};
                  
//---------------------------------------------------------------------------
enum enumIND_SEQ {//índices p/ as sequência 0, 1 e 2
                  indSEQ_0=0, indSEQ_1, indSEQ_2, MAX_SEQ
                  };
                  
//---------------------------------------------------------------------------
#endif
//eof

