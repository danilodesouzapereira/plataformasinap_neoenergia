//---------------------------------------------------------------------------
#ifndef VTDemandaH
#define VTDemandaH
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;

//---------------------------------------------------------------------------
class VTDemanda : public TObject
   {
   public:  //property
		__property TDateTime       DataReferencia                 = {read=PM_GetDataReferencia, write=PM_SetDataReferencia};
		__property int             DiasMes                        = {read=PM_GetDiasMes};
      __property double          Edia_mwh[VTEqpto *eqpto]       = {read=PM_GetEdia_mwh};
		__property double          Emes_mwh[VTEqpto *eqpto]       = {read=PM_GetEmes_mwh};
      __property double          E_mwh[VTEqpto *eqpto][int np]  = {read=PM_GetE_mwh};
      __property double          P_mw[VTEqpto *eqpto][int np]   = {read=PM_GetP_mw,    write=PM_SetP_mw};
      __property double          Q_mvar[VTEqpto *eqpto][int np] = {read=PM_GetQ_mvar,  write=PM_SetQ_mvar};
      __property complex<double> S_mva[VTEqpto *eqpto][int np]  = {read=PM_GetS_mva,   write=PM_SetS_mva};
      // __property double          Pf_mw[VTEqpto *eqpto][int np][int nf] = {read=PM_GetPf_mw};
      // __property double          Qf_mvar[VTEqpto *eqpto][int np][int nf] = {read=PM_GetQf_mvar};
      // __property complex<double> Sf_mva[VTEqpto *eqpto][int np][int nf]  = {read=PM_GetSf_mva};

   public:
                              __fastcall  VTDemanda(void) {};
      virtual                 __fastcall ~VTDemanda(void) {};
      virtual void            __fastcall  ReiniciaCurva(VTEqpto *eqpto) = 0;
//alteração OpenDSS
      virtual bool            __fastcall  GetSmax_mva(VTEqpto *eqpto, double &s_mva, double &p_mw, double &q_mvar) = 0;
      virtual bool            __fastcall  GetSmedia_mva(VTEqpto *eqpto, double &s_mva, double &p_mw, double &q_mvar) = 0;
      virtual bool            __fastcall  GetSmin_mva(VTEqpto *eqpto, double &s_mva, double &p_mw, double &q_mvar) = 0;
      virtual bool            __fastcall  Get_S3f_mva(VTEqpto *eqpto, int np, complex<double> s_mva[MAX_FASE]) = 0;
      virtual bool            __fastcall  Set_S3f_mva(VTEqpto *eqpto, int np, complex<double> s_mva[MAX_FASE]) = 0;

	protected: //métodos acessados via property
		virtual TDateTime       __fastcall PM_GetDataReferencia(void) = 0;
		virtual int             __fastcall PM_GetDiasMes(void) = 0;
		virtual double          __fastcall PM_GetEdia_mwh(VTEqpto *eqpto) = 0;
		virtual double          __fastcall PM_GetEmes_mwh(VTEqpto *eqpto) = 0;
      virtual double          __fastcall PM_GetE_mwh(VTEqpto *eqpto, int np) = 0;
      virtual double          __fastcall PM_GetP_mw(VTEqpto *eqpto, int np) = 0;
      virtual double          __fastcall PM_GetQ_mvar(VTEqpto *eqpto, int np) = 0;
      virtual complex<double> __fastcall PM_GetS_mva(VTEqpto *eqpto, int np) = 0;
      //métodos p/ atualização
		virtual void            __fastcall PM_SetDataReferencia(TDateTime data) = 0;
      virtual void            __fastcall PM_SetP_mw(VTEqpto   *eqpto, int np, double p) = 0;
      virtual void            __fastcall PM_SetQ_mvar(VTEqpto   *eqpto, int np, double q) = 0;
      virtual bool            __fastcall PM_SetS_mva(VTEqpto *eqpto, int np, complex<double> s_mva) = 0;

      // virtual double          __fastcall PM_GetPf_mw(VTEqpto *eqpto, int np, int nf) = 0;
      // virtual double          __fastcall PM_GetQf_mvar(VTEqpto *eqpto, int np, int nf) = 0;
      // virtual complex<double> __fastcall PM_GetSf_mva(VTEqpto *eqpto, int np, int nf) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTDemanda* __fastcall NewObjDemanda(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
