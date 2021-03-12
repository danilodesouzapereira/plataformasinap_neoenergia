//---------------------------------------------------------------------------
#ifndef VTResEstimadorH
#define VTResEstimadorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\Interface.h"

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTResEstimador : public TObject
   {
	public:
						 __fastcall  VTResEstimador(void) {};
		virtual      __fastcall ~VTResEstimador(void) {};
		virtual bool __fastcall  ResMedidor(smcRES_MEDIDOR *smc_res_medidor) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTResEstimador * __fastcall NewObjResEstimador(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

