//---------------------------------------------------------------------------
#ifndef TResultadosH3H
#define TResultadosH3H

//---------------------------------------------------------------------------
#include <complex>
#include ".\VTResultadosH3.h"
using namespace std;

class TBarraH3;
class VTGeralC;

//---------------------------------------------------------------------------
class TResultadosH3 : public VTResultadosH3
   {
   public:  // Métodos
           __fastcall  TResultadosH3(VTGeralC *geralC,
                                     VTRede3  *rede3,
                                     VTRedeFH *redeFH);
           __fastcall ~TResultadosH3(void);
      bool __fastcall  SalvaResultadosPatamar(int   np,
                                              TList *lisBAR_H3);

   private:  // Objetos externos
      VTGeralC *geralC;
      VTRede3  *rede3;
      VTRedeFH *redeFH;

   private:  // Dados
      double          *vet_dh_corr;
      complex<double> *vet_iamp, *vet_perda_kva;
      complex<double> czero;

   private:  // Dados & Estruturas

   private:  // Métodos
      bool __fastcall SalvaResultadosLigacoes(int      np,
                                              TBarraH3 *barH3);
   };

#endif
//---------------------------------------------------------------------------

