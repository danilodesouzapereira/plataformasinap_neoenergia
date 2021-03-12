//---------------------------------------------------------------------------
#ifndef VTAtualiza1H
#define VTAtualiza1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class TBarra;
class VTApl;
class VTGeralC;
class VTRede1;
class VTSisLin;

//---------------------------------------------------------------------------
class VTAtualiza1 : public TObject
   {
   public:  // Métodos
                   __fastcall  VTAtualiza1() {};
      virtual      __fastcall ~VTAtualiza1(void) {};
      virtual bool __fastcall  AtualizaCapacitorReator(int seq,
                                                       int np) = 0;
      virtual bool __fastcall  AtualizaCarga(int np) = 0;
      virtual bool __fastcall  AtualizaCurto(int np) = 0;
      virtual bool __fastcall  AtualizaEstimador1(int np) = 0;
      virtual bool __fastcall  AtualizaFluxoRadial1(int np) = 0;
      virtual bool __fastcall  AtualizaGauss1(int np) = 0;
      virtual bool __fastcall  AtualizaEstabilidade(int np) = 0;
		virtual bool __fastcall  AtualizaNR1(int np) = 0;
      virtual bool __fastcall  AtualizaPartidaMotor(int np) = 0;
      virtual bool __fastcall  AtualizaSuprimentos(int np) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTAtualiza1
//---------------------------------------------------------------------------
VTAtualiza1* __fastcall NewObjAtualiza1(VTRede1  *rede1,
                                        VTGeralC *geralC);

#endif
//---------------------------------------------------------------------------

