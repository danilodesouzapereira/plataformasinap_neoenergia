//---------------------------------------------------------------------------
#ifndef VTBarFHH
#define VTBarFHH

//---------------------------------------------------------------------------
using namespace std;

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <complex>
//#include "..\Rede\VTBarra.h"
class VTBarra;

//---------------------------------------------------------------------------
class VTBarFH : public TObject
   {
   public:   //property
		__property VTBarra *       Barra                                              = {read=PM_GetBarra,      write=PM_SetBarra};
      __property double          DH_Corr[int ind_pat][int ind_fase][int ind_freq]   = {read=PM_GetDH_Corr,    write=PM_SetDH_Corr};
      __property double          DH_Tensao[int ind_pat][int ind_fase][int ind_freq] = {read=PM_GetDH_Tensao,  write=PM_SetDH_Tensao};
      __property double          DHT_Corr[int ind_pat][int ind_fase]                = {read=PM_GetDHT_Corr,   write=PM_SetDHT_Corr};
      __property double          DHT_Tensao[int ind_pat][int ind_fase]              = {read=PM_GetDHT_Tensao, write=PM_SetDHT_Tensao};
      __property complex<double> Iamp[int ind_pat][int ind_fase][int ind_freq]      = {read=PM_GetIamp,       write=PM_SetIamp};
      __property double          Ief_amp[int ind_pat][int ind_fase]                 = {read=PM_GetIef_amp,    write=PM_SetIef_amp};
      __property double          Vef_pu[int ind_pat][int ind_fase]                  = {read=PM_GetVef_pu,     write=PM_SetVef_pu};
      __property complex<double> Vpu[int ind_pat][int ind_fase][int ind_freq]       = {read=PM_GetVpu,        write=PM_SetVpu};

   public:
                   __fastcall  VTBarFH(void)  {};
      virtual      __fastcall ~VTBarFH(void) {};
      virtual void __fastcall  ReiniciaValores(void) = 0;

   protected:   //métodos acessados via property
      virtual VTBarra *       __fastcall PM_GetBarra(void) = 0;
      virtual double          __fastcall PM_GetDH_Corr(int ind_pat, int ind_fase, int ind_freq) = 0;
      virtual double          __fastcall PM_GetDH_Tensao(int ind_pat, int ind_fase, int ind_freq) = 0;
      virtual double          __fastcall PM_GetDHT_Corr(int ind_pat, int ind_fase) = 0;
      virtual double          __fastcall PM_GetDHT_Tensao(int ind_pat, int ind_fase) = 0;
      virtual complex<double> __fastcall PM_GetIamp(int ind_pat, int ind_fase, int ind_freq) = 0;
      virtual double          __fastcall PM_GetIef_amp(int ind_pat, int ind_fase) = 0;
      virtual double          __fastcall PM_GetVef_pu(int ind_pat, int ind_fase) = 0;
      virtual complex<double> __fastcall PM_GetVpu(int ind_pat, int ind_fase, int indfreq) = 0;
      //
		virtual void            __fastcall PM_SetBarra(VTBarra *barra) = 0;
      virtual void            __fastcall PM_SetDH_Corr(int ind_pat, int ind_fase, int ind_freq, double dh) = 0;
      virtual void            __fastcall PM_SetDH_Tensao(int ind_pat, int ind_fase, int ind_freq, double dh) = 0;
      virtual void            __fastcall PM_SetDHT_Corr(int ind_pat, int ind_fase, double dht) = 0;
      virtual void            __fastcall PM_SetDHT_Tensao(int ind_pat, int ind_fase, double dht) = 0;
      virtual void            __fastcall PM_SetIamp(int ind_pat, int ind_fase, int ind_freq, complex<double> iamp) = 0;
      virtual void            __fastcall PM_SetIef_amp(int ind_pat, int ind_fase, double ief_amp) = 0;
      virtual void            __fastcall PM_SetVef_pu(int ind_pat, int ind_fase, double vef_pu) = 0;
      virtual void            __fastcall PM_SetVpu(int ind_pat, int ind_fase, int ind_freq, complex<double> vpu) = 0;
   };

//-----------------------------------------------------------------------------
VTBarFH* __fastcall NewObjBarFH(VTBarra *barra,
                                int     num_pat,
                                int     num_freq);

#endif
//-----------------------------------------------------------------------------
// eof
