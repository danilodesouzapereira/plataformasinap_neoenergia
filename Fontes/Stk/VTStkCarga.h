//---------------------------------------------------------------------------
#ifndef VTStkCargaH
#define VTStkCargaH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
class VTRedes;

//---------------------------------------------------------------------------
class VTStkCarga : public TObject
	{
   public :
			          __fastcall  VTStkCarga(void) {};
		virtual      __fastcall ~VTStkCarga(void) {};
	  virtual bool __fastcall  AlteraDemanda(VTCarga *carga, int ind_pat, double p_kw, double q_kvar) = 0;
	  virtual bool __fastcall  EquilibraRedes(VTRedes *redes) = 0;
	  virtual bool __fastcall  RedeEquilibrada(VTRedes *redes) = 0;
      virtual bool __fastcall  RedefineCargaComDemandaPropriaDesequilibrada(VTCarga *carga) = 0;
	  virtual bool __fastcall  RedefineCargaComDemandaPropriaEquilibrada(VTCarga *carga) = 0;
	};

//---------------------------------------------------------------------------
VTStkCarga* __fastcall NewObjStkCarga(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof
