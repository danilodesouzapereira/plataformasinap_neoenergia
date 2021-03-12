// ---------------------------------------------------------------------------
#ifndef TInfoZccManagerH
#define TInfoZccManagerH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTInfoZccManager.h"
// ---------------------------------------------------------------------------
class VTApl;
// ---------------------------------------------------------------------------
class TInfoZccManager : public VTInfoZccManager
{
public:
	__fastcall TInfoZccManager(VTApl *apl);
	__fastcall ~TInfoZccManager(void);

private:
	strInfoZcc* __fastcall ExisteInfoZcc(int externIdRede);
	strInfoZcc* __fastcall InsereInfoZcc(void);
	bool 		__fastcall LeInfoZcc(void);
	bool		__fastcall SalvaInfoZcc(void);

public:
	TList *lisINFOZCC;

private:
	VTApl *apl;
};

#endif
// ---------------------------------------------------------------------------
// eof
