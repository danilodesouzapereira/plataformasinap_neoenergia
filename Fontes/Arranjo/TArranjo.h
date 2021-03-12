//---------------------------------------------------------------------------
#ifndef TArranjoH
#define TArranjoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTArranjo.h"

//---------------------------------------------------------------------------
class TArranjo : public VTArranjo
   {
   protected: //dados acessados via property
      struct   {
               int     tipo_cfg;         //ver enumCFG_ARRANJO
               int     fases;            //ver enumFASE em VTFases.h
               double  iadm_amp;         //A
               double  vmin_kv, vmax_kv; //limites de tensão em kV
               double  res_terra;        //resistividade de terra (ohm*m)
               TColor  color;            //cor p/ apresentação gráfica
               TDate   data;
               VTEqpto *owner;
               } PD;

   private: //dados locais
      VTMatZ    *matZ;          //matriz de impedâncias  (ohm/km)
      VTMatC    *matC;          //matriz de capacitâncias(nF/km)

   public:
                 __fastcall TArranjo(VTEqpto *owner);
                 __fastcall ~TArranjo(void);
      VTArranjo* __fastcall  Clone(void);
      void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
      void       __fastcall  DefCaboFase(int ind_fase, VTCabo *cabo, int multiplicidade);
      void       __fastcall  DefCaboZ0Z1(VTCabo *cabo);
      void       __fastcall  DefC0C1_nF_km(double c0, double c1);
      void       __fastcall  DefC0C1_pu(double Sbase, double Vbase, double comp, double yc0_pu, double yc1_pu);
      void       __fastcall  DefPtoFixacaoFase(int ind_fase, VTPtoFixacao *pto_fixacao);
      void       __fastcall  DefSuporte(VTSuporte *suporte);
      void       __fastcall  DefZ0Z1_ohm_km(strIMP z0, strIMP z1);
      void       __fastcall  DefZ0Z1_pu(double Sbase, double Vbase, double comp, strIMP z0_pu, strIMP z1_pu);
      double     __fastcall  MaiorCapacitancia(int seq);
      bool       __fastcall  TemCapacitancia(void);
      int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);
      AnsiString __fastcall  TipoDeArranjo(void);
      bool       __fastcall  Z0_ohm(double comp, strIMP &z0_ohm, double &yc0_mho);
      bool       __fastcall  Z0_pu (double Sbase, double Vbase, double comp, strIMP &z0_pu,  double &yc0_pu);
      bool       __fastcall  Z1_ohm(double comp, strIMP &z1_ohm, double &yc1_mho);
      bool       __fastcall  Z1_pu (double Sbase, double Vbase, double comp, strIMP &z1_pu,  double &yc1_pu);

   protected: //métodos das classe base redefinidos
      void      __fastcall PM_SetCodigo(AnsiString codigo);

   protected: //métodos acessados via property
      TColor          __fastcall PM_GetColor(void);
      TDate           __fastcall PM_GetData(void);
      int             __fastcall PM_GetFases(void);
      double          __fastcall PM_GetIadm_amp(void);
      double          __fastcall PM_GetIfase_amp(int nf);
      VTMatC*         __fastcall PM_GetMatC(void);
      VTMatZ*         __fastcall PM_GetMatZ(void);
      int             __fastcall PM_GetNumeroCabos(void);
      VTEqpto*        __fastcall PM_GetOwner(void);
      double          __fastcall PM_GetResTerra(void);
      int             __fastcall PM_GetTipoCfg(void);
      double          __fastcall PM_GetVmax_KV(void);
      double          __fastcall PM_GetVmin_KV(void);
      double          __fastcall PM_GetYcap_siemens_km(int i, int j);
      complex<double> __fastcall PM_GetZ_ohm_km(int i, int j);
      void            __fastcall PM_SetColor(TColor color);
      void            __fastcall PM_SetData(TDate data);
      void            __fastcall PM_SetFases(int fases);
      void            __fastcall PM_SetIadm_amp(double i_amp);
      void            __fastcall PM_SetOwner(VTEqpto *owner);
      void            __fastcall PM_SetResTerra(double res_terra);
      void            __fastcall PM_SetTipoCfg(int tipo_cfg);
      void            __fastcall PM_SetVmax_KV(double vmax);
      void            __fastcall PM_SetVmin_KV(double vmin);


   private: //métodos
      double          __fastcall Calcula_C0(double c3x3[3][3]);
      double          __fastcall Calcula_C1(double c3x3[3][3]);
      complex<double> __fastcall Calcula_Z0(complex<double> z3x3[3][3]);
      complex<double> __fastcall Calcula_Z1(complex<double> z3x3[3][3]);
      void            __fastcall MontaMatrizC3x3(double c3x3[3][3]);
      void            __fastcall MontaMatrizZ3x3(complex<double> zaux[3][3]);
      void            __fastcall EliminaNeutro(complex<double> z4x4[4][4], complex<double> z3x3[3][3]);
      double          __fastcall Ycap_siemens_km(double cap_nF_km);
   };
                       
//---------------------------------------------------------------------------
#endif
//eof

