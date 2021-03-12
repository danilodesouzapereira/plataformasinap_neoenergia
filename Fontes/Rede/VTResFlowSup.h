//---------------------------------------------------------------------------
#ifndef VTResFlowSupH
#define VTResFlowSupH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>

//---------------------------------------------------------------------------
class VTEqbar;

//---------------------------------------------------------------------------
class VTResFlowSup : public TObject
   {
   public:  //property
      __property VTEqbar*        Eqbar                  = {read=PM_GetEqbar};
      __property int             Max_pat                = {read=PM_GetMaxPat,      write=PM_SetMaxPat};
      __property int             TipoSup                = {read=PM_GetTipoSup,     write=PM_SetTipoSup};
      //carregamento  (%)
      __property double          Cargto[int np]         = {read=PM_GetCargto};
      __property double          Cargto_pu[int np]      = {read=PM_GetCargto_pu,   write=PM_SetCargto_pu};
      //diagnóstico
      __property int             Diagnostico[int np]    = {read=PM_GetDiagnostico, write=PM_SetDiagnostico};
      //potência total
      __property double          P_mw[int np]           = {read=PM_GetP_mw};
      __property double          Q_mvar[int np]         = {read=PM_GetQ_mvar};
      __property complex<double> S_mva[int np]          = {read=PM_GetS_mva,       write=PM_SetS_mva};
      //potência de fase
      __property double          Pa_mw[int np]          = {read=PM_GetPa_mw};
      __property double          Pb_mw[int np]          = {read=PM_GetPb_mw};
      __property double          Pc_mw[int np]          = {read=PM_GetPc_mw};
      __property double          Pf_mw[int np][int nf]  = {read=PM_GetPf_mw};
      __property double          Pf_mw_max[int np]      = {read=PM_GetPf_mw_max};
      __property double          Pf_mw_min[int np]      = {read=PM_GetPf_mw_min};
      __property double          Qa_mvar[int np]        = {read=PM_GetQa_mvar};
      __property double          Qb_mvar[int np]        = {read=PM_GetQb_mvar};
      __property double          Qc_mvar[int np]        = {read=PM_GetQc_mvar};
      __property double          Qf_mvar[int np][int nf]= {read=PM_GetQf_mvar};
      __property double          Qf_mvar_max[int np]    = {read=PM_GetQf_mvar_max};
      __property double          Qf_mvar_min[int np]    = {read=PM_GetQf_mvar_min};
      __property complex<double> Sa_mva[int np]         = {read=PM_GetSa_mva, write=PM_SetSa_mva};
      __property complex<double> Sb_mva[int np]         = {read=PM_GetSb_mva, write=PM_SetSb_mva};
      __property complex<double> Sc_mva[int np]         = {read=PM_GetSc_mva, write=PM_SetSc_mva};
      __property complex<double> Sf_mva[int np][int nf] = {read=PM_GetSf_mva, write=PM_SetSf_mva};
      __property double          Sf_mva_max[int np]     = {read=PM_GetSf_mva_max};
      __property double          Sf_mva_min[int np]     = {read=PM_GetSf_mva_min};
      //potência de neutro
      __property double          Pn_mw[int np]          = {read=PM_GetPn_mw};
      __property double          Qn_mvar[int np]        = {read=PM_GetQn_mvar};
      __property complex<double> Sn_mva[int np]         = {read=PM_GetSn_mva, write=PM_SetSn_mva};
      //fator de potência de fase
      __property double          FPa[int np]            = {read=PM_GetFPa,};
      __property double          FPb[int np]            = {read=PM_GetFPb,};
      __property double          FPc[int np]            = {read=PM_GetFPc,};
      __property double          FPf[int np][int nf]    = {read=PM_GetFPf,};
      __property double          FPf_max[int np]        = {read=PM_GetFPf_max};
      __property double          FPf_min[int np]        = {read=PM_GetFPf_min};
      //fator de potência de neutro
      __property double          FPn[int np]            = {read=PM_GetFPn};
      //corrente de fase
      __property complex<double> Ia_amp[int np]         = {read=PM_GetIa_amp};
      __property complex<double> Ib_amp[int np]         = {read=PM_GetIb_amp};
      __property complex<double> Ic_amp[int np]         = {read=PM_GetIc_amp};
      __property complex<double> If_amp[int np][int nf] = {read=PM_GetIf_amp};
      __property complex<double> If_amp_max[int np]     = {read=PM_GetIf_amp_max};
      __property complex<double> If_amp_min[int np]     = {read=PM_GetIf_amp_min};
      //corrente de neutro
      __property complex<double> In_amp[int np]         = {read=PM_GetIn_amp};

   public:  //funções públicas
                            __fastcall  VTResFlowSup(void) {};
      virtual               __fastcall ~VTResFlowSup(void) {};
		virtual void          __fastcall  CopiaAtributosDe(VTResFlowSup &ref) = 0;
		virtual bool          __fastcall  ExisteDiagnostico(int tipo_diag) = 0;
		virtual void          __fastcall  Inicia(int np) = 0;
		virtual void          __fastcall  Reinicia(void) = 0;

   protected: //funções acessadas via property
      //Eqbar associado
      virtual VTEqbar*        __fastcall PM_GetEqbar(void) = 0;
      //tipo de suprimento
      virtual int             __fastcall PM_GetTipoSup(void) = 0;
      virtual void            __fastcall PM_SetTipoSup(int tipo_sup) = 0;
      //carregamento (%)
      virtual double          __fastcall PM_GetCargto(int np) = 0;
      //carregamento (pu)
      virtual double          __fastcall PM_GetCargto_pu(int np) = 0;
      virtual void            __fastcall PM_SetCargto_pu(int np, double valor) = 0;
      //diagnóstico
      virtual int             __fastcall PM_GetDiagnostico(int np) = 0;
      virtual void            __fastcall PM_SetDiagnostico(int np, int valor) = 0;
      //número de patamares
      virtual int             __fastcall PM_GetMaxPat(void) = 0;
      virtual void            __fastcall PM_SetMaxPat(int max_pat) = 0;
      //potência total
      virtual double          __fastcall PM_GetP_mw(int np) = 0;
      virtual double          __fastcall PM_GetQ_mvar(int np) = 0;
      virtual complex<double> __fastcall PM_GetS_mva(int np) = 0;
      virtual void            __fastcall PM_SetS_mva(int np, complex<double> s_mva) = 0;
      //potência de fase
      virtual double          __fastcall PM_GetPa_mw(int np) = 0;
      virtual double          __fastcall PM_GetPb_mw(int np) = 0;
      virtual double          __fastcall PM_GetPc_mw(int np) = 0;
      virtual double          __fastcall PM_GetPf_mw(int np, int nf) = 0;
      virtual double          __fastcall PM_GetPf_mw_max(int np) = 0;
      virtual double          __fastcall PM_GetPf_mw_min(int np) = 0;
      virtual double          __fastcall PM_GetQa_mvar(int np) = 0;
      virtual double          __fastcall PM_GetQb_mvar(int np) = 0;
      virtual double          __fastcall PM_GetQc_mvar(int np) = 0;
      virtual double          __fastcall PM_GetQf_mvar(int np, int nf) = 0;
      virtual double          __fastcall PM_GetQf_mvar_max(int np) = 0;
      virtual double          __fastcall PM_GetQf_mvar_min(int np) = 0;
      virtual complex<double> __fastcall PM_GetSa_mva(int np) = 0;
      virtual complex<double> __fastcall PM_GetSb_mva(int np) = 0;
      virtual complex<double> __fastcall PM_GetSc_mva(int np) = 0;
      virtual complex<double> __fastcall PM_GetSf_mva(int np, int nf) = 0;
      virtual double          __fastcall PM_GetSf_mva_max(int np) = 0;
      virtual double          __fastcall PM_GetSf_mva_min(int np) = 0;
      virtual void            __fastcall PM_SetSa_mva(int np, complex<double> s_mva) = 0;
      virtual void            __fastcall PM_SetSb_mva(int np, complex<double> s_mva) = 0;
      virtual void            __fastcall PM_SetSc_mva(int np, complex<double> s_mva) = 0;
      virtual void            __fastcall PM_SetSf_mva(int np, int nf, complex<double> s_mva) = 0;
      //potência de neutro
      virtual double          __fastcall PM_GetPn_mw(int np) = 0;
      virtual double          __fastcall PM_GetQn_mvar(int np) = 0;
      virtual complex<double> __fastcall PM_GetSn_mva(int np) = 0;
      virtual void            __fastcall PM_SetSn_mva(int np, complex<double> s_mva) = 0;
      //fator de potência de fase
      virtual double          __fastcall PM_GetFPa(int np) = 0;
      virtual double          __fastcall PM_GetFPb(int np) = 0;
      virtual double          __fastcall PM_GetFPc(int np) = 0;
      virtual double          __fastcall PM_GetFPf(int np, int nf) = 0;
      virtual double          __fastcall PM_GetFPf_max(int np) = 0;
      virtual double          __fastcall PM_GetFPf_min(int np) = 0;
      //fator de potência de neutro
      virtual double          __fastcall PM_GetFPn(int np) = 0;
      //corrente de fase
      virtual complex<double> __fastcall PM_GetIa_amp(int np) = 0;
      virtual complex<double> __fastcall PM_GetIb_amp(int np) = 0;
      virtual complex<double> __fastcall PM_GetIc_amp(int np) = 0;
      virtual complex<double> __fastcall PM_GetIf_amp(int np, int nf) = 0;
      virtual double          __fastcall PM_GetIf_amp_max(int np) = 0;
      virtual double          __fastcall PM_GetIf_amp_min(int np) = 0;
      //corrente de neutro
      virtual complex<double> __fastcall PM_GetIn_amp(int np) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTResFlowSup
//---------------------------------------------------------------------------
VTResFlowSup* __fastcall NewObjResFlowSup(VTEqbar *eqbar, int num_pat);

//---------------------------------------------------------------------------
#endif
//eof
