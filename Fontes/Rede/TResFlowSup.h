//---------------------------------------------------------------------------
#ifndef TResFlowSupH
#define TResFlowSupH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTResFlowSup.h"

//---------------------------------------------------------------------------
class TResFlowSup : public VTResFlowSup
   {
   public:
                    __fastcall  TResFlowSup(VTEqbar *eqbar, int num_pat);
						  __fastcall ~TResFlowSup(void);
		void          __fastcall  CopiaAtributosDe(VTResFlowSup &ref);
		bool          __fastcall  ExisteDiagnostico(int tipo_diag);
      void          __fastcall  Inicia(int np);
		void          __fastcall  Reinicia(void);

   private: //métodos acessados via property
      //Eqbar associado
      VTEqbar*        __fastcall PM_GetEqbar(void);
      //tipo de suprimento
      int             __fastcall PM_GetTipoSup(void);
      void            __fastcall PM_SetTipoSup(int tipo_sup);
      //carregamento (%)
      double          __fastcall PM_GetCargto(int np);
      //carregamento (pu)
      double          __fastcall PM_GetCargto_pu(int np);
      void            __fastcall PM_SetCargto_pu(int np, double valor);
      //diagnóstico
      int             __fastcall PM_GetDiagnostico(int np);
      void            __fastcall PM_SetDiagnostico(int np, int valor);
      //número de patamares
      int             __fastcall PM_GetMaxPat(void);
      void            __fastcall PM_SetMaxPat(int max_pat);
      //sentido do fluxo
      int             __fastcall PM_GetSentidoP(int np);
      int             __fastcall PM_GetSentidoQ(int np);
      void            __fastcall PM_SetSentidoP(int np, int valor);
      void            __fastcall PM_SetSentidoQ(int np, int valor);
      //potência total
      double          __fastcall PM_GetP_mw(int np);
      double          __fastcall PM_GetP_mw_max(int np);
      double          __fastcall PM_GetP_mw_min(int np);
      double          __fastcall PM_GetQ_mvar(int np);
      double          __fastcall PM_GetQ_mvar_max(int np);
      double          __fastcall PM_GetQ_mvar_min(int np);
      complex<double> __fastcall PM_GetS_mva(int np);
      void            __fastcall PM_SetS_mva(int np, complex<double> s_mva);
      //potência de fase
      double          __fastcall PM_GetPa_mw(int np);
      double          __fastcall PM_GetPb_mw(int np);
      double          __fastcall PM_GetPc_mw(int np);
      double          __fastcall PM_GetPf_mw(int np, int nf);
      double          __fastcall PM_GetPf_mw_max(int np);
      double          __fastcall PM_GetPf_mw_min(int np);
      double          __fastcall PM_GetQa_mvar(int np);
      double          __fastcall PM_GetQb_mvar(int np);
      double          __fastcall PM_GetQc_mvar(int np);
      double          __fastcall PM_GetQf_mvar(int np, int nf);
      double          __fastcall PM_GetQf_mvar_max(int np);
      double          __fastcall PM_GetQf_mvar_min(int np);
      complex<double> __fastcall PM_GetSa_mva(int np);
      complex<double> __fastcall PM_GetSb_mva(int np);
      complex<double> __fastcall PM_GetSc_mva(int np);
      complex<double> __fastcall PM_GetSf_mva(int np, int nf);
      double          __fastcall PM_GetSf_mva_max(int np);
      double          __fastcall PM_GetSf_mva_min(int np);
      void            __fastcall PM_SetSa_mva(int np, complex<double> s_mva);
      void            __fastcall PM_SetSb_mva(int np, complex<double> s_mva);
      void            __fastcall PM_SetSc_mva(int np, complex<double> s_mva);
      void            __fastcall PM_SetSf_mva(int np, int nf, complex<double> s_mva);
      //potência de neutro
      double          __fastcall PM_GetPn_mw(int np);
      double          __fastcall PM_GetQn_mvar(int np);
      complex<double> __fastcall PM_GetSn_mva(int np);
      void            __fastcall PM_SetSn_mva(int np, complex<double> s_mva);
      //fator de potência de fase
      double          __fastcall PM_GetFPa(int np);
      double          __fastcall PM_GetFPb(int np);
      double          __fastcall PM_GetFPc(int np);
      double          __fastcall PM_GetFPf(int np, int nf);
      double          __fastcall PM_GetFPf_max(int np);
      double          __fastcall PM_GetFPf_min(int np);
      //fator de potência de neutro
      double          __fastcall PM_GetFPn(int np);
      //corrente de fase
      complex<double> __fastcall PM_GetIa_amp(int np);
      complex<double> __fastcall PM_GetIb_amp(int np);
      complex<double> __fastcall PM_GetIc_amp(int np);
      complex<double> __fastcall PM_GetIf_amp(int np, int nf);
      double          __fastcall PM_GetIf_amp_max(int np);
      double          __fastcall PM_GetIf_amp_min(int np);
      //corrente de neutro
      complex<double> __fastcall PM_GetIn_amp(int np);

   private: //métodos
      complex<double> __fastcall Corrente_amp(complex<double>sf_mva, complex<double>vf_kv);
      double          __fastcall FatorPotencia(complex<double> s);
      void            __fastcall VetorDelete(void);
      void            __fastcall VetorNew(void);

   private: //objetos externos

   protected: //dados acessados via property
      struct   {
               VTEqbar *eqbar;
               int      max_pat;
               int      tipo_sup;
               } PD;

   private: //dados locais
      int    *diag;    //diagnóstico
      float  *cargto;  //carregamento
      struct strPOTENCIA {
                         complex<float> sa, sb, sc, sn; //MVA
                         } *potencia;
   };

#endif
//---------------------------------------------------------------------------
//eof

