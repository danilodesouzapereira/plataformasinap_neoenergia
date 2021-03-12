//---------------------------------------------------------------------------
#ifndef TResEstimadorH
#define TResEstimadorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\VTResEstimador.h"

//---------------------------------------------------------------------------
class VTPatamares;

//---------------------------------------------------------------------------
class TResEstimador : public VTResEstimador
	{
   public:
			  __fastcall  TResEstimador(VTApl *apl);
			  __fastcall ~TResEstimador(void);
		bool __fastcall  ResMedidor(smcRES_MEDIDOR *smc_res_medidor);

	private: //objetos externos
	  VTApl       *apl;
     VTPatamares *patamares;
	};

#endif
//---------------------------------------------------------------------------
//eof

