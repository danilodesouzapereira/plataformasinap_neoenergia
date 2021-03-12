//---------------------------------------------------------------------------
#ifndef VTResCurtoH
#define VTResCurtoH
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\VTLigacao.h"
#include "..\RedeCC\ResCurto.h"

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;

//---------------------------------------------------------------------------
class VTResCurto : public TObject
   {
   public: //property
		__property double Sbase_mva = {read=PD.sbase_mva, write=PD.sbase_mva};

	public:
						 __fastcall  VTResCurto(void) {};
		virtual      __fastcall ~VTResCurto(void) {};
		virtual bool __fastcall  SalvaIccBarDef(VTEqpto  *eqp_no, STR_FASE *ifase, STR_SEQ *iseq, STR_FASE *assim_ifase) = 0;
		virtual bool __fastcall  SalvaIccBarSup(VTEqpto  *eqp_no, STR_FASE *ifase, STR_SEQ *iseq) = 0;
		virtual bool __fastcall  SalvaIccLigacao(VTEqpto *eqp_lig, VTEqpto *eqp_no_ref,  STR_FASE ifase[][NBAR_LIG], STR_SEQ iseq[][NBAR_LIG]) = 0;
      virtual bool __fastcall  SalvaPotenciasCurto(complex<double> s3f_mva, complex<double> sft_mva) = 0;
		virtual bool __fastcall  SalvaVccBarra(VTEqpto *eqp_no, STR_FASE *vfase, STR_SEQ *vseq) = 0;
		virtual bool __fastcall  SalvaZentrada(complex<double> z0_ohm, complex<double> z1_ohm) = 0;
      virtual bool __fastcall  SalvaZentradaSoTrechosRede(complex<double> z0_ohm, complex<double> z1_ohm) = 0;

	protected:	//dados acessados via property
		struct{
				double sbase_mva;
				}PD;

	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTResCurto* __fastcall NewObjResCurto(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

