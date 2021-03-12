//---------------------------------------------------------------------------
#ifndef ResCurtoH
#define ResCurtoH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
#define NUM_MAX_DEF   5    //n�mero de defeitos calculados
#define NUM_MAX_FAS   3    //n�mero de tens�es/correntes de fase
#define NUM_MAX_SEQ   3    //n�mero de tens�es/correntes de seq��ncia
#define MAX_CU        NUM_MAX_DEF

//---------------------------------------------------------------------------
enum tipoDEFEITO {defeito3F=0, defeito2F, defeitoFT, defeitoFTZ, defeito2FT};

//---------------------------------------------------------------------------
struct STR_FASE {bool enabled; complex<double> fasA, fasB, fasC, fasN;};
struct STR_SEQ  {bool enabled; complex<double> seq0, seq1, seq2;};
struct STR_ZDef {//imped�ncias de defeito, em ohm
                strIMP  Zdef3F_ff, Zdef2F_ff, Zdef2FT_ff, Zdef2FT_ft, Zdef1FT_ft;
                };

//---------------------------------------------------------------------------
#endif
//eof
