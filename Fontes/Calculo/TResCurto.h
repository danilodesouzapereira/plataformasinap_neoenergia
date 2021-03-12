//---------------------------------------------------------------------------
#ifndef TResCurtoH
#define TResCurtoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTResCurto.h"

//---------------------------------------------------------------------------
class VTRedeCC;

//---------------------------------------------------------------------------
class TResCurto : public VTResCurto
   {
   public:
			  __fastcall  TResCurto(VTApl *apl);
			  __fastcall ~TResCurto(void);
		bool __fastcall  SalvaIccBarDef(VTEqpto *eqp_no, STR_FASE *ifase, STR_SEQ *iseq, STR_FASE *assim_ifase);
		bool __fastcall  SalvaIccBarSup(VTEqpto *eqp_no, STR_FASE *ifase, STR_SEQ *iseq);
		bool __fastcall  SalvaIccLigacao(VTEqpto *eqpto, VTEqpto *eqp_no_ref,  STR_FASE ifase[][NBAR_LIG], STR_SEQ iseq[][NBAR_LIG]);
      bool __fastcall  SalvaPotenciasCurto(complex<double> s3f_mva, complex<double> sft_mva);
		bool __fastcall  SalvaVccBarra(VTEqpto *eqp_no, STR_FASE *vfase, STR_SEQ *vseq);
		bool __fastcall  SalvaZentrada(complex<double> z0_ohm, complex<double> z1_ohm);
		bool __fastcall  SalvaZentradaSoTrechosRede(complex<double> z0_ohm, complex<double> z1_ohm);

	protected:	//métodos acessados via property

	private: //objetos externos
		VTApl    *apl;
		VTRedeCC *redeCC;
	};

#endif
//---------------------------------------------------------------------------
//eof

