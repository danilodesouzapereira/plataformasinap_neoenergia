// ---------------------------------------------------------------------------
#ifndef TEstiloLinkH
#define TEstiloLinkH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEstiloLink.h"
#include <map>
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
class TEstiloLink : public VTEstiloLink
{
	public:
				__fastcall TEstiloLink(VTApl *apl);
				__fastcall ~TEstiloLink(void);
		bool 	__fastcall LeEstiloAtributos(strESTILO_ATRIBUTO *ref);
		bool __fastcall SalvaEstiloAtributos(void);
	private:
};

#endif
// ---------------------------------------------------------------------------
// eof
