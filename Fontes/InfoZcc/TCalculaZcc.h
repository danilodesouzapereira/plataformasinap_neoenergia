// ---------------------------------------------------------------------------
#ifndef TCalculaZccH
#define TCalculaZccH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "..\RedeCC\ResCurto.h"
#include "VTCalculaZcc.h"
// ---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTBarra;
class VTCurtocircuito;
// ---------------------------------------------------------------------------
class TCalculaZcc : public VTCalculaZcc
{
public:
			__fastcall TCalculaZcc(VTApl *apl);
			__fastcall ~TCalculaZcc(void);
void		__fastcall CalculaZcc(VTBarra *barra, strIMP &z0, strIMP &z1);

private:
    strCalculaZcc* __fastcall ExisteCalculado(VTEqpto *eqpto);
	complex<double> __fastcall StrToComplex(AnsiString dadoValor);
public:
	TList *lisSTRCALCULAZCC;

private:
	VTApl *apl;

private:
	VTCurtocircuito *curtocircuito;
	STR_ZDef zDef;
};

#endif
// ---------------------------------------------------------------------------
// eof
