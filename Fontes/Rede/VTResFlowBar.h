//---------------------------------------------------------------------------
#ifndef VTResFlowBarH
#define VTResFlowBarH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>

//---------------------------------------------------------------------------
class VTBarra;

//---------------------------------------------------------------------------
class VTResFlowBar : public TObject
   {
   public:  //property
      __property VTBarra*        Barra                  = {read=PM_GetBarra};
      __property int             Max_pat                = {read=PM_GetMaxPat,      write=PM_SetMaxPat};
      __property int             Fases                  = {read=PM_GetFases,       write=PM_SetFases };
      //diagnóstico
      __property int             Diagnostico[int np]    = {read=PM_GetDiagnostico, write=PM_SetDiagnostico};
      //desequilíbrio (%)
      __property double          Desequilibrio[int np]  = {read=PM_GetDesequilibrio};
      //índices ANEEL
      __property double          DRA                    = {read=PM_GetDRA};
      __property double          DRC                    = {read=PM_GetDRC};
      __property double          DRP                    = {read=PM_GetDRP};
      //-----------------------------Tensões em pu
      //tensões de fase/neutro em pu
      __property complex<double> Van_pu[int np]         = {read=PM_GetVan_pu, write=PM_SetVan_pu};
      __property complex<double> Vbn_pu[int np]         = {read=PM_GetVbn_pu, write=PM_SetVbn_pu};
      __property complex<double> Vcn_pu[int np]         = {read=PM_GetVcn_pu, write=PM_SetVcn_pu};
      __property complex<double> Vfn_pu[int np][int nf] = {read=PM_GetVfn_pu, write=PM_SetVfn_pu};
      __property double          Vfn_pu_max[int np]     = {read=PM_GetVfn_pu_max};
      __property double          Vfn_pu_min[int np]     = {read=PM_GetVfn_pu_min};
      //tensões neutro/terra em pu
      __property complex<double> Vnt_pu[int np]         = {read=PM_GetVnt_pu, write=PM_SetVnt_pu};
      //tensões fase/terra em pu
      __property complex<double> Vat_pu[int np]         = {read=PM_GetVat_pu};
      __property complex<double> Vbt_pu[int np]         = {read=PM_GetVbt_pu};
      __property complex<double> Vct_pu[int np]         = {read=PM_GetVct_pu};
      __property complex<double> Vft_pu[int np][int nf] = {read=PM_GetVft_pu};
      __property double          Vft_pu_max[int np]     = {read=PM_GetVft_pu_max};
      __property double          Vft_pu_min[int np]     = {read=PM_GetVft_pu_min};
      //tensões de linha em pu
      __property complex<double> Vab_pu[int np]         = {read=PM_GetVab_pu};
      __property complex<double> Vbc_pu[int np]         = {read=PM_GetVbc_pu};
      __property complex<double> Vca_pu[int np]         = {read=PM_GetVca_pu};
      __property complex<double> Vff_pu[int np][int nf] = {read=PM_GetVff_pu};
      __property double          Vff_pu_max[int np]     = {read=PM_GetVff_pu_max};
      __property double          Vff_pu_min[int np]     = {read=PM_GetVff_pu_min};
      //-----------------------------Tensões em kV
      //tensões de fase/neutro em kv
      __property complex<double> Van_kv[int np]         = {read=PM_GetVan_kv};
      __property complex<double> Vbn_kv[int np]         = {read=PM_GetVbn_kv};
      __property complex<double> Vcn_kv[int np]         = {read=PM_GetVcn_kv};
      __property complex<double> Vfn_kv[int np][int nf] = {read=PM_GetVfn_kv};
      __property double          Vfn_kv_max[int np]     = {read=PM_GetVfn_kv_max};
      __property double          Vfn_kv_min[int np]     = {read=PM_GetVfn_kv_min};
      //tensões neutro/terra em kv
      __property complex<double> Vnt_kv[int np]         = {read=PM_GetVnt_kv};
      //tensões fase/terra em kv
      __property complex<double> Vat_kv[int np]         = {read=PM_GetVat_kv};
      __property complex<double> Vbt_kv[int np]         = {read=PM_GetVbt_kv};
      __property complex<double> Vct_kv[int np]         = {read=PM_GetVct_kv};
      __property complex<double> Vft_kv[int np][int nf] = {read=PM_GetVft_kv};
      __property double          Vft_kv_max[int np]     = {read=PM_GetVft_kv_max};
      __property double          Vft_kv_min[int np]     = {read=PM_GetVft_kv_min};
      //tensões de linha em kv
      __property complex<double> Vab_kv[int np]         = {read=PM_GetVab_kv};
      __property complex<double> Vbc_kv[int np]         = {read=PM_GetVbc_kv};
      __property complex<double> Vca_kv[int np]         = {read=PM_GetVca_kv};
      __property complex<double> Vff_kv[int np][int nf] = {read=PM_GetVff_kv};
      __property double          Vff_kv_max[int np]     = {read=PM_GetVff_kv_max};
      __property double          Vff_kv_min[int np]     = {read=PM_GetVff_kv_min};
      //
      //corrente injetada por fase
      __property complex<double> Ia_amp[int np]         = {read=PM_GetIa_amp, write=PM_SetIa_amp};
      __property complex<double> Ib_amp[int np]         = {read=PM_GetIb_amp, write=PM_SetIb_amp};
      __property complex<double> Ic_amp[int np]         = {read=PM_GetIc_amp, write=PM_SetIc_amp};
      __property complex<double> In_amp[int np]         = {read=PM_GetIn_amp, write=PM_SetIn_amp};
      __property complex<double> If_amp[int np][int nf] = {read=PM_GetIf_amp, write=PM_SetIf_amp};
      //corrente injetada por fase
      __property complex<double> Sf_mva[int np][int nf] = {read=PM_GetSf_mva};

   public:  //funções públicas
									 __fastcall  VTResFlowBar(void) {};
		virtual               __fastcall ~VTResFlowBar(void) {};
		virtual void          __fastcall  CopiaAtributosDe(VTResFlowBar &ref) = 0;
		virtual bool          __fastcall  ExisteDiagnostico(int tipo_diag) = 0;
      virtual void          __fastcall  Inicia(int np) = 0;
		//virtual VTResFlowBar& __fastcall  operator<<(VTResFlowBar &ref) = 0;
      virtual void          __fastcall  Reinicia(void) = 0;
      virtual double        __fastcall  Vfn_pu_max_allpat(void) = 0;
      virtual double        __fastcall  Vfn_pu_min_allpat(void) = 0;

   protected: //funções acessadas via property
      //barra associada
      virtual VTBarra*        __fastcall PM_GetBarra(void) = 0;
      virtual int             __fastcall PM_GetFases(void) = 0;
      virtual void            __fastcall PM_SetFases(int fases) = 0;
      //desequilíbrio (%)
      virtual double          __fastcall PM_GetDesequilibrio(int np) = 0;  // (%)
      //diagnóstico
      virtual int             __fastcall PM_GetDiagnostico(int np) = 0;
      virtual void            __fastcall PM_SetDiagnostico(int np, int valor) = 0;
      //índices ANELL
      virtual double          __fastcall PM_GetDRA(void) = 0;
      virtual double          __fastcall PM_GetDRC(void) = 0;
      virtual double          __fastcall PM_GetDRP(void) = 0;
      //número de patamares
      virtual int             __fastcall PM_GetMaxPat(void) = 0;
      virtual void            __fastcall PM_SetMaxPat(int max_pat) = 0;
      //tensões de fase/neutro em pu
      virtual complex<double> __fastcall PM_GetVan_pu(int np) = 0;
      virtual complex<double> __fastcall PM_GetVbn_pu(int np) = 0;
      virtual complex<double> __fastcall PM_GetVcn_pu(int np) = 0;
      virtual complex<double> __fastcall PM_GetVfn_pu(int np, int nf) = 0;
      virtual double          __fastcall PM_GetVfn_pu_max(int np) = 0;
      virtual double          __fastcall PM_GetVfn_pu_min(int np) = 0;
      virtual void            __fastcall PM_SetVan_pu(int np, complex<double> v_pu) = 0;
      virtual void            __fastcall PM_SetVbn_pu(int np, complex<double> v_pu) = 0;
      virtual void            __fastcall PM_SetVcn_pu(int np, complex<double> v_pu) = 0;
      virtual void            __fastcall PM_SetVfn_pu(int np, int nf, complex<double> v_pu) = 0;
      //tensões neutro/terra em pu
      virtual complex<double> __fastcall PM_GetVnt_pu(int np) = 0;
      virtual void            __fastcall PM_SetVnt_pu(int np, complex<double> v_pu) = 0;
      //tensões fase/terra em pu
      virtual complex<double> __fastcall PM_GetVat_pu(int np) = 0;
      virtual complex<double> __fastcall PM_GetVbt_pu(int np) = 0;
      virtual complex<double> __fastcall PM_GetVct_pu(int np) = 0;
      virtual complex<double> __fastcall PM_GetVft_pu(int np, int nf) = 0;
      virtual double          __fastcall PM_GetVft_pu_max(int np) = 0;
      virtual double          __fastcall PM_GetVft_pu_min(int np) = 0;
      //tensões de linha em pu
      virtual complex<double> __fastcall PM_GetVab_pu(int np) = 0;
      virtual complex<double> __fastcall PM_GetVbc_pu(int np) = 0;
      virtual complex<double> __fastcall PM_GetVca_pu(int np) = 0;
      virtual complex<double> __fastcall PM_GetVff_pu(int np, int nf) = 0;
      virtual double          __fastcall PM_GetVff_pu_max(int np) = 0;
      virtual double          __fastcall PM_GetVff_pu_min(int np) = 0;
      //tensões de fase/neutro em kV
      virtual complex<double> __fastcall PM_GetVan_kv(int np) = 0;
      virtual complex<double> __fastcall PM_GetVbn_kv(int np) = 0;
      virtual complex<double> __fastcall PM_GetVcn_kv(int np) = 0;
      virtual complex<double> __fastcall PM_GetVfn_kv(int np, int nf) = 0;
      virtual double          __fastcall PM_GetVfn_kv_max(int np) = 0;
      virtual double          __fastcall PM_GetVfn_kv_min(int np) = 0;
      //tensões neutro/terra em kV
      virtual complex<double> __fastcall PM_GetVnt_kv(int np) = 0;
      //tensões fase/terra em kV
      virtual complex<double> __fastcall PM_GetVat_kv(int np) = 0;
      virtual complex<double> __fastcall PM_GetVbt_kv(int np) = 0;
      virtual complex<double> __fastcall PM_GetVct_kv(int np) = 0;
      virtual complex<double> __fastcall PM_GetVft_kv(int np, int nf) = 0;
      virtual double          __fastcall PM_GetVft_kv_max(int np) = 0;
      virtual double          __fastcall PM_GetVft_kv_min(int np) = 0;
      //tensões de linha em kV
      virtual complex<double> __fastcall PM_GetVab_kv(int np) = 0;
      virtual complex<double> __fastcall PM_GetVbc_kv(int np) = 0;
      virtual complex<double> __fastcall PM_GetVca_kv(int np) = 0;
      virtual complex<double> __fastcall PM_GetVff_kv(int np, int nf) = 0;
      virtual double          __fastcall PM_GetVff_kv_max(int np) = 0;
      virtual double          __fastcall PM_GetVff_kv_min(int np) = 0;
      //correntes injetada por fase
      virtual complex<double> __fastcall PM_GetIa_amp(int np) = 0;
      virtual complex<double> __fastcall PM_GetIb_amp(int np) = 0;
      virtual complex<double> __fastcall PM_GetIc_amp(int np) = 0;
      virtual complex<double> __fastcall PM_GetIn_amp(int np) = 0;
      virtual void            __fastcall PM_SetIa_amp(int np, complex<double> i_amp) = 0;
      virtual void            __fastcall PM_SetIb_amp(int np, complex<double> i_amp) = 0;
      virtual void            __fastcall PM_SetIc_amp(int np, complex<double> i_amp) = 0;
      virtual void            __fastcall PM_SetIn_amp(int np, complex<double> i_amp) = 0;
      //correntes injetada por fase
      virtual complex<double> __fastcall PM_GetIf_amp(int np, int nf) = 0;
      virtual void            __fastcall PM_SetIf_amp(int np, int nf, complex<double> i_amp) = 0;
      //potência injetada por fase
      virtual complex<double> __fastcall PM_GetSf_mva(int np, int nf) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTResFlowBar
//---------------------------------------------------------------------------
VTResFlowBar* __fastcall NewObjResFlowBar(VTBarra *pbarra, int num_pat);

//---------------------------------------------------------------------------
#endif
//eof
