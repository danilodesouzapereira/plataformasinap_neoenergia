// ---------------------------------------------------------------------------
#ifndef TEstiloComentarioH
#define TEstiloComentarioH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEstiloComentario.h"
#include <map>
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
class TEstiloComentario : public VTEstiloComentario
{
	public:
					__fastcall TEstiloComentario(VTApl *apl);
					__fastcall ~TEstiloComentario(void);
		bool __fastcall LeEstiloAtributos(strESTILO_ATRIBUTO *ref);
		bool __fastcall SalvaEstiloAtributos(void);
};

#endif
// ---------------------------------------------------------------------------
// eof
