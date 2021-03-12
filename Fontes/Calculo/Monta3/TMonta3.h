//---------------------------------------------------------------------------
#ifndef TMonta3H
#define TMonta3H

//---------------------------------------------------------------------------
#include <complex>
using namespace std;
#include ".\VTMonta3.h"

#define NUM_NOS_COMUNS 3

class  TBarra;
class  TCapSerie;
class  TFase;
class  TReg12;
class  TReg32;
class  TSup;
class  TTrafo12;
class  TTrafo32;
class  TTrafo33;
class  TTrafoEquiv;
class  TTrafoZZ;
class  TTrecho;
class  VTSisLin;
struct smcMUTUA;
struct smcTRAFO2E;
struct smcTRAFO3E;

//---------------------------------------------------------------------------
class TMonta3 : public VTMonta3
   {
   public:  // Métodos
           __fastcall  TMonta3(TImpressao3 *impr3,
                               VTRede3     *rede3,
                               VTGeralC    *geralC,
                               TCurtoAux3  *curto_aux3=NULL);
           __fastcall ~TMonta3(void);
      bool __fastcall  AnalisaSuprimentosPatamar(int np);
      bool __fastcall  AtribuiLinhasY(void);
      bool __fastcall  CalculaZentrada_1Vez3nx3n(bool so_trechos=false);
      // bool __fastcall  CalculaZentrada_nVezes3x3(void);
      bool __fastcall  DefineAterramentoBarras(int             tipo_ater,
                                               complex<double> zater_ohm=complex<double>(0., 0.));
      void __fastcall  ImprimeNumeracaoGlobal(int chave);
      bool __fastcall  InicializaYnodal(void);
      bool __fastcall  MontaLigacoesBarra(void);
      bool __fastcall  MontaRedeEstimador3(void);
      bool __fastcall  MontaRedeFluxoH3(void);
      bool __fastcall  MontaRedeGauss3(void);
      bool __fastcall  MontaRedeNR3(void);
      bool __fastcall  MontaRedeRedutor3(void);
      bool __fastcall  MontaY(int  ordemh=1,
                              bool flag_inclui_mutuas=true,
                              bool so_trechos=false);
      bool __fastcall  MontaY_Conexa(void);
      bool __fastcall  OrdenaBarrasSup(TList    *lisBAR,
                                       VTGeralC *geralC_aux,
                                       TList    *lisSUP=NULL);
      // bool __fastcall  VerificaNeutroConexo(void);
      bool __fastcall  VerificaRedeConexa(bool flag_ordena_barras);

   private:  // Objetos externos
      VTApl       *apl;
      VTData      *data;
      TImpressao3 *impr3;
      VTRede3     *rede3;
      VTGeralC    *geralC;
      TCurtoAux3  *curto_aux3;

   private:  // Dados
      bool            flag_monta_lig_barra; // Controle de montagem lista lig/barra
      int             ordemh;               // Ordem harmônica
      double          graurad, raiz3, raiz3_inverso;
      complex<double> cum, czero;

   private:  // Métodos

      // FUNÇÕES ABAIXO TEMPORÁRIAS, APENAS PARA IMPLEMENTAÇÃO E DEBUG DO ESTIMADOR3
      // bool __fastcall AddMedidor_BAD_MEASUREMENT(void);
      // bool __fastcall AddMedidor_Demo_VTCDs(void);
      // bool __fastcall AddMedidor_LIGmedMOD_I(void);
      // bool __fastcall AddMedidor_LIGmedS_ComSinal(void);
      // bool __fastcall AddMedidor_LIGmedS_SemSinal(void);
      // bool __fastcall AddMedidor_ORIGINAL(void);
      // bool __fastcall AddMedidor_SEM_BARRAS_3456(void);

      void __fastcall AcertaAngRefBarras(void);
      // bool __fastcall AcertaImpedanciaReguladoresNR3(void);
      void __fastcall AcertaMatrizesTrechos(void);
      bool __fastcall AddAterramento(void);
      bool __fastcall AddBarra(void);
      bool __fastcall AddBarraInternaSuprimento(void);
      bool __fastcall AddBarraTrechoFicticios(void);
      bool __fastcall AddBateria(void);
      bool __fastcall AddCapSerie(void);
      bool __fastcall AddCarga(void);
      bool __fastcall AddChave(void);
      bool __fastcall AddCNL(void);
      bool __fastcall AddGrupoLTC12(void);
      bool __fastcall AddGrupoLTC32(void);
      bool __fastcall AddMedidorReal(void);
      bool __fastcall AddMedidorVirtual(void);
      bool __fastcall AddMutua(void);
      bool __fastcall AddReg(void);
      bool __fastcall AddSuprimento(void);
      bool __fastcall AddTrafo(void);
      bool __fastcall AddTrafo12(smcTRAFO2E *trafo, bool flag_regulador);
      bool __fastcall AddTrafo32(smcTRAFO2E *trafo, bool flag_regulador);
      bool __fastcall AddTrafo33(void);
      bool __fastcall AddTrafoEquiv(smcTRAFO2E *trafo);
      bool __fastcall AddTrafoZZ(void);
      bool __fastcall AddTramo(void);
      bool __fastcall AddTrecho(void);
      bool __fastcall AddTrechoInternoSuprimento(void);
      bool __fastcall AddYrefReduc(void);
      bool __fastcall AnalisaAterramentoTrafosReguladoresMono(void);
      bool __fastcall AterraBarrasPorCargaOuEstatico(void);
      void __fastcall CalculaYpYm(int             num_fases,
                                  complex<double> Ynodal0,
                                  complex<double> Ynodal1,
                                  complex<double> *yp,
                                  complex<double> *ym);
      void __fastcall DefineLinhasYnodal(int nv,
                                         int *linha);
      bool __fastcall DeterminaFaseComumAUTOTRAFO4(void);
      bool __fastcall IncluiMutua(smcMUTUA *mutua,
                                  TList    *lisMUTUA_TEMP);
      bool __fastcall IncluirMedidorModV(TBarra *bar1);
      bool __fastcall InicializaTensaoSuprimentos(void);
      bool __fastcall InsereAterramentos(void);
      bool __fastcall InsereCLsH(void);
      bool __fastcall InsereEstaticoBarraH(void);
      bool __fastcall InsereSuprimentosH(void);
      bool __fastcall InsereZdef(bool flag_inverte=false);
      bool __fastcall MontaListaBancosDF(void);
      void __fastcall MontaListasTrafosReguladores(void);
      void __fastcall OrdenaLigacoesBarra(TList *lisLIG);
      bool __fastcall OrdenaMutuas(TList *lisMUTUA_TEMP);
      void __fastcall RedefineAterramentoTrafos(void);
      void __fastcall RenumeraNos(TList *lisTRIPLA_GLOBAL);
      bool __fastcall VerificaAterramento(int    tipo_ater,
                                          TBarra *bar1);
      // bool __fastcall VerificaFasesSuprimentos(void);
      bool __fastcall VerificaTensaoNominalBarras(void);
   };

#endif
//---------------------------------------------------------------------------

