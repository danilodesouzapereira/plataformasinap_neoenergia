// ---------------------------------------------------------------------------

#ifndef TFamiliaArranjosH
#define TFamiliaArranjosH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTFamiliaArranjos.h"
#include "..\Constante\Fases.h"
// ---------------------------------------------------------------------------
class VTApl;
class VTArranjo;

// ---------------------------------------------------------------------------
class TFamiliaArranjos : public VTFamiliaArranjos
{
public:
	__fastcall TFamiliaArranjos(VTApl *apl);
	__fastcall ~TFamiliaArranjos(void);

public:
	bool __fastcall InsereArranjo(VTArranjo *arranjo);

private:
	VTArranjo* __fastcall PM_GetArranjo(int fase);

private:// objetos externos
	VTApl *apl;

};

#endif
// ---------------------------------------------------------------------------
// eof
