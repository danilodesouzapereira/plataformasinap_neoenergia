// ---------------------------------------------------------------------------
#ifndef TEstiloRetanguloH
#define TEstiloRetanguloH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEstiloRetangulo.h"
#include <map>
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
class TEstiloRetangulo : public VTEstiloRetangulo
{
	public:
				__fastcall TEstiloRetangulo(VTApl *apl);
				__fastcall ~TEstiloRetangulo(void);
		bool 	__fastcall LeEstiloAtributos(strESTILO_ATRIBUTO *ref);
		bool    __fastcall SalvaEstiloAtributos(void);
};

#endif
// ---------------------------------------------------------------------------
// eof
