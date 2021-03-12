//---------------------------------------------------------------------------
#ifndef VTResFluxoH
#define VTResFluxoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Interface.h"

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTResFluxo : public TObject
   {
   public: //property
		__property double Sbase_mva = {read=PD.sbase_mva, write=PD.sbase_mva};

	public:
						 __fastcall  VTResFluxo(void) {};
		virtual      __fastcall ~VTResFluxo(void) {};
		virtual bool __fastcall  ResFlowBarra      (VTEqpto *eqpto, int ind_pat, smcFLUXO   *smc_fluxo) = 0;
		virtual bool __fastcall  ResFlowBarra      (VTEqpto *eqpto, int ind_pat, smcTENSAO  *smc_tensao) = 0;
      virtual bool __fastcall  ResFlowBateria    (smcESTADO_BATERIA *estado_bateria) = 0;
		virtual bool __fastcall  ResFlowGeracao    (VTEqpto *eqpto, int ind_pat, smcFLUXO   *smc_fluxo) = 0;
		virtual bool __fastcall  ResFlowLigacao    (VTEqpto *eqpto, int ind_bar, int ind_pat, smcFLUXO *smc_fluxo) = 0;
      virtual bool __fastcall  ResFlowRegulador(VTEqpto *eqpto, int ind_pat, smcTRAFO2E *smc_trafo2E) = 0;
		virtual bool __fastcall  ResPerdaEnergiaGlobal (smcPERDA_ENERGIA_GLOBAL  *smc_perda) = 0;
		virtual bool __fastcall  ResPerdaEnergiaLigacao(smcPERDA_ENERGIA_LIGACAO *smc_perda) = 0;

	protected:	//dados acessados via property
		struct{
				double sbase_mva;
				}PD;

	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTResFluxo* __fastcall NewObjResFluxo(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

