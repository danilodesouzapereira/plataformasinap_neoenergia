//---------------------------------------------------------------------------
#ifndef TMonta1H
#define TMonta1H

//---------------------------------------------------------------------------
#include <complex>
#include ".\VTMonta1.h"
using namespace std;

class  TBarra;
class  TCapSerie;
class  TFiltro;
class  TGrupoMutua;
class  TSup;
class  TTrafo2;
class  TTrafo3;
class  TTrafoEquiv;
class  TTrafoZZ;
class  TTrecho;
class  VTGeralC;
class  VTSisLin;
struct smcMUTUA;
struct smcTRAFO2E;

//---------------------------------------------------------------------------
class TMonta1 : public VTMonta1
   {
   public:  // Métodos
           __fastcall TMonta1(TImpressao1 *impr1,
                              VTRede1     *rede1,
                              VTGeralC    *geralC);
           __fastcall ~TMonta1(void);
      bool __fastcall  AnalisaSuprimentosPatamar(int np);
      bool __fastcall  InicializaYnodal(int seq);
      bool __fastcall  MontaLigacoesBarra(void);
      bool __fastcall  MontaRedeCurtoCS(void);
      bool __fastcall  MontaRedeEstabilidade(void);
      bool __fastcall  MontaRedeEstimador1(void);
      bool __fastcall  MontaRedeFluxoRadial1(void);
      bool __fastcall  MontaRedeGauss1(void);
      bool __fastcall  MontaRedeNR1(void);
      bool __fastcall  MontaRedePartidaMotor(void);
      bool __fastcall  MontaRedeRedutor1(void);
      bool __fastcall  MontaY_Seq0(bool so_trechos);
      bool __fastcall  MontaY_Seq1(bool            flag_admit_sup=false,
                                   complex<double> *vet_ycarga=NULL,
                                   bool            flag_inclui_mutuas=true,
                                   bool            so_trechos=false);
      bool __fastcall  MontaY_Seq1_Conexa(void);
      int  __fastcall  OffsetBarra(int linY);
      bool __fastcall  OrdenaBarrasSup(TList    *lisBAR,
                                       VTGeralC *geralC_aux,
                                       TList    *lisSUP=NULL);
      bool __fastcall  VerificaRedeConexa(bool flag_ordena_barras);

   private:  // Objetos externos
      VTApl       *apl;
      VTData      *data;
      TImpressao1 *impr1;
      VTRede1     *rede1;
      VTGeralC    *geralC;

   private:  // Dados
      bool            flag_monta_lig_barra; // Controle de montagem lista lig/barra
      int             *vet_offset_barra;
      double          graurad, raiz3, raiz3_inverso;
      complex<double> cum, cdois, ctres, czero, czero_mais;
      TList           *lisMUTUA_TEMP;

   private:  // Métodos

      // FUNÇÕES ABAIXO TEMPORÁRIAS, APENAS PARA IMPLEMENTAÇÃO E DEBUG DO ESTIMADOR1
      // bool __fastcall AddMedidor_BAD_MEASUREMENT(void);
      // bool __fastcall AddMedidor_LIGmedMOD_I(void);
      // bool __fastcall AddMedidor_LIGmedS_ComSinal(void);
      // bool __fastcall AddMedidor_LIGmedS_SemSinal(void);
      // bool __fastcall AddMedidor_ORIGINAL(void);
      // bool __fastcall AddMedidor_SEM_BARRAS_3456(void);

      // bool __fastcall AcertaImpedanciaReguladoresNR1(void);
      bool __fastcall AddBarra(void);
      bool __fastcall AddBarraTrechoFicticios(void);
      bool __fastcall AddBateria(void);
      bool __fastcall AddCapSerie(void);
      bool __fastcall AddCarga(void);
      bool __fastcall AddChave(void);
      bool __fastcall AddCNL(void);
      bool __fastcall AddFiltro(void);
      bool __fastcall AddGrupoLTC(void);
      bool __fastcall AddMedidorReal(void);
      bool __fastcall AddMedidorVirtual(void);
      bool __fastcall AddMutua(void);
      bool __fastcall AddReg(void);
      bool __fastcall AddSuprimento(void);
      bool __fastcall AddTrafo(void);
      bool __fastcall AddTrafo2(smcTRAFO2E *trafo,
                                bool       flag_regulador);
      bool __fastcall AddTrafo3(void);
      bool __fastcall AddTrafoEquiv(smcTRAFO2E *trafo);
      bool __fastcall AddTrafoZZ(void);
      bool __fastcall AddTramo(void);
      bool __fastcall AddTrecho(void);
      bool __fastcall AddYrefReduc(void);
      // void __fastcall AnalisaChaves(void);
      bool __fastcall CalculaSomaLinhasYnodal(int seq);
      bool __fastcall IncluiMutua(smcMUTUA *mutua);
      bool __fastcall IncluirMedidorModV(TBarra *bar1);
      bool __fastcall InsereCapReator(int seq);
      void __fastcall InsereCargasCapReator_Seq1(void);
      void __fastcall InsereCargasCapReator_Seq1(complex<double> *vet_ycarga);
      bool __fastcall InsereYrefReduc(int seq);
      bool __fastcall InsereYrefSup(int  seq,
                                    bool flag_zpequena);
      void __fastcall MontaListasTrafosReguladores(void);
      void __fastcall OrdenaLigacoesBarra(TList *lisLIG);
      bool __fastcall OrdenaMutuas(void);
   };

#endif
//---------------------------------------------------------------------------

