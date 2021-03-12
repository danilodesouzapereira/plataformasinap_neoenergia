//---------------------------------------------------------------------------
#ifndef VTResultados3H
#define VTResultados3H

//---------------------------------------------------------------------------
#include <complex>
using namespace std;

class TCurtoAux3;
class VTData;
class VTGeralC;
class VTRede3;

//---------------------------------------------------------------------------
class VTResultados3 : public TObject
   {
   public:  // Métodos
                   __fastcall  VTResultados3() {};
      virtual      __fastcall ~VTResultados3(void) {};
      virtual bool __fastcall  ResEstimador3(int np) = 0;
      virtual bool __fastcall  ResFluxoH3(int np) = 0;
      virtual bool __fastcall  ResGauss3_CurtoMA(TCurtoAux3 *curto_aux3,
                                                int         tipo_curto,
                                                bool        salva_zent) = 0;
      virtual bool __fastcall  ResGauss3_Fluxo(int np) = 0;
      virtual bool __fastcall  ResNR3(int np) = 0;
      virtual bool __fastcall  ResPerdaEnergia(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTResultados3
//---------------------------------------------------------------------------
VTResultados3* __fastcall NewObjResultados3(VTRede3  *rede3,
                                            VTGeralC *geralC);

#endif
//---------------------------------------------------------------------------

