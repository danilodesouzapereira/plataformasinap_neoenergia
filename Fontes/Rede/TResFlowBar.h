//---------------------------------------------------------------------------
#ifndef TResFlowBarH
#define TResFlowBarH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTResFlowBar.h"

//---------------------------------------------------------------------------
class TResFlowBar : public VTResFlowBar
   {
   public:
                    __fastcall  TResFlowBar(VTBarra *pbarra, int num_pat);
                    __fastcall ~TResFlowBar(void);
		void          __fastcall  CopiaAtributosDe(VTResFlowBar &ref);
		bool          __fastcall  ExisteDiagnostico(int tipo_diag);
      void          __fastcall  Inicia(int np);
		void          __fastcall  Reinicia(void);
      double        __fastcall  Vfn_pu_max_allpat(void);
      double        __fastcall  Vfn_pu_min_allpat(void);

   private: //métodos acessados via property
      //barra associada
      VTBarra*        __fastcall PM_GetBarra(void);
      int             __fastcall PM_GetFases(void);
      void            __fastcall PM_SetFases(int fases);
      //desequilíbrio (%)
      double          __fastcall PM_GetDesequilibrio(int np);
      //diagnóstico
      int             __fastcall PM_GetDiagnostico(int np);
      void            __fastcall PM_SetDiagnostico(int np, int valor);
      //índices ANELL
      double          __fastcall PM_GetDRA(void);
      double          __fastcall PM_GetDRC(void);
      double          __fastcall PM_GetDRP(void);
      //número de patamares
      int             __fastcall PM_GetMaxPat(void);
      void            __fastcall PM_SetMaxPat(int max_pat);
      //tensões de fase/neutro em pu
      complex<double> __fastcall PM_GetVan_pu(int np);
      complex<double> __fastcall PM_GetVbn_pu(int np);
      complex<double> __fastcall PM_GetVcn_pu(int np);
      complex<double> __fastcall PM_GetVfn_pu(int np, int nf);
      double          __fastcall PM_GetVfn_pu_max(int np);
      double          __fastcall PM_GetVfn_pu_min(int np);
      void            __fastcall PM_SetVan_pu(int np, complex<double> v_pu);
      void            __fastcall PM_SetVbn_pu(int np, complex<double> v_pu);
      void            __fastcall PM_SetVcn_pu(int np, complex<double> v_pu);
      void            __fastcall PM_SetVfn_pu(int np, int nf, complex<double> v_pu);
      //tensões neutro/terra em pu
      complex<double> __fastcall PM_GetVnt_pu(int np);
      void            __fastcall PM_SetVnt_pu(int np, complex<double> v_pu);
      //tensões fase/terra em pu
      complex<double> __fastcall PM_GetVat_pu(int np);
      complex<double> __fastcall PM_GetVbt_pu(int np);
      complex<double> __fastcall PM_GetVct_pu(int np);
      complex<double> __fastcall PM_GetVft_pu(int np, int nf);
      double          __fastcall PM_GetVft_pu_max(int np);
      double          __fastcall PM_GetVft_pu_min(int np);
      //tensões de linha em pu
      complex<double> __fastcall PM_GetVab_pu(int np);
      complex<double> __fastcall PM_GetVbc_pu(int np);
      complex<double> __fastcall PM_GetVca_pu(int np);
      complex<double> __fastcall PM_GetVff_pu(int np, int nf);
      double          __fastcall PM_GetVff_pu_max(int np);
      double          __fastcall PM_GetVff_pu_min(int np);
      //tensões de fase/neutro em kV
      complex<double> __fastcall PM_GetVan_kv(int np);
      complex<double> __fastcall PM_GetVbn_kv(int np);
      complex<double> __fastcall PM_GetVcn_kv(int np);
      complex<double> __fastcall PM_GetVfn_kv(int np, int nf);
      double          __fastcall PM_GetVfn_kv_max(int np);
      double          __fastcall PM_GetVfn_kv_min(int np);
      //tensões neutro/terra em kV
      complex<double> __fastcall PM_GetVnt_kv(int np);
      //tensões fase/terra em kV
      complex<double> __fastcall PM_GetVat_kv(int np);
      complex<double> __fastcall PM_GetVbt_kv(int np);
      complex<double> __fastcall PM_GetVct_kv(int np);
      complex<double> __fastcall PM_GetVft_kv(int np, int nf);
      double          __fastcall PM_GetVft_kv_max(int np);
      double          __fastcall PM_GetVft_kv_min(int np);
      //tensões de linha em kV
      complex<double> __fastcall PM_GetVab_kv(int np);
      complex<double> __fastcall PM_GetVbc_kv(int np);
      complex<double> __fastcall PM_GetVca_kv(int np);
      complex<double> __fastcall PM_GetVff_kv(int np, int nf);
      double          __fastcall PM_GetVff_kv_max(int np);
      double          __fastcall PM_GetVff_kv_min(int np);
      //
      //correntes injetada por fase A,B,C,N
      complex<double> __fastcall PM_GetIa_amp(int np);
      complex<double> __fastcall PM_GetIb_amp(int np);
      complex<double> __fastcall PM_GetIc_amp(int np);
      complex<double> __fastcall PM_GetIn_amp(int np);
      void            __fastcall PM_SetIa_amp(int np, complex<double> i_amp);
      void            __fastcall PM_SetIb_amp(int np, complex<double> i_amp);
      void            __fastcall PM_SetIc_amp(int np, complex<double> i_amp);
      void            __fastcall PM_SetIn_amp(int np, complex<double> i_amp);
      //correntes injetada por fase
      complex<double> __fastcall PM_GetIf_amp(int np, int nf);
      void            __fastcall PM_SetIf_amp(int np, int nf, complex<double> i_amp);
      //potência injetada por fase
      complex<double> __fastcall PM_GetSf_mva(int np, int nf);

   private: //métodos
      complex<double> __fastcall Tensao_pu(int np, int nf);
      void            __fastcall VetorDelete(void);
      void            __fastcall VetorNew(void);

   protected: //dados acessados via property
      struct   {
               VTBarra *pbarra;
               int     max_pat;
               int     fases;
               } PD;

   private: //objetos externos

   private: //dados locais
      int    *diag;
      struct strTENSAO  {//organizado como vet[Max_pat]
                        //complex<float> van, vbn, vcn, vnt; //pu
                        complex<double> van, vbn, vcn, vnt; //pu
                        } *tensao;
      struct strCORRENTE {//organizado como vet[Max_pat]
                         complex<float> ia_amp, ib_amp, ic_amp, in_amp;
                         } *corrente;
   };

#endif
//---------------------------------------------------------------------------
//eof

