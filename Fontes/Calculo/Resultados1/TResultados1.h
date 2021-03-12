//---------------------------------------------------------------------------
#ifndef TResultados1H
#define TResultados1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include ".\VTResultados1.h"
using namespace std;

class  VTGeralC;
struct smcFLUXO;
struct smcTRAFO2E;

//---------------------------------------------------------------------------
class TResultados1 : public VTResultados1
   {
   public:  // Métodos
           __fastcall  TResultados1(VTRede1  *rede1,
                                    VTGeralC *geralC);
           __fastcall ~TResultados1(void);
      bool __fastcall  ResEstabilidade(int np);
      bool __fastcall  ResEstimador1(int np);
      bool __fastcall  ResFluxoRadial1(int np);
      bool __fastcall  ResGauss1(int np);
      bool __fastcall  ResNR1(int np);
      bool __fastcall  ResPerdaEnergia(void);

   private:  // Objetos externos
      VTGeralC *geralC;
      VTData   *data;
      VTRede1  *rede1;

   private:  // Dados
      double          raiz3, raiz3_inverso;
      complex<double> alfa, alfa2, czero;

   private:  // Métodos
      bool __fastcall ResBarra(int np);
      bool __fastcall ResBateria(int np);
      bool __fastcall ResCapSerie(int np);
      bool __fastcall ResMedidor(int np);
      bool __fastcall ResSup(int np);
      bool __fastcall ResTrafo2(int np);
      bool __fastcall ResTrafo3(int np);
      bool __fastcall ResTrafoEquiv(int np);
      bool __fastcall ResTrecho(int np);
      void __fastcall Zera_smcFLUXO(smcFLUXO *smc_fluxo);
      void __fastcall Zera_smcTRAFO2E(smcTRAFO2E *smc_trafo2E);
      bool __fastcall ZeroResCapSerie(int np);
      bool __fastcall ZeroResSup(int np);
      bool __fastcall ZeroResTrafo2(int np);
      bool __fastcall ZeroResTrafo3(int np);
      bool __fastcall ZeroResTrafoEquiv(int np);
      bool __fastcall ZeroResTrecho(int np);
   };

#endif
//---------------------------------------------------------------------------

