//---------------------------------------------------------------------------
#ifndef TAtualiza3H
#define TAtualiza3H

//---------------------------------------------------------------------------
#include <complex>
#include ".\VTAtualiza3.h"
using namespace std;

class TBarra;
class VTData;

//---------------------------------------------------------------------------
class TAtualiza3 : public VTAtualiza3
   {
   public:  // Métodos
           __fastcall  TAtualiza3(VTRede3  *rede3,
                                  VTGeralC *geralC);
           __fastcall ~TAtualiza3(void);
      bool __fastcall  AtualizaBateria(int np);
      bool __fastcall  AtualizaCNLsFundamental(bool flag_troca_sinal=false);
      bool __fastcall  AtualizaEstimador3(int np);
      bool __fastcall  AtualizaFluxoH3(int np);
      bool __fastcall  AtualizaGauss3(int np);
      bool __fastcall  AtualizaNR3(int np);

   private:  // Objetos externos
      VTData   *data;
      VTRede3  *rede3;
      VTGeralC *geralC;

   private:  // Dados
      double          grau_rad, raiz3, raiz3_inverso;
      complex<double> czero;

   private:  // Métodos
      bool __fastcall AtualizaCapacitorReator(int np);
      bool __fastcall AtualizaCarga(int np);
      bool __fastcall AtualizaMedidoresReais(int np);
      bool __fastcall AtualizaSuprimentos(int np);
   };

#endif
//---------------------------------------------------------------------------

