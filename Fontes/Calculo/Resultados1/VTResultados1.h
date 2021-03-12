//---------------------------------------------------------------------------
#ifndef VTResultados1H
#define VTResultados1H

//---------------------------------------------------------------------------
#include <complex>
using namespace std;

class VTData;
class VTGeralC;
class VTRede1;

//---------------------------------------------------------------------------
class VTResultados1 : public TObject
   {
   public:  // Métodos
                   __fastcall  VTResultados1() {};
      virtual      __fastcall ~VTResultados1(void) {};
      // virtual bool __fastcall  ResAjuste1(int  np,
      //                                     bool flag_redundancia,
      //                                     int  iter_med) = 0;
      virtual bool __fastcall  ResEstabilidade(int np) = 0;
      virtual bool __fastcall  ResEstimador1(int np) = 0;
      virtual bool __fastcall  ResFluxoRadial1(int np) = 0;
      virtual bool __fastcall  ResGauss1(int np) = 0;
      virtual bool __fastcall  ResNR1(int np) = 0;
      virtual bool __fastcall  ResPerdaEnergia(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTResultados1
//---------------------------------------------------------------------------
VTResultados1 * __fastcall NewObjResultados1(VTRede1  *rede1,
                                             VTGeralC *geralC);

#endif
//---------------------------------------------------------------------------

