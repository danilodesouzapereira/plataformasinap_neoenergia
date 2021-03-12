// ---------------------------------------------------------------------------

#ifndef TFabricaFormFinalizaH
#define TFabricaFormFinalizaH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTFabricaFormFinaliza.h"

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
class TFabricaFormFinaliza : public VTFabricaFormFinaliza
{
	public:

			__fastcall 	TFabricaFormFinaliza(VTApl *apl);
			__fastcall 	~TFabricaFormFinaliza(void);
	TForm*  __fastcall  NewFormFinalizaObra(TComponent* Owner, VTApl *apl_owner, VTObra *obra);


};

#endif
// ---------------------------------------------------------------------------
// eof
