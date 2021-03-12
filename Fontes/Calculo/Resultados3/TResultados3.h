//---------------------------------------------------------------------------
#ifndef TResultados3H
#define TResultados3H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include ".\VTResultados3.h"
#include "..\..\RedeCC\ResCurto.h"
using namespace std;

struct smcFLUXO;
struct smcTENSAO;
struct smcTRAFO2E;

//---------------------------------------------------------------------------
class TResultados3 : public VTResultados3
   {
   public:  // Métodos
           __fastcall  TResultados3(VTRede3  *rede3,
                                    VTGeralC *geralC);
           __fastcall ~TResultados3(void);
      bool __fastcall  ResEstimador3(int np);
      bool __fastcall  ResFluxoH3(int np);
      bool __fastcall  ResGauss3_CurtoMA(TCurtoAux3 *curto_aux3,
                                         int        tipo_curto,
                                         bool       salva_zent);
      bool __fastcall  ResGauss3_Fluxo(int np);
      bool __fastcall  ResNR3(int np);
      bool __fastcall  ResPerdaEnergia(void);

   private:  // Objetos externos
      VTData   *data;
      VTRede3  *rede3;
      VTGeralC *geralC;

   private:  // Dados
      complex<double> czero;
      STR_SEQ         iseq[MAX_CU];
      STR_FASE        ifase[MAX_CU];
     	STR_FASE        assim_ifase[MAX_CU];

   private:  // Métodos
      bool __fastcall ResBarra(int np);
      bool __fastcall ResBateria(int np);
      bool __fastcall ResCapSerie(int np);
      bool __fastcall ResMedidor(int np);
      bool __fastcall ResSup(int np);
      bool __fastcall ResTrafo12(int np);
      bool __fastcall ResTrafo32(int np);
      bool __fastcall ResTrafo33(int np);
      bool __fastcall ResTrafoEquiv(int np);
      bool __fastcall ResTrecho(int np);
      void __fastcall SalvaResIccBarraDef(TCurtoAux3 *curto_aux3, int nt);
      void __fastcall SalvaResIccBarraSup(TCurtoAux3 *curto_aux3, int nt);
      void __fastcall SalvaResIccCapSerie(TCurtoAux3 *curto_aux3, int nt);
      void __fastcall SalvaResIccTrafo12(TCurtoAux3 *curto_aux3, int nt);
      void __fastcall SalvaResIccTrafo32(TCurtoAux3 *curto_aux3, int nt);
      void __fastcall SalvaResIccTrafo33(TCurtoAux3 *curto_aux3, int nt);
      void __fastcall SalvaResIccTrafoEquiv(TCurtoAux3 *curto_aux3, int nt);
      void __fastcall SalvaResIccTrecho(TCurtoAux3 *curto_aux3, int nt);
      void __fastcall SalvaResVccBarra(TCurtoAux3 *curto_aux3, int nt);
      void __fastcall Zera_smcFLUXO(smcFLUXO *smc_fluxo);
      void __fastcall Zera_smcTENSAO(smcTENSAO *smc_tensao);
      void __fastcall Zera_smcTRAFO2E(smcTRAFO2E *smc_trafo2E);
      void __fastcall Zera_STR_FASE(int nelem, STR_FASE *vetor);
      void __fastcall Zera_STR_SEQ(int nelem, STR_SEQ *vetor);

   };

#endif
//---------------------------------------------------------------------------

