// ---------------------------------------------------------------------------
#ifndef VTCalculaZccH
#define VTCalculaZccH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\Estrutura.h"
// ---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTEqpto;
// ---------------------------------------------------------------------------
class strCalculaZcc : public TObject
{
	public:
		VTEqpto *eqpto;
		strIMP z0cc_pu;
		strIMP z1cc_pu;
};
// ---------------------------------------------------------------------------
class VTCalculaZcc : public TObject
{
public:
				__fastcall VTCalculaZcc(void){};
				__fastcall ~VTCalculaZcc(void){};
virtual void	__fastcall CalculaZcc(VTBarra *barra, strIMP &z0, strIMP &z01)=0;
};
// ---------------------------------------------------------------------------
// função global
// ---------------------------------------------------------------------------
VTCalculaZcc* __fastcall NewObjCalculaZcc(VTApl *apl);

#endif
// ---------------------------------------------------------------------------
// eof
