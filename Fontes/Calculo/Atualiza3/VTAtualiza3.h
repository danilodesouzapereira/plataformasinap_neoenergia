//---------------------------------------------------------------------------
#ifndef VTAtualiza3H
#define VTAtualiza3H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TBarra;
class VTApl;
class VTGeralC;
class VTRede3;
class VTSisLin;

//---------------------------------------------------------------------------
class VTAtualiza3 : public TObject
   {
   public:  // Métodos
                   __fastcall  VTAtualiza3() {};
      virtual      __fastcall ~VTAtualiza3(void) {};
      virtual bool __fastcall  AtualizaCNLsFundamental(bool flag_troca_sinal=false) = 0;
      virtual bool __fastcall  AtualizaEstimador3(int np) = 0;
      virtual bool __fastcall  AtualizaFluxoH3(int np) = 0;
      virtual bool __fastcall  AtualizaGauss3(int np) = 0;
      virtual bool __fastcall  AtualizaNR3(int np) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTAtualiza3
//---------------------------------------------------------------------------
VTAtualiza3* __fastcall NewObjAtualiza3(VTRede3  *rede3,
                                        VTGeralC *geralC);

#endif
//---------------------------------------------------------------------------

