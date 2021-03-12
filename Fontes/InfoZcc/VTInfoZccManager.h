// ---------------------------------------------------------------------------
#ifndef VTInfoZccManagerH
#define VTInfoZccManagerH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\Estrutura.h"
// ---------------------------------------------------------------------------
#define IZ_ZCCFILE "InfoZcc"
// ---------------------------------------------------------------------------
class VTApl;
// ---------------------------------------------------------------------------
class strInfoZcc : public TObject
{
	public:
		int externIdRede;
		double pcc3f;
		double pcc1f;
		double acc3f;
		double acc1f;
		double sBase_mva;
		double vBase_kva;
		strIMP z0_pu;
		strIMP z1_pu;
		int ref_enr_trafo; //impedancia referenciada a qual enrolamento do trafo
};
// ---------------------------------------------------------------------------
class VTInfoZccManager : public TObject
{
public:
	__fastcall VTInfoZccManager(void){};
	__fastcall ~VTInfoZccManager(void){};

	virtual strInfoZcc* __fastcall ExisteInfoZcc(int externIdRede) = 0;
	virtual strInfoZcc* __fastcall InsereInfoZcc(void) = 0;
	virtual bool 		__fastcall LeInfoZcc(void) = 0;
	virtual bool		__fastcall SalvaInfoZcc(void) = 0;

	//nome dos campos no infoset
	#define izID 	"ID"
	#define izPCC3F	"PCC_3F"
	#define izPCC1F	"PCC_1F"
	#define izACC3F	"ACC_3F"
	#define izACC1F	"ACC_1F"
	#define izSBASE	"SBASE_MVA"
	#define izVBASE	"SVBASE_KV"
	#define izR0    "R0"
	#define izX0    "X0"
	#define izR1    "R1"
	#define izX1    "X1"
	#define izREF 	"REFERENCIA_ENR_TRAFO"
};

// ---------------------------------------------------------------------------
// função global
// ---------------------------------------------------------------------------
VTInfoZccManager* __fastcall NewObjInfoZccManager(VTApl *apl);

#endif
// ---------------------------------------------------------------------------
// eof
