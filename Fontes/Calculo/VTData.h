//---------------------------------------------------------------------------
#ifndef VTDataH
#define VTDataH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\Interface.h"

//---------------------------------------------------------------------------
enum enum_ERROS_FLUXO {rfSemErro=0,
                       rfErroMontagemYnodal,
                       rfErroYnodalSingular,
                       rfErroYnodalNaoEspecificado,
                       rfErroNeutroDesconexo,
                       rfErroRedeDesconexa,
                       rfErroTermoConhecido,
                       rfErroCalculoFinal,
                       rfErroImpressao};

// IMPORTANTE: enums abaixo similares a 'enumREGT' em Fontes\Rede\VTRegulador.h
// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             definição de 'codigos_modelo_regu[]', em TGeralC.cpp
enum enum_MODELO_REGULADOR {regINDEF1=0, regPADRAO, regENERQCT1, regENERQCT2,
                            regENERQCT3, regDOIS_SENTIDOS};
// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             definição de 'codigos_sub_modelo_regu[]', em TGeralC.cpp
enum enum_SUB_MODELO_REGULADOR {regINDEF2=0, regAUTO_BI, regAUTO_UNI, regAUTO_FIXA};

// ATENÇÃO: enum abaixo copiada de 'enum TIPO_ZONA' em Fontes\Zona\VTZona.h
enum enum_TIPO_ZONA_CALCULO {znTRIFASICA=1, znMONOFASICA, znDELTA_ABERTO,
                             znDELTA_FECHADO, znINDEFINIDA};

//---------------------------------------------------------------------------
class VTApl;
class VTNet;
class VTResCurto;
class VTResEstimador;
class VTResFluxo;

//---------------------------------------------------------------------------
class VTData : public TObject
   {
   public: //property
		__property int              AnoCarga                = {read=PM_GetAnoCarga,                write=PM_SetAnoCarga};
		__property bool             AterramentosComuns      = {read=PM_GetAterramentosComuns,      write=PM_SetAterramentosComuns};
		__property bool             BarraInternaSuprimentos = {read=PM_GetBarraInternaSuprimentos, write=PM_SetBarraInternaSuprimentos};
		__property bool             CapacitanciaLinhas      = {read=PM_GetCapacitanciaLinhas,      write=PM_SetCapacitanciaLinhas};
		__property bool             Cargas_Ynodal           = {read=PM_GetCargas_Ynodal};
		__property bool             GravaYnodal             = {read=PM_GetGravaYnodal,             write=PM_SetGravaYnodal};
		__property int              ErroFluxo               = {read=PM_GetErroFluxo,               write=PM_SetErroFluxo};
		__property double           FrequenciaHz            = {read=PM_GetFrequenciaHz,            write=PM_SetFrequenciaHz};
		__property int              Harmonica[int index]    = {read=PM_GetHarmonica};
		__property bool             IncluiCNL               = {read=PM_GetIncluiCNL,               write=PM_SetIncluiCNL};
		__property bool             IncluirChave            = {read=PM_GetIncluirChave,            write=PM_SetIncluirChave};
		__property int              MaxIteracao             = {read=PM_GetMaxIteracao,             write=PM_SetMaxIteracao};
		__property int              MaxIteracaoVDNRM        = {read=PM_GetMaxIteracaoVDNRM,        write=PM_SetMaxIteracaoVDNRM};
		__property int              ModeloCarga             = {read=PM_GetModeloCarga,             write=PM_SetModeloCarga};
		__property AnsiString       NomeDaRede              = {read=PM_GetNomeDaRede,              write=PM_SetNomeDaRede};
		__property int              NumeroHarmonicas        = {read=PM_GetNumeroHarmonicas,        write=PM_SetNumeroHarmonicas};
		__property bool             PerdaFerroEnabled       = {read=PM_GetPerdaFerroEnabled,       write=PM_SetPerdaFerroEnabled};
		__property double           Precisao                = {read=PM_GetPrecisao,                write=PM_SetPrecisao};
		__property bool             Rede4Fios               = {read=PM_GetRede4Fios};
		__property bool             RedeComCarga            = {read=PM_GetRedeComCarga,            write=PM_SetRedeComCarga};
		__property VTResCurto     * ResCurto                = {read=PM_GetResCurto};
		__property VTResEstimador * ResEstimador            = {read=PM_GetResEstimador};
		__property VTResFluxo     * ResFluxo                = {read=PM_GetResFluxo};
		__property double           Sbase                   = {read=PM_GetSbase,                   write=PM_SetSbase};
		__property bool             UsaPiEquivalente        = {read=PM_GetUsaPiEquivalente,        write=PM_SetUsaPiEquivalente};
		__property bool             UsaVDNRM                = {read=PM_GetUsaVDNRM,                write=PM_SetUsaVDNRM};

      //---------------------------------------------------------------------
      //quantidades de equipamentos da rede
      __property int NumeroArranjo     = {read=PM_GetNumeroArranjo    };
      __property int NumeroAterramento = {read=PM_GetNumeroAterramento};
      __property int NumeroBarra       = {read=PM_GetNumeroBarra      };
      __property int NumeroBateria     = {read=PM_GetNumeroBateria    };
      __property int NumeroCapacitor   = {read=PM_GetNumeroCapacitor  };
      __property int NumeroCapserie    = {read=PM_GetNumeroCapserie   };
      __property int NumeroCarga       = {read=PM_GetNumeroCarga      };
      __property int NumeroChave       = {read=PM_GetNumeroChave      };
      __property int NumeroCNL         = {read=PM_GetNumeroCNL        };
      __property int NumeroFase        = {read=PM_GetNumeroFase       };
      __property int NumeroFiltro      = {read=PM_GetNumeroFiltro     };
      __property int NumeroGerador     = {read=PM_GetNumeroGerador    };
      __property int NumeroLigacao     = {read=PM_GetNumeroLigacao    };
      __property int NumeroMedidor     = {read=PM_GetNumeroMedidor    };
      __property int NumeroMutua       = {read=PM_GetNumeroMutua      };
      __property int NumeroPatamar     = {read=PM_GetNumeroPatamar    };
      __property int NumeroReator      = {read=PM_GetNumeroReator     };
      __property int NumeroRegulador   = {read=PM_GetNumeroRegulador  };
      __property int NumeroSuprimento  = {read=PM_GetNumeroSuprimento };
      __property int NumeroTrafo2E     = {read=PM_GetNumeroTrafo2E    };
      __property int NumeroTrafo3E     = {read=PM_GetNumeroTrafo3E    };
      __property int NumeroTrafoZZ     = {read=PM_GetNumeroTrafoZZ    };
      __property int NumeroTramo       = {read=PM_GetNumeroTramo      };
      __property int NumeroTrecho      = {read=PM_GetNumeroTrecho     };
      __property int NumeroYref        = {read=PM_GetNumeroYref       };

   public:  //métodos
                   __fastcall  VTData(void) {};
      virtual      __fastcall ~VTData(void) {};
      virtual bool __fastcall  Demanda           (VTEqpto *eqpto, int ind_pat,  smcDEMANDA *smc_demanda) = 0;
      virtual bool __fastcall  DemandaPorFase    (VTEqpto *eqpto, int ind_pat,  smcDEMANDA *smc_demanda) = 0;
      virtual bool __fastcall  EqptoAterramento  (int index, smcATERRAMENTO *smc_ater      ) = 0;
      virtual bool __fastcall  EqptoBarra        (int index, smcBARRA       *smc_barra     , void *barraCALC) = 0;
      virtual bool __fastcall  EqptoBateria      (int index, smcBATERIA     *smc_bateria   ) = 0;
      virtual bool __fastcall  EqptoCapacitor    (int index, smcCAPACITOR   *smc_capacitor ) = 0;
      virtual bool __fastcall  EqptoCapserie     (int index, smcCAPSERIE    *smc_capserie  ) = 0;
      virtual bool __fastcall  EqptoCarga        (int index, smcCARGA       *smc_carga     ) = 0;
      virtual bool __fastcall  EqptoChave        (int index, smcCHAVE       *smc_chave     ) = 0;
      virtual bool __fastcall  EqptoCNL          (int index, smcCNL         *smc_cnl       ) = 0;
      virtual bool __fastcall  EqptoFiltro       (int index, smcFILTRO      *smc_cfltro    ) = 0;
      virtual bool __fastcall  EqptoGerador      (int index, smcGERADOR     *smc_gerador   ) = 0;
      virtual bool __fastcall  EqptoMedidor      (int index, smcMEDIDOR     *smc_medidor   ) = 0;
      virtual bool __fastcall  EqptoMutua        (int index, smcMUTUA       *smc_mutua     ) = 0;
      virtual bool __fastcall  EqptoReator       (int index, smcREATOR      *smc_reator    ) = 0;
		virtual bool __fastcall  EqptoRegulador    (int index, smcTRAFO2E     *smc_trafo2e   ) = 0;
      virtual bool __fastcall  EqptoSuprimento   (int index, smcSUPRIMENTO  *smc_suprimento) = 0;
      virtual bool __fastcall  EqptoTrafo2E      (int index, smcTRAFO2E     *smc_trafo2E   ) = 0;
      virtual bool __fastcall  EqptoTrafo3E      (int index, smcTRAFO3E     *smc_trafo3E   ) = 0;
      virtual bool __fastcall  EqptoTrafoZZ      (int index, smcTRAFOZZ     *smc_trafoZZ   ) = 0;
      virtual bool __fastcall  EqptoTramo        (int index, smcTRAMO       *smc_tramo     ) = 0;
      virtual bool __fastcall  EqptoTrecho       (int index, smcTRECHO      *smc_trecho    ) = 0;
      virtual bool __fastcall  EqptoYref         (int index, smcYREF        *smc_yref      ) = 0;
      virtual bool __fastcall  EstadoBateria     (smcESTADO_BATERIA *estado_bateria) = 0;
      virtual bool __fastcall  ExisteGeracaoPV_VF(VTEqpto *eqpto, int ind_pat) = 0;
      virtual bool __fastcall  Geracao           (VTEqpto    *eqpto,
                                                  int        ind_pat,
                                                  bool       zona_unica,
                                                  smcGERACAO *smc_geracao) = 0;
      virtual bool __fastcall  Inicia            (VTApl *apl, VTNet *net) = 0;
      virtual void __fastcall  InsereRelatorio   (AnsiString arq_relatorio) = 0;
      virtual bool __fastcall  Medicao           (int ind_pat, smcMEDICAO     &smc_medicao) = 0;

   protected:  //métodos acessados via property
		virtual int              __fastcall PM_GetAnoCarga(void) = 0;
		virtual bool             __fastcall PM_GetAterramentosComuns(void) = 0;
		virtual bool             __fastcall PM_GetBarraInternaSuprimentos(void) = 0;
		virtual bool             __fastcall PM_GetCapacitanciaLinhas(void) = 0;
		virtual bool             __fastcall PM_GetCargas_Ynodal(void) = 0;
		virtual int              __fastcall PM_GetErroFluxo(void) = 0;
		virtual double           __fastcall PM_GetFrequenciaHz(void) = 0;
		virtual bool             __fastcall PM_GetGravaYnodal(void) = 0;
		virtual int              __fastcall PM_GetHarmonica(int index) = 0;
		virtual bool             __fastcall PM_GetIncluiCNL(void) = 0;
		virtual bool             __fastcall PM_GetIncluirChave(void) = 0;
		virtual int              __fastcall PM_GetMaxIteracao(void) = 0;
		virtual int              __fastcall PM_GetMaxIteracaoVDNRM(void) = 0;
		virtual int              __fastcall PM_GetModeloCarga(void) = 0;
		virtual AnsiString       __fastcall PM_GetNomeDaRede(void) = 0;
		virtual bool             __fastcall PM_GetPerdaFerroEnabled(void) = 0;
		virtual int              __fastcall PM_GetNumeroArranjo(void) = 0;
		virtual int              __fastcall PM_GetNumeroAterramento(void) = 0;
		virtual int              __fastcall PM_GetNumeroBarra(void) = 0;
		virtual int              __fastcall PM_GetNumeroBateria(void) = 0;
		virtual int              __fastcall PM_GetNumeroCapacitor(void) = 0;
		virtual int              __fastcall PM_GetNumeroCapserie(void) = 0;
		virtual int              __fastcall PM_GetNumeroCarga(void) = 0;
		virtual int              __fastcall PM_GetNumeroChave(void) = 0;
		virtual int              __fastcall PM_GetNumeroCNL(void) = 0;
		virtual int              __fastcall PM_GetNumeroFase(void) = 0;
		virtual int              __fastcall PM_GetNumeroFiltro(void) = 0;
      virtual int              __fastcall PM_GetNumeroGerador(void) = 0;
		virtual int              __fastcall PM_GetNumeroHarmonicas(void) = 0;
		virtual int              __fastcall PM_GetNumeroLigacao(void) = 0;
		virtual int              __fastcall PM_GetNumeroMedidor(void) = 0;
		virtual int              __fastcall PM_GetNumeroMutua(void) = 0;
		virtual int              __fastcall PM_GetNumeroPatamar(void) = 0;
		virtual int              __fastcall PM_GetNumeroReator(void) = 0;
		virtual int              __fastcall PM_GetNumeroRegulador(void) = 0;
		virtual int              __fastcall PM_GetNumeroSuprimento(void) = 0;
      virtual int              __fastcall PM_GetNumeroTrafo2E(void) = 0;
		virtual int              __fastcall PM_GetNumeroTrafo3E(void) = 0;
		virtual int              __fastcall PM_GetNumeroTrafoZZ(void) = 0;
		virtual int              __fastcall PM_GetNumeroTramo(void) = 0;
		virtual int              __fastcall PM_GetNumeroTrecho(void) = 0;
		virtual int              __fastcall PM_GetNumeroYref(void) = 0;
      virtual double           __fastcall PM_GetPrecisao(void) = 0;
		virtual bool             __fastcall PM_GetRedeComCarga(void) = 0;
		virtual bool             __fastcall PM_GetRede4Fios(void) = 0;
		virtual VTResCurto     * __fastcall PM_GetResCurto(void) = 0;
		virtual VTResEstimador * __fastcall PM_GetResEstimador(void) = 0;
		virtual VTResFluxo     * __fastcall PM_GetResFluxo(void) = 0;
		virtual double           __fastcall PM_GetSbase(void) = 0;
		virtual bool             __fastcall PM_GetUsaPiEquivalente(void) = 0;
		virtual bool             __fastcall PM_GetUsaVDNRM(void) = 0;
		//
		virtual void             __fastcall PM_SetAnoCarga(int ano_carga) = 0;
		virtual void             __fastcall PM_SetAterramentosComuns(bool aterr_comuns) = 0;
		virtual void             __fastcall PM_SetBarraInternaSuprimentos(bool bar_int_sup) = 0;
		virtual void             __fastcall PM_SetCapacitanciaLinhas(bool capac_LT) = 0;
		virtual void             __fastcall PM_SetErroFluxo(int erro_fluxo) = 0;
		virtual void             __fastcall PM_SetFrequenciaHz(double frequencia_hz) = 0;
		virtual void             __fastcall PM_SetGravaYnodal(bool grava_ynodal) = 0;
		virtual void             __fastcall PM_SetIncluiCNL(bool inclui_CNL) = 0;
		virtual void             __fastcall PM_SetIncluirChave(bool incluir_chave) = 0;
		virtual void             __fastcall PM_SetMaxIteracao(int max_iteracao) = 0;
		virtual void             __fastcall PM_SetMaxIteracaoVDNRM(int max_iteracao) = 0;
		virtual void             __fastcall PM_SetModeloCarga(int modelo_carga) = 0;
		virtual void             __fastcall PM_SetNomeDaRede(AnsiString nome_rede) = 0;
		virtual void             __fastcall PM_SetNumeroHarmonicas(int num_harmonicas) = 0;
		virtual void             __fastcall PM_SetPerdaFerroEnabled(bool enabled) = 0;
		virtual void             __fastcall PM_SetPrecisao(double precisao) = 0;
		virtual void             __fastcall PM_SetRedeComCarga(bool rede_com_carga) = 0;
		virtual void             __fastcall PM_SetSbase(double sbase) = 0;
		virtual void             __fastcall PM_SetUsaPiEquivalente(bool usa_pi_equiv) = 0;
		virtual void             __fastcall PM_SetUsaVDNRM(bool usa_vdnrm) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTData
//---------------------------------------------------------------------------
VTData* __fastcall NewObjData(void);

//---------------------------------------------------------------------------
#endif
//eof


