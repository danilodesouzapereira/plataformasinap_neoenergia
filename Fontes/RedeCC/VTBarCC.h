//---------------------------------------------------------------------------
#ifndef VTBarCCH
#define VTBarCCH

//---------------------------------------------------------------------------
using namespace std;

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "..\Rede\VTBarra.h"

//---------------------------------------------------------------------------
class VTBarCC : public TObject
   {
   public:   //property
      __property VTBarra*        Barra                                    = {read=PM_GetBarra,          write=PM_SetBarra};
      __property complex<double> Assim_Ifas_amp[int ind_def][int ind_fas] = {read=PM_GetAssim_Ifas_amp, write=PM_SetAssim_Ifas_amp};
      __property complex<double> Ifas_amp[int ind_def][int ind_fas]       = {read=PM_GetIfas_amp,       write=PM_SetIfas_amp};
      __property complex<double> Iseq_amp[int ind_def][int ind_seq]       = {read=PM_GetIseq_amp,       write=PM_SetIseq_amp};
      __property complex<double> S_mva[int ind_def]                       = {read=PM_GetS_mva};
      __property complex<double> Vfas_kv[int ind_def][int ind_fas]        = {read=PM_GetVfas_kv};
      __property complex<double> Vfas_pu[int ind_def][int ind_fas]        = {read=PM_GetVfas_pu,        write=PM_SetVfas_pu};
      __property complex<double> Vseq_kv[int ind_def][int ind_seq]        = {read=PM_GetVseq_kv};
      __property complex<double> Vseq_pu[int ind_def][int ind_seq]        = {read=PM_GetVseq_pu,        write=PM_SetVseq_pu};

   public:
                   __fastcall  VTBarCC(void)  {};
      virtual      __fastcall ~VTBarCC(void) {};
      virtual void __fastcall  ReiniciaValores(void) = 0;
      virtual bool __fastcall  Vfase_pu(int ind_def, int fases_def, double v_pu[MAX_FASE]) = 0;

   protected:   //métodos acessados via property
      virtual VTBarra*        __fastcall PM_GetBarra(void) = 0;
      virtual complex<double> __fastcall PM_GetAssim_Ifas_amp(int ind_def, int ind_fas) = 0;
      virtual complex<double> __fastcall PM_GetIfas_amp(int ind_def, int ind_fas) = 0;
      virtual complex<double> __fastcall PM_GetIseq_amp(int ind_def, int ind_seq) = 0;
      virtual complex<double> __fastcall PM_GetS_mva(int ind_def) = 0;
      virtual complex<double> __fastcall PM_GetVfas_kv(int ind_def, int ind_fas) = 0;
      virtual complex<double> __fastcall PM_GetVfas_pu(int ind_def, int ind_fas) = 0;
      virtual complex<double> __fastcall PM_GetVseq_kv(int ind_def, int ind_seq) = 0;
      virtual complex<double> __fastcall PM_GetVseq_pu(int ind_def, int ind_seq) = 0;
      virtual void            __fastcall PM_SetBarra(VTBarra *barra) = 0;
      virtual void            __fastcall PM_SetAssim_Ifas_amp(int ind_def, int ind_fas, complex<double> ifas) = 0;
      virtual void            __fastcall PM_SetIfas_amp(int ind_def, int ind_fas, complex<double> ifas) = 0;
      virtual void            __fastcall PM_SetIseq_amp(int ind_def, int ind_seq, complex<double> iseq) = 0;
      virtual void            __fastcall PM_SetVfas_pu(int ind_def, int ind_fas, complex<double> vfas) = 0;
      virtual void            __fastcall PM_SetVseq_pu(int ind_def, int ind_seq, complex<double> vseq) = 0;
   };

//-----------------------------------------------------------------------------
VTBarCC* __fastcall NewObjBarCC(VTBarra *barra);

#endif
//-----------------------------------------------------------------------------
// eof
