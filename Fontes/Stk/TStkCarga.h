//---------------------------------------------------------------------------
#ifndef TStkCargaH
#define TStkCargaH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTStkCarga.h"

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;

//---------------------------------------------------------------------------
class TStkCarga : public VTStkCarga
	{
   public :
			  __fastcall  TStkCarga(VTApl *apl_owner);
			  __fastcall ~TStkCarga(void);
	  bool __fastcall  AlteraDemanda(VTCarga *carga, int ind_pat, double p_kw, double q_kvar);
	  bool __fastcall  EquilibraRedes(VTRedes *redes);
      bool __fastcall  RedeEquilibrada(VTRedes *redes);
      bool __fastcall  RedefineCargaComDemandaPropriaDesequilibrada(VTCarga *carga);
	  bool __fastcall  RedefineCargaComDemandaPropriaEquilibrada(VTCarga *carga);

	private: //objetos externos
      VTApl *apl;

	private: //métodos
	  bool __fastcall ConverteCargasParaTrifasicas(TList *lisCARGA);
	  void __fastcall LisCargaNaoTrifasica(TList *lisCARGA);
	  void __fastcall LisTrafoNaoTrifasico(TList *lisTRAFO);
	  void __fastcall LisTrechoNaoTrifasico(TList *lisTRECHO);

	private: //dados locais
	};

//---------------------------------------------------------------------------
#endif
//eof
