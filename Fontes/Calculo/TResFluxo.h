//---------------------------------------------------------------------------
#ifndef TResFluxoH
#define TResFluxoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTResFluxo.h"

//---------------------------------------------------------------------------
class VTFornecimento;

//---------------------------------------------------------------------------
class TResFluxo : public VTResFluxo
	{
   public:
			  __fastcall  TResFluxo(VTApl *apl);
			  __fastcall ~TResFluxo(void);
		bool __fastcall  ResFlowBarra      (VTEqpto *eqpto, int ind_pat, smcFLUXO   *smc_fluxo);
		bool __fastcall  ResFlowBarra      (VTEqpto *eqpto, int ind_pat, smcTENSAO  *smc_tensao);
      bool __fastcall  ResFlowBateria    (smcESTADO_BATERIA *estado_bateria);
		bool __fastcall  ResFlowGeracao    (VTEqpto *eqpto, int ind_pat, smcFLUXO *smc_fluxo);
		bool __fastcall  ResFlowLigacao    (VTEqpto *eqpto, int ind_bar, int ind_pat, smcFLUXO *smc_fluxo);
      bool __fastcall  ResFlowRegulador(VTEqpto *eqpto, int ind_pat, smcTRAFO2E *smc_trafo2E);
		bool __fastcall  ResPerdaEnergiaGlobal (smcPERDA_ENERGIA_GLOBAL  *smc_perda);
		bool __fastcall  ResPerdaEnergiaLigacao(smcPERDA_ENERGIA_LIGACAO *smc_perda);

	private:	//métodos
		void __fastcall RetiraGeradorSuprimentoInativo(TList *lisGS, int ind_pat);
		int  __fastcall SentidoFluxo(double s_mva);

	private: //objetos externos
	  VTApl           *apl;
      VTFornecimento *fornecimento;

	private: //dados locais
		TList *lisEQP;
	};

#endif
//---------------------------------------------------------------------------
//eof

