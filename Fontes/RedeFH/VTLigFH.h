//---------------------------------------------------------------------------
#ifndef VTLigFHH
#define VTLigFHH

//---------------------------------------------------------------------------
using namespace std;

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <complex>
//#include "..\Rede\VTLigacao.h"
class VTEqpto;
class VTLigacao;

//---------------------------------------------------------------------------
class VTLigFH : public TObject
   {
   public:   //property
      __property VTEqpto *       BarRef                                             = {read=PM_GetBarRef,    write=PM_SetBarRef};
      __property double          DH_Corr[int ind_pat][int ind_fase][int ind_freq]   = {read=PM_GetDH_Corr,   write=PM_SetDH_Corr};
      __property double          DHT_Corr[int ind_pat][int ind_fase]                = {read=PM_GetDHT_Corr,  write=PM_SetDHT_Corr};
      __property complex<double> Iamp[int ind_pat][int ind_fase][int ind_freq]      = {read=PM_GetIamp,      write=PM_SetIamp};
      __property double          Ief_amp[int ind_pat][int ind_fase]                 = {read=PM_GetIef_amp,   write=PM_SetIef_amp};
      __property VTLigacao *     Ligacao                                            = {read=PM_GetLigacao};
      __property complex<double> Perda_kva[int ind_pat][int ind_fase][int ind_freq] = {read=PM_GetPerda_kva, write=PM_SetPerda_kva};

   public:
                   __fastcall  VTLigFH(void) {};
      virtual      __fastcall ~VTLigFH(void) {};
      virtual void __fastcall  ReiniciaValores(void) = 0;

   protected:   //métodos acessados via property
      virtual VTEqpto *       __fastcall PM_GetBarRef(void) = 0;
      virtual double          __fastcall PM_GetDH_Corr(int ind_pat, int ind_fase, int ind_freq) = 0;
      virtual double          __fastcall PM_GetDHT_Corr(int ind_pat, int ind_fase) = 0;
      virtual complex<double> __fastcall PM_GetIamp(int ind_pat, int ind_fase, int ind_freq) = 0;
      virtual double          __fastcall PM_GetIef_amp(int ind_pat, int ind_fase) = 0;
      virtual VTLigacao *     __fastcall PM_GetLigacao(void) = 0;
      virtual complex<double> __fastcall PM_GetPerda_kva(int ind_pat, int ind_fase, int indfreq) = 0;
      //
      virtual void            __fastcall PM_SetBarRef(VTEqpto *bar_ref) = 0;
      virtual void            __fastcall PM_SetDH_Corr(int ind_pat, int ind_fase, int ind_freq, double dh) = 0;
      virtual void            __fastcall PM_SetDHT_Corr(int ind_pat, int ind_fase, double dht) = 0;
      virtual void            __fastcall PM_SetIamp(int ind_pat, int ind_fase, int ind_freq, complex<double> iamp) = 0;
      virtual void            __fastcall PM_SetIef_amp(int ind_pat, int ind_fase, double ief_amp) = 0;
      virtual void            __fastcall PM_SetPerda_kva(int ind_pat, int ind_fase, int ind_freq, complex<double> perda_kva) = 0;
   };

//-----------------------------------------------------------------------------
VTLigFH* __fastcall NewObjLigFH(VTLigacao *ligacao,
                                int       num_pat,
                                int       num_freq);

#endif
//-----------------------------------------------------------------------------
// eof
