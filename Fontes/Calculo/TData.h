//---------------------------------------------------------------------------
#ifndef TDataH
#define TDataH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTData.h"

//---------------------------------------------------------------------------
class VTCanal;
class VTDemanda;
class VTFases;
class VTFornecimento;
class VTPatamares;
class VTMontaZ;
//teste
//class VTLog;

//---------------------------------------------------------------------------
class TData : public VTData
   {
   public:  //métodos
           __fastcall  TData(void);
           __fastcall ~TData(void);
      bool __fastcall  Demanda           (VTEqpto *eqpto, int ind_pat, smcDEMANDA *smc_demanda);
      bool __fastcall  DemandaPorFase    (VTEqpto *eqpto, int ind_pat, smcDEMANDA *smc_demanda);
      bool __fastcall  EqptoAterramento  (int index, smcATERRAMENTO *smc_ater);
      bool __fastcall  EqptoBarra        (int index, smcBARRA       *smc_barra,   void *barraCALC);
      bool __fastcall  EqptoBateria      (int index, smcBATERIA     *smc_bateria);
      bool __fastcall  EqptoCapacitor    (int index, smcCAPACITOR   *smc_capacitor);
      bool __fastcall  EqptoCapserie     (int index, smcCAPSERIE    *smc_capserie);
      bool __fastcall  EqptoCarga        (int index, smcCARGA       *smc_carga);
      bool __fastcall  EqptoChave        (int index, smcCHAVE       *smc_chave);
      bool __fastcall  EqptoCNL          (int index, smcCNL         *smc_cnl);
      bool __fastcall  EqptoFiltro       (int index, smcFILTRO      *smc_cfltro);
      bool __fastcall  EqptoGerador      (int index, smcGERADOR     *smc_gerador);
      bool __fastcall  EqptoMedidor      (int index, smcMEDIDOR     *smc_medidor);
      bool __fastcall  EqptoMutua        (int index, smcMUTUA       *smc_mutua);
      bool __fastcall  EqptoReator       (int index, smcREATOR      *smc_reator);
		bool __fastcall  EqptoRegulador    (int index, smcTRAFO2E     *smc_trafo2e);
      bool __fastcall  EqptoSuprimento   (int index, smcSUPRIMENTO  *smc_suprimento);
      bool __fastcall  EqptoTrafo2E      (int index, smcTRAFO2E     *smc_trafo2E);
      bool __fastcall  EqptoTrafo3E      (int index, smcTRAFO3E     *smc_trafo3E);
      bool __fastcall  EqptoTrafoZZ      (int index, smcTRAFOZZ     *smc_trafoZZ);
      bool __fastcall  EqptoTramo        (int index, smcTRAMO       *smc_tramo);
      bool __fastcall  EqptoTrecho       (int index, smcTRECHO      *smc_trecho);
      bool __fastcall  EqptoYref         (int index, smcYREF        *smc_yref);
      bool __fastcall  EstadoBateria     (smcESTADO_BATERIA *estado_bateria);
      bool __fastcall  ExisteGeracaoPV_VF(VTEqpto *eqpto, int ind_pat);
      bool __fastcall  Geracao           (VTEqpto    *eqpto,
                                          int        ind_pat,
                                          bool       zona_unica,
														smcGERACAO *smc_geracao);
      bool __fastcall  Inicia            (VTApl *apl, VTNet *net);
      void __fastcall  InsereRelatorio   (AnsiString arq_relatorio);
      bool __fastcall  Medicao           (int ind_pat, smcMEDICAO     &smc_medicao);

   protected:  //métodos acessados via property
      int              __fastcall PM_GetAnoCarga(void);
		bool             __fastcall PM_GetAterramentosComuns(void);
		bool             __fastcall PM_GetBarraInternaSuprimentos(void);
		bool             __fastcall PM_GetCapacitanciaLinhas(void);
      bool             __fastcall PM_GetCargas_Ynodal(void);
      int              __fastcall PM_GetErroFluxo(void);
      double           __fastcall PM_GetFrequenciaHz(void);
      bool             __fastcall PM_GetGravaYnodal(void);
      int              __fastcall PM_GetHarmonica(int index);
		bool             __fastcall PM_GetIncluiCNL(void);
      bool             __fastcall PM_GetIncluirChave(void);
      int              __fastcall PM_GetMaxIteracao(void);
      int              __fastcall PM_GetMaxIteracaoVDNRM(void);
      int              __fastcall PM_GetModeloCarga(void);
      AnsiString       __fastcall PM_GetNomeDaRede(void);
      bool             __fastcall PM_GetPerdaFerroEnabled(void);
      int              __fastcall PM_GetNumeroArranjo(void);
      int              __fastcall PM_GetNumeroAterramento(void);
      int              __fastcall PM_GetNumeroBarra(void);
      int              __fastcall PM_GetNumeroBateria(void);
      int              __fastcall PM_GetNumeroCapacitor(void);
      int              __fastcall PM_GetNumeroCapserie(void);
      int              __fastcall PM_GetNumeroCarga(void);
      int              __fastcall PM_GetNumeroChave(void);
      int              __fastcall PM_GetNumeroCNL(void);
      int              __fastcall PM_GetNumeroFase(void);
      int              __fastcall PM_GetNumeroFiltro(void);
      int              __fastcall PM_GetNumeroGerador(void);
      int              __fastcall PM_GetNumeroHarmonicas(void);
      int              __fastcall PM_GetNumeroLigacao(void);
      int              __fastcall PM_GetNumeroMedidor(void);
      int              __fastcall PM_GetNumeroMutua(void);
      int              __fastcall PM_GetNumeroPatamar(void);
      int              __fastcall PM_GetNumeroReator(void);
      int              __fastcall PM_GetNumeroRegulador(void);
      int              __fastcall PM_GetNumeroSuprimento(void);
      int              __fastcall PM_GetNumeroTrafo2E(void);
      int              __fastcall PM_GetNumeroTrafo3E(void);
      int              __fastcall PM_GetNumeroTrafoZZ(void);
      int              __fastcall PM_GetNumeroTramo(void);
		int              __fastcall PM_GetNumeroTrecho(void);
      int              __fastcall PM_GetNumeroYref(void);
      double           __fastcall PM_GetPrecisao(void);
      bool             __fastcall PM_GetRede4Fios(void);
		bool             __fastcall PM_GetRedeComCarga(void);
		VTResCurto     * __fastcall PM_GetResCurto(void);
		VTResEstimador * __fastcall PM_GetResEstimador(void);
		VTResFluxo     * __fastcall PM_GetResFluxo(void);
		double           __fastcall PM_GetSbase(void);
		bool             __fastcall PM_GetUsaPiEquivalente(void);
		bool             __fastcall PM_GetUsaVDNRM(void);
		//
		void             __fastcall PM_SetAnoCarga(int ano_carga);
		void             __fastcall PM_SetAterramentosComuns(bool aterr_comuns);
		void             __fastcall PM_SetBarraInternaSuprimentos(bool bar_int_sup);
		void             __fastcall PM_SetCapacitanciaLinhas(bool capac_LT);
		void             __fastcall PM_SetErroFluxo(int erro_fluxo);
		void             __fastcall PM_SetFrequenciaHz(double frequencia_hz);
		void             __fastcall PM_SetGravaYnodal(bool grava_ynodal);
		void             __fastcall PM_SetIncluiCNL(bool inclui_CNL);
		void             __fastcall PM_SetIncluirChave(bool incluir_chave);
		void             __fastcall PM_SetMaxIteracao(int max_iteracao);
		void             __fastcall PM_SetMaxIteracaoVDNRM(int max_iteracao_vdnrm);
		void             __fastcall PM_SetModeloCarga(int modelo_carga);
		void             __fastcall PM_SetNomeDaRede(AnsiString nome_rede);
		void             __fastcall PM_SetNumeroHarmonicas(int num_harmonicas);
		void             __fastcall PM_SetPerdaFerroEnabled(bool enabled);
		void             __fastcall PM_SetPrecisao(double precisao);
		void             __fastcall PM_SetRedeComCarga(bool rede_com_carga);
		void             __fastcall PM_SetSbase(double sbase);
		void             __fastcall PM_SetUsaPiEquivalente(bool usa_pi_equiv);
		void             __fastcall PM_SetUsaVDNRM(bool usa_vdnrm);

   private: //métodos
      bool      __fastcall CanalRedundante(VTCanal *canal);
      void      __fastcall DefineNomeRede(void);
      void      __fastcall Demanda(smcDEMANDA *smc_demanda, int modelo_carga, int fases_tag, complex<double> s_mva);
      void      __fastcall Demanda(smcDEMANDA *smc_demanda, int modelo_carga, complex<double> sf_mva[MAX_FASE]);
      int       __fastcall DeterminaGrupoLigacao(smcTRAFO3E *smc_trafo3e);
      VTEqpto * __fastcall ExisteEqpto(TList *lisEQP, int eqpto_id);
      void      __fastcall IniciaLisArranjo(TList *lisARR);
      void      __fastcall IniciaLisChave(TList *lisCHV);
      void      __fastcall IniciaLisEqbar(TList *lisEQB, int tipo);
      void      __fastcall IniciaLisLigacao(TList *lisLIG, int tipo);
      void      __fastcall IniciaLisMutua(TList *lisMUT);
      void      __fastcall IniciaLisNo(TList *lisNO);
      void      __fastcall IniciaLisRegulador(TList *lisREG);
      void      __fastcall IniciaStrListHarmonica(void);
      void      __fastcall RedefineDeltaComoPrimario(smcTRAFO3E *smc_trafo3e);
      void      __fastcall RotacionaPriSecTer(smcTRAFO3E *smc_trafo3e);

	private: //objetos externos
      VTApl          *apl;
      VTFases        *fases;
      VTNet          *net;
      VTDemanda      *demanda;
      VTFornecimento *fornecimento;
      VTPatamares    *patamares;

   private: //dados locais
      complex<double> zreg_trafo_pu; // Valor de default para a impedância de reguladores e trafos
		VTMontaZ        *montaZ;
		VTResCurto      *rescurto;
		VTResEstimador  *resestimador;
		VTResFluxo      *resfluxo;
		struct   {
               int        ano_carga;
               bool       aterr_comuns;
               bool       bar_int_sup;
               bool       capac_LT;
					int        erro_fluxo;
               double     frequencia_hz;
               bool       grava_ynodal;
               TStrings   *harmonicas;
               bool       inclui_CNL;
               bool       incluir_chave;
               TList      *lisARRANJO;
               TList      *lisATERRAMENTO;
					TList      *lisBATERIA;
					TList      *lisCAPACITOR;
               TList      *lisCAPSERIE;
               TList      *lisCARGA;
               TList      *lisCHAVE;
               TList      *lisCNL;
               TList      *lisEQP; // Lista usada de maneira temporária
               TList      *lisFILTRO;
               TList      *lisGERADOR;
               TList      *lisLIGACAO;
               TList      *lisLIST;
               TList      *lisMEDIDOR;
               TList      *lisMUTUA;
               TList      *lisNO;
               TList      *lisREATOR;
               TList      *lisREGULADOR;
               TList      *lisSUPRIMENTO;
               TList      *lisTRAFO2E;
               TList      *lisTRAFO3E;
               TList      *lisTRAFOZZ;
               TList      *lisTRAMO;
               TList      *lisTRECHO;
               TList      *lisYREF;
					int        max_iteracao;
					int        max_iteracao_vdnrm;
					int        modelo_carga;
					AnsiString nome_rede;
               int        num_harmonicas;
               bool       perda_ferro_enabled;
               double     precisao;
               bool       rede_com_carga;
               TStrings   *relatorios;
               double     sbase;
               bool       usa_pi_equiv;
               bool       usa_vdnrm;
               } PD;
   };

//---------------------------------------------------------------------------
#endif
//eof


