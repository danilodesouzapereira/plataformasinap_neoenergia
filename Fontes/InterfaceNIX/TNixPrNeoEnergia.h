// ---------------------------------------------------------------------------
#ifndef TNixPrNeoEnergiaH
#define TNixPrNeoEnergiaH

// ---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>
#include "TNixPr.h"
// ---------------------------------------------------------------------------
class VTChave;
class VTTipoChave;

// ---------------------------------------------------------------------------
class TNixPrNeoEnergia : public TNixPr
{
public:
	__fastcall TNixPrNeoEnergia(VTChave *chave, int index, bool pr_com_corrente);
	__fastcall ~TNixPrNeoEnergia(void);

private: // métodos
	AnsiString __fastcall TraduzCodigo(VTChave *chave, int index);
	AnsiString __fastcall SupoeTipoChave(VTTipoChave *tipo);
};

// ---------------------------------------------------------------------------
#endif
// eof
