//---------------------------------------------------------------------------
#ifndef TDemandaH
#define TDemandaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTDemanda.h"
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class VTBateria;
class VTCapacitor;
class VTCarga;
class VTReator;
class VTCalculaDem;
class VTFases;
class VTPatamares;

//---------------------------------------------------------------------------
class TDemanda : public VTDemanda
   {
   public:
                      __fastcall  TDemanda(VTApl *apl);
                      __fastcall ~TDemanda(void);
      void            __fastcall  ReiniciaCurva(VTEqpto *eqpto);
//alteração OpenDSS
      bool            __fastcall  GetSmax_mva(VTEqpto *eqpto, double &s_mva, double &p_mw, double &q_mvar);
      bool            __fastcall  GetSmedia_mva(VTEqpto *eqpto, double &s_mva, double &p_mw, double &q_mvar);
      bool            __fastcall  GetSmin_mva(VTEqpto *eqpto, double &s_mva, double &p_mw, double &q_mvar);
      bool            __fastcall  Get_S3f_mva(VTEqpto *eqpto, int np, complex<double> s_mva[MAX_FASE]);
      bool            __fastcall  Set_S3f_mva(VTEqpto *eqpto, int np, complex<double> s_mva[MAX_FASE]);

	protected: //métodos acessados via property
		TDateTime       __fastcall PM_GetDataReferencia(void);
		int             __fastcall PM_GetDiasMes(void);
		double          __fastcall PM_GetEdia_mwh(VTEqpto *eqpto);
		double          __fastcall PM_GetEmes_mwh(VTEqpto *eqpto);
      double          __fastcall PM_GetE_mwh(VTEqpto *eqpto, int np);
      double          __fastcall PM_GetP_mw(VTEqpto *eqpto, int np);
      double          __fastcall PM_GetQ_mvar(VTEqpto *eqpto, int np);
      complex<double> __fastcall PM_GetS_mva(VTEqpto *eqpto, int np);
      //métodos p/ atualização
      void            __fastcall PM_SetDataReferencia(TDateTime data);
      void            __fastcall PM_SetP_mw(VTEqpto   *eqpto, int np, double p);//hk
      void            __fastcall PM_SetQ_mvar(VTEqpto   *eqpto, int np, double q);//hk
      bool            __fastcall PM_SetS_mva(VTEqpto *eqpto, int np, complex<double> s_mva);

   protected: //métodos
      bool            __fastcall GetValorPorFaseCapacitor(VTCapacitor *capacitor, int np, complex<double> s_mva[MAX_FASE]);
      bool            __fastcall GetValorPorFaseCarga(VTCarga *carga, int np, complex<double> s_mva[MAX_FASE]);
      bool            __fastcall GetValorPorFaseCargaReducao(VTCarga *carga, int np, complex<double> s_mva[MAX_FASE]);
	  bool            __fastcall GetValorPorFaseReator(VTReator *reator, int np, complex<double> s_mva[MAX_FASE]);
	  complex<double> __fastcall GetValorTotalBateria(VTBateria *bateria, int np);
      complex<double> __fastcall GetValorTotalCapacitor(VTCapacitor *capacitor, int np);
      complex<double> __fastcall GetValorTotalCarga(VTCarga *carga, int np);
      complex<double> __fastcall GetValorTotalCargaReducao(VTCarga *carga, int np);
      complex<double> __fastcall GetValorTotalReator(VTReator *reator, int np);
      void            __fastcall ReiniciaCurvaCapacitor(VTCapacitor *capacitor);
      void            __fastcall ReiniciaCurvaCarga(VTCarga *carga);
      void            __fastcall ReiniciaCurvaCargaReducao(VTCarga *carga);
      void            __fastcall ReiniciaCurvaReator(VTReator *reator);
      bool            __fastcall SetUmValorCapacitor(VTCapacitor *capacitor, int np, int ind_valor, double &valor);
      bool            __fastcall SetUmValorCarga(VTCarga *carga, int np, int ind_valor, double &valor);
      bool            __fastcall SetUmValorReator(VTReator *reator, int np, int ind_valor, double &valor);
      bool            __fastcall SetUmValorCargaReducao(VTCarga *carga, int np, int ind_valor, double &valor);
      bool            __fastcall SetValorPorFaseCapacitor(VTCapacitor *capacitor, int np, complex<double> s_mva[MAX_FASE]);
      bool            __fastcall SetValorPorFaseCarga(VTCarga *carga, int np, complex<double> s_mva[MAX_FASE]);
      bool            __fastcall SetValorPorFaseCargaReducao(VTCarga *carga, int np, complex<double> s_mva[MAX_FASE]);
      bool            __fastcall SetValorPorFaseReator(VTReator *reator, int np, complex<double> s_mva[MAX_FASE]);
      bool            __fastcall SetValorTotalCapacitor(VTCapacitor *capacitor, int np, complex<double> s_mva);
      bool            __fastcall SetValorTotalCarga(VTCarga *carga, int np, complex<double> s_mva);
      bool            __fastcall SetValorTotalCargaReducao(VTCarga *carga, int np, complex<double> s_mva);
      bool            __fastcall SetValorTotalReator(VTReator *reator, int np, complex<double> s_mva);

   protected: //objetos externos
      VTApl            *apl;
      VTFases          *fases;
      VTPatamares      *patamares;

   protected: //odados locais
      //enum           indVALOR {indP=0, indQ, indV, indT};
      VTCalculaDem   *calcula;

   private: //dados acessados via property
      struct{
            int data_referencia;
            int dias_mes;
            }PD;
   };

#endif
//---------------------------------------------------------------------------
//eof
