//---------------------------------------------------------------------------
#ifndef TResFlowLigH
#define TResFlowLigH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTResFlowLig.h"

//---------------------------------------------------------------------------
class TResFlowLig : public VTResFlowLig
   {
   public:
                      __fastcall  TResFlowLig(VTLigacao *pliga, int num_bar, int num_pat);
						    __fastcall ~TResFlowLig(void);
		void            __fastcall  CopiaAtributosDe(VTResFlowLig &ref);
      bool            __fastcall  ExisteDiagnostico(int tipo_diag);
      void            __fastcall  Inicia(int np);
		void            __fastcall  Reinicia(void);
      complex<double> __fastcall  S_mva_max_allpat(int nb);
      complex<double> __fastcall  S_mva_med_allpat(int nb);
      complex<double> __fastcall  S_mva_min_allpat(int nb);

   private: //métodos
      complex<double> __fastcall CorrenteFase_amp(complex<double>sf_mva, complex<double>vf_kv);
      complex<double> __fastcall CorrenteNeutro_amp(complex<double>sf_mva, complex<double>vf_kv);
      double          __fastcall FatorPotencia(complex<double> s);
      int             __fastcall IndexOf(int nb, int np);
      complex<double> __fastcall TensaoFaseTerra(int nb, int np, int nf);
      complex<double> __fastcall TensaoNeutro(int nb, int np);

   private: //métodos acessados via property
      //carregamento (%)
      double          __fastcall PM_GetCargto(int np);
      double          __fastcall PM_GetCargtoMax_pu(void);
      double          __fastcall PM_GetCargtoMin_pu(void);
      //carregamento (pu)
      double          __fastcall PM_GetCargto_pu(int np);
      void            __fastcall PM_SetCargto_pu(int np, double valor);
      //diagnóstico
      int             __fastcall PM_GetDiagnostico(int np);
      void            __fastcall PM_SetDiagnostico(int np, int valor);
      //ligação associada
      VTLigacao*      __fastcall PM_GetLigacao(void);
      //número de barras
      int             __fastcall PM_GetMaxBar(void);
      //número de patamares
      int             __fastcall PM_GetMaxPat(void);
      void            __fastcall PM_SetMaxPat(int max_pat);
      //sentido do fluxo
      int             __fastcall PM_GetSentidoP(int nb, int np);
      int             __fastcall PM_GetSentidoQ(int nb, int np);
      void            __fastcall PM_SetSentidoP(int nb, int np, int valor);
      void            __fastcall PM_SetSentidoQ(int nb, int np, int valor);
      //potência total
      double          __fastcall PM_GetP_mw(int nb, int np);
      double          __fastcall PM_GetQ_mvar(int nb, int np);
      complex<double> __fastcall PM_GetS_mva(int nb, int np);
      //potência de fase
      double          __fastcall PM_GetPa_mw(int nb, int np);
      double          __fastcall PM_GetPb_mw(int nb, int np);
      double          __fastcall PM_GetPc_mw(int nb, int np);
      double          __fastcall PM_GetPf_mw(int nb, int np, int nf);
      double          __fastcall PM_GetPf_mw_max(int nb, int np);
      double          __fastcall PM_GetPf_mw_min(int nb, int np);
      double          __fastcall PM_GetQa_mvar(int nb, int np);
      double          __fastcall PM_GetQb_mvar(int nb, int np);
      double          __fastcall PM_GetQc_mvar(int nb, int np);
      double          __fastcall PM_GetQf_mvar(int nb, int np, int nf);
      double          __fastcall PM_GetQf_mvar_max(int nb, int np);
      double          __fastcall PM_GetQf_mvar_min(int nb, int np);
      complex<double> __fastcall PM_GetSa_mva(int nb, int np);
      complex<double> __fastcall PM_GetSb_mva(int nb, int np);
      complex<double> __fastcall PM_GetSc_mva(int nb, int np);
      complex<double> __fastcall PM_GetSf_mva(int nb, int np, int nf);
      double          __fastcall PM_GetSf_mva_max(int nb, int np);
      double          __fastcall PM_GetSf_mva_min(int nb, int np);
      //potência de neutro
      double          __fastcall PM_GetPn_mw(int nb, int np);
      double          __fastcall PM_GetQn_mvar(int nb, int np);
      complex<double> __fastcall PM_GetSn_mva(int nb, int np);
      //fator de potência
      double          __fastcall PM_GetFP_pu(int nb, int np);
      double          __fastcall PM_GetFPa_pu(int nb, int np);
      double          __fastcall PM_GetFPb_pu(int nb, int np);
      double          __fastcall PM_GetFPc_pu(int nb, int np);
      double          __fastcall PM_GetFPf_pu(int nb, int np, int nf);
      double          __fastcall PM_GetFPf_pu_max(int nb, int np);
      double          __fastcall PM_GetFPf_pu_min(int nb, int np);
      double          __fastcall PM_GetFPn_pu(int nb, int np);
      //corrente de fase
      complex<double> __fastcall PM_GetIa_amp(int nb, int np);
      complex<double> __fastcall PM_GetIb_amp(int nb, int np);
      complex<double> __fastcall PM_GetIc_amp(int nb, int np);
      complex<double> __fastcall PM_GetIf_amp(int nb, int np, int nf);
      double          __fastcall PM_GetIf_amp_max(int nb, int np);
      double          __fastcall PM_GetIf_amp_min(int nb, int np);
      void            __fastcall PM_SetIa_amp(int nb, int np, complex<double> i_amp);
      void            __fastcall PM_SetIb_amp(int nb, int np, complex<double> i_amp);
      void            __fastcall PM_SetIc_amp(int nb, int np, complex<double> i_amp);
      void            __fastcall PM_SetIf_amp(int nb, int np, int nf, complex<double> i_amp);
      //corrente de neutro
      complex<double> __fastcall PM_GetIn_amp(int nb, int np);
      void            __fastcall PM_SetIn_amp(int nb, int np, complex<double> i_amp);
      //energia
      double          __fastcall PM_GetEnergiaDiaria_mwh(int nb);
      //perda
      double          __fastcall PM_GetPerda_mw(int np);
      double          __fastcall PM_GetPerdaDiaria_mwh(void);
      double          __fastcall PM_GetPerdaDiariaFerro_mwh(void);
      void            __fastcall PM_SetPerdaDiaria_mwh(double perda_mwh);
      void            __fastcall PM_SetPerdaDiariaFerro_mwh(double perda_mwh);
      //queda de tensão em pu
      double          __fastcall PM_GetQuedaVfn_pu_max(int np);
      //índice de balanceamento
      double          __fastcall PM_GetIndiceBal(int nb, int np);

   private: //métodos
      void __fastcall VetorDelete(void);
      void __fastcall VetorNew(void);

   private: //objetos externos

   protected: //dados acessados via property
      struct   {
               VTLigacao *ligacao;
               int       max_bar;
               int       max_pat;
               } PD;

   private: //dados locais
      float  perda_diaria_mwh;    //perda energia
      float  perda_diaria_fe_mwh; //perda em energia no ferro - usado apenas p/ Trafo e Trafo3E
      int    *diag;               //diagnóstico
      float  *cargto;             //carregamento
      int    *sentido;            //sentido da pot.ativa e reativa em relação a cada barra da ligação
      struct strCORRENTE { //organizado como vet[Max_bar][Max_pat]
                         complex<float> ia_amp, ib_amp, ic_amp, in_amp; //A
                         } *corrente;
   };

#endif
//---------------------------------------------------------------------------
//eof

