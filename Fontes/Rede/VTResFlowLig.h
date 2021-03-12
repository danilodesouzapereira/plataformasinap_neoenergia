//---------------------------------------------------------------------------
#ifndef VTResFlowLigH
#define VTResFlowLigH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>

//---------------------------------------------------------------------------
class VTLigacao;

//---------------------------------------------------------------------------
class VTResFlowLig : public TObject
   {
   public:  //property
      __property VTLigacao*      Ligacao                        = {read=PM_GetLigacao};
      __property int             Max_bar                        = {read=PM_GetMaxBar};
      __property int             Max_pat                        = {read=PM_GetMaxPat, write=PM_SetMaxPat};
      //carregamento  (%)
      __property double          Cargto[int np]                 = {read=PM_GetCargto};
      //carregamento  (pu)
      __property double          Cargto_pu[int np]              = {read=PM_GetCargto_pu,   write=PM_SetCargto_pu};
      __property double          CargtoMax_pu                   = {read=PM_GetCargtoMax_pu};
      __property double          CargtoMin_pu                   = {read=PM_GetCargtoMin_pu};
      //diagnóstico
      __property int             Diagnostico[int np]            = {read=PM_GetDiagnostico, write=PM_SetDiagnostico};
      //sentido do fluxo
      __property int             SentidoP[int nb][int np]       = {read=PM_GetSentidoP, write=PM_SetSentidoP};
      __property int             SentidoQ[int nb][int np]       = {read=PM_GetSentidoQ, write=PM_SetSentidoQ};
      //potência total
      __property double          P_mw[int nb][int np]           = {read=PM_GetP_mw};
      __property double          Q_mvar[int nb][int np]         = {read=PM_GetQ_mvar};
      __property complex<double> S_mva[int nb][int np]          = {read=PM_GetS_mva};
      //potência de fase
      __property double          Pa_mw[int nb][int np]          = {read=PM_GetPa_mw};
      __property double          Pb_mw[int nb][int np]          = {read=PM_GetPb_mw};
      __property double          Pc_mw[int nb][int np]          = {read=PM_GetPc_mw};
      __property double          Pf_mw[int nb][int np][int nf]  = {read=PM_GetPf_mw};
      __property double          Pf_mw_max[int nb][int np]      = {read=PM_GetPf_mw_max};
      __property double          Pf_mw_min[int nb][int np]      = {read=PM_GetPf_mw_min};
      __property double          Qa_mvar[int nb][int np]        = {read=PM_GetQa_mvar};
      __property double          Qb_mvar[int nb][int np]        = {read=PM_GetQb_mvar};
      __property double          Qc_mvar[int nb][int np]        = {read=PM_GetQc_mvar};
      __property double          Qf_mvar[int nb][int np][int nf]= {read=PM_GetQf_mvar};
      __property double          Qf_mvar_max[int nb][int np]    = {read=PM_GetQf_mvar_max};
      __property double          Qf_mvar_min[int nb][int np]    = {read=PM_GetQf_mvar_min};
      __property complex<double> Sa_mva[int nb][int np]         = {read=PM_GetSa_mva};
      __property complex<double> Sb_mva[int nb][int np]         = {read=PM_GetSb_mva};
      __property complex<double> Sc_mva[int nb][int np]         = {read=PM_GetSc_mva};
      __property complex<double> Sf_mva[int nb][int np][int nf] = {read=PM_GetSf_mva};
      __property double          Sf_mva_max[int nb][int np]     = {read=PM_GetSf_mva_max};
      __property double          Sf_mva_min[int nb][int np]     = {read=PM_GetSf_mva_min};
      //potência de neutro
      __property double          Pn_mw[int nb][int np]          = {read=PM_GetPn_mw};
      __property double          Qn_mvar[int nb][int np]        = {read=PM_GetQn_mvar};
      __property complex<double> Sn_mva[int nb][int np]         = {read=PM_GetSn_mva};
      //fator de potência total
      __property double          FP_pu[int nb][int np]          = {read=PM_GetFP_pu,};
      //fator de potência de fase
      __property double          FPa_pu[int nb][int np]         = {read=PM_GetFPa_pu,};
      __property double          FPb_pu[int nb][int np]         = {read=PM_GetFPb_pu,};
      __property double          FPc_pu[int nb][int np]         = {read=PM_GetFPc_pu,};
      __property double          FPf_pu[int nb][int np][int nf] = {read=PM_GetFPf_pu,};
      __property double          FPf_pu_max[int nb][int np]     = {read=PM_GetFPf_pu_max};
      __property double          FPf_pu_min[int nb][int np]     = {read=PM_GetFPf_pu_min};
      //fator de potência de neutro
      __property double          FPn_pu[int nb][int np]         = {read=PM_GetFPn_pu};
      //corrente de fase
      __property complex<double> Ia_amp[int nb][int np]         = {read=PM_GetIa_amp,    write=PM_SetIa_amp};
      __property complex<double> Ib_amp[int nb][int np]         = {read=PM_GetIb_amp,    write=PM_SetIb_amp};
      __property complex<double> Ic_amp[int nb][int np]         = {read=PM_GetIc_amp,    write=PM_SetIc_amp};
      __property complex<double> If_amp[int nb][int np][int nf] = {read=PM_GetIf_amp,    write=PM_SetIf_amp};
      __property double          If_amp_max[int nb][int np]     = {read=PM_GetIf_amp_max};
      __property double          If_amp_min[int nb][int np]     = {read=PM_GetIf_amp_min};
      //corrente de neutro
      __property complex<double> In_amp[int nb][int np]         = {read=PM_GetIn_amp,    write=PM_SetIn_amp};
      //energia
      __property double          EnergiaDiaria_mwh[int nb]      = {read=PM_GetEnergiaDiaria_mwh};
      //perda
      __property double          Perda_mw[int np]               = {read=PM_GetPerda_mw};
      __property double          PerdaDiaria_mwh                = {read=PM_GetPerdaDiaria_mwh,      write=PM_SetPerdaDiaria_mwh};
      __property double          PerdaDiariaFerro_mwh           = {read=PM_GetPerdaDiariaFerro_mwh, write=PM_SetPerdaDiariaFerro_mwh};
      //queda de tensão em pu
      __property double          QuedaVfn_pu_max[int np]        = {read=PM_GetQuedaVfn_pu_max};
      //índice de balanceamento
      __property double          IndiceBal[int nb][int np]      = {read=PM_GetIndiceBal};

   public:  //funções públicas
                              __fastcall  VTResFlowLig(void) {};
		virtual                 __fastcall ~VTResFlowLig(void) {};
		virtual void            __fastcall  CopiaAtributosDe(VTResFlowLig &ref) = 0;
		virtual bool            __fastcall  ExisteDiagnostico(int tipo_diag) = 0;
		virtual void            __fastcall  Inicia(int np) = 0;
		virtual void            __fastcall  Reinicia(void) = 0;
      virtual complex<double> __fastcall  S_mva_max_allpat(int nb=0) = 0;
      virtual complex<double> __fastcall  S_mva_med_allpat(int nb=0) = 0;
      virtual complex<double> __fastcall  S_mva_min_allpat(int nb=0) = 0;

   protected: //funções acessadas via property
      //carregamento (%)
      virtual double          __fastcall PM_GetCargto(int np) = 0;
      //carregamento (pu)
      virtual double          __fastcall PM_GetCargto_pu(int np) = 0;
      virtual double          __fastcall PM_GetCargtoMax_pu(void) = 0;
      virtual double          __fastcall PM_GetCargtoMin_pu(void) = 0;
      virtual void            __fastcall PM_SetCargto_pu(int np, double valor) = 0;
      //diagnóstico
      virtual int             __fastcall PM_GetDiagnostico(int np) = 0;
      virtual void            __fastcall PM_SetDiagnostico(int np, int valor) = 0;
      //ligação associada
      virtual VTLigacao*      __fastcall PM_GetLigacao(void) = 0;
      //número de barras
      virtual int             __fastcall PM_GetMaxBar(void) = 0;
      //número de patamares
      virtual int             __fastcall PM_GetMaxPat(void) = 0;
      virtual void            __fastcall PM_SetMaxPat(int max_pat) = 0;
      //sentido do fluxo
      virtual int             __fastcall PM_GetSentidoP(int nb, int np) = 0;
      virtual int             __fastcall PM_GetSentidoQ(int nb, int np) = 0;
      virtual void            __fastcall PM_SetSentidoP(int nb, int np, int valor) = 0;
      virtual void            __fastcall PM_SetSentidoQ(int nb, int np, int valor) = 0;
      //potência total
      virtual double          __fastcall PM_GetP_mw(int nb, int np) = 0;
      virtual double          __fastcall PM_GetQ_mvar(int nb, int np) = 0;
      virtual complex<double> __fastcall PM_GetS_mva(int nb, int np) = 0;
      //potência de fase
      virtual double          __fastcall PM_GetPa_mw(int nb, int np) = 0;
      virtual double          __fastcall PM_GetPb_mw(int nb, int np) = 0;
      virtual double          __fastcall PM_GetPc_mw(int nb, int np) = 0;
      virtual double          __fastcall PM_GetPf_mw(int nb, int np, int nf) = 0;
      virtual double          __fastcall PM_GetPf_mw_max(int nb, int np) = 0;
      virtual double          __fastcall PM_GetPf_mw_min(int nb, int np) = 0;
      virtual double          __fastcall PM_GetQa_mvar(int nb, int np) = 0;
      virtual double          __fastcall PM_GetQb_mvar(int nb, int np) = 0;
      virtual double          __fastcall PM_GetQc_mvar(int nb, int np) = 0;
      virtual double          __fastcall PM_GetQf_mvar(int nb, int np, int nf) = 0;
      virtual double          __fastcall PM_GetQf_mvar_max(int nb, int np) = 0;
      virtual double          __fastcall PM_GetQf_mvar_min(int nb, int np) = 0;
      virtual complex<double> __fastcall PM_GetSa_mva(int nb, int np) = 0;
      virtual complex<double> __fastcall PM_GetSb_mva(int nb, int np) = 0;
      virtual complex<double> __fastcall PM_GetSc_mva(int nb, int np) = 0;
      virtual complex<double> __fastcall PM_GetSf_mva(int nb, int np, int nf) = 0;
      virtual double          __fastcall PM_GetSf_mva_max(int nb, int np) = 0;
      virtual double          __fastcall PM_GetSf_mva_min(int nb, int np) = 0;
      //potência de neutro
      virtual double          __fastcall PM_GetPn_mw(int nb, int np) = 0;
      virtual double          __fastcall PM_GetQn_mvar(int nb, int np) = 0;
      virtual complex<double> __fastcall PM_GetSn_mva(int nb, int np) = 0;
      //fator de potência de fase
      virtual double          __fastcall PM_GetFP_pu(int nb, int np) = 0;
      virtual double          __fastcall PM_GetFPa_pu(int nb, int np) = 0;
      virtual double          __fastcall PM_GetFPb_pu(int nb, int np) = 0;
      virtual double          __fastcall PM_GetFPc_pu(int nb, int np) = 0;
      virtual double          __fastcall PM_GetFPf_pu(int nb, int np, int nf) = 0;
      virtual double          __fastcall PM_GetFPf_pu_max(int nb, int np) = 0;
      virtual double          __fastcall PM_GetFPf_pu_min(int nb, int np) = 0;
      //fator de potência de neutro
      virtual double          __fastcall PM_GetFPn_pu(int nb, int np) = 0;
      //corrente de fase
      virtual complex<double> __fastcall PM_GetIa_amp(int nb, int np) = 0;
      virtual complex<double> __fastcall PM_GetIb_amp(int nb, int np) = 0;
      virtual complex<double> __fastcall PM_GetIc_amp(int nb, int np) = 0;
      virtual complex<double> __fastcall PM_GetIf_amp(int nb, int np, int nf) = 0;
      virtual double          __fastcall PM_GetIf_amp_max(int nb, int np) = 0;
      virtual double          __fastcall PM_GetIf_amp_min(int nb, int np) = 0;
      virtual void            __fastcall PM_SetIa_amp(int nb, int np, complex<double> i_amp) = 0;
      virtual void            __fastcall PM_SetIb_amp(int nb, int np, complex<double> i_amp) = 0;
      virtual void            __fastcall PM_SetIc_amp(int nb, int np, complex<double> i_amp) = 0;
      virtual void            __fastcall PM_SetIf_amp(int nb, int np, int nf, complex<double> i_amp) = 0;
      //corrente de neutro
      virtual complex<double> __fastcall PM_GetIn_amp(int nb, int np) = 0;
      virtual void            __fastcall PM_SetIn_amp(int nb, int np, complex<double> i_amp) = 0;
      //energia
      virtual double          __fastcall PM_GetEnergiaDiaria_mwh(int nb) = 0;
      //perda
      virtual double          __fastcall PM_GetPerda_mw(int np) = 0;
      virtual double          __fastcall PM_GetPerdaDiaria_mwh(void) = 0;
      virtual double          __fastcall PM_GetPerdaDiariaFerro_mwh(void) = 0;
      virtual void            __fastcall PM_SetPerdaDiaria_mwh(double perda_mwh) = 0;
      virtual void            __fastcall PM_SetPerdaDiariaFerro_mwh(double perda_mwh) = 0;
      //queda de tensão
      virtual double          __fastcall PM_GetQuedaVfn_pu_max(int np) = 0;
      //índice de balanceamento
      virtual double          __fastcall PM_GetIndiceBal(int nb, int np) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTResFlowLig
//---------------------------------------------------------------------------
VTResFlowLig* __fastcall NewObjResFlowLig(VTLigacao *pliga, int num_bar, int num_pat);

//---------------------------------------------------------------------------
#endif
//eof
