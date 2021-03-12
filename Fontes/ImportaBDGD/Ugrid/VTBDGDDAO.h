// --------------------------------------------------------------------------------------------- bof
#ifndef VTBDGDDAOH
#define VTBDGDDAOH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
// ------------------------------------------------------------------------------------------- class
class VTBDGDDAO : public TObject
{

public:
	__fastcall VTBDGDDAO(void){};
	virtual __fastcall ~VTBDGDDAO(void){};
	virtual void __fastcall Close(void) = 0;
};
#endif
//-----------------------------------------------------------------------------
// função global para criar objeto VTBDGDDAO
//-----------------------------------------------------------------------------
VTBDGDDAO* NewObjBDGDDAO(UnicodeString filePathCOMMONS, UnicodeString filePathSED = "");
// --------------------------------------------------------------------------------------------- eof
