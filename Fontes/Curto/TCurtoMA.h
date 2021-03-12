
//---------------------------------------------------------------------------
#ifndef TCurtoMAH
#define TCurtoMAH

//---------------------------------------------------------------------------
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include <complex>
#include "VTCurtoMA.h"
#include "..\RedeCC\ResCurto.h"

//---------------------------------------------------------------------------

class VTData;
class VTApl;
class VTBarra;
class VTGauss3;
class VTNet;
class VTRedeCC;
class VTTrecho;

//---------------------------------------------------------------------------
class TCurtoMA : public VTCurtoMA
{
public:
        __fastcall  TCurtoMA(VTApl *apl);
        __fastcall ~TCurtoMA(void);

   bool __fastcall  Executa(int      ind_pat,
                            VTData   *data,
                            VTBarra  *barra,
                            VTNet    *net,
                            VTRedeCC *redeCC);
   bool __fastcall  Executa(int      ind_pat,
                            VTData   *data,
                            TList    *lisDEF,
                            VTNet    *net,
                            VTRedeCC *redeCC);
   bool __fastcall  Executa(int     tipo_def,
                            strIMP  zdef_fase_ohm,
                            strIMP  zdef_terra_ohm,
                            VTBarra *barra);
   bool __fastcall  Executa(int      tipo_def,
                            strIMP   zdef_fase_ohm,
                            strIMP   zdef_terra_ohm,
                            VTTrecho *trecho,
                            VTBarra  *barra_ref,
                            double   dist_m);
   bool __fastcall  Iamp(VTLigacao       *ligacao,
                         complex<double> iamp[MAX_FASE+MAX_FASE]);
   bool __fastcall  Inicia(VTData *data,
                           VTNet  *net,
                           int    ind_pat);
   bool __fastcall  Vfn_kv(VTBarra         *barra,
                           complex<double> vkv[MAX_FASE]);

private:  // Objetos externos
   VTApl    *apl;
   VTGauss3 *gauss3;
};

#endif
//---------------------------------------------------------------------------
//eof

