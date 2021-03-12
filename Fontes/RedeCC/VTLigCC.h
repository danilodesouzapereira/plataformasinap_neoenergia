//---------------------------------------------------------------------------
#ifndef VTLigCCH
#define VTLigCCH

//---------------------------------------------------------------------------
using namespace std;

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "..\Rede\VTLigacao.h"

//---------------------------------------------------------------------------
class VTLigCC : public TObject
   {
   public:   //property
      __property VTLigacao*      Ligacao                                         = {read=PM_GetLigacao};
      __property double          Ifas_amp_max[int ind_def][int ind_bar]          = {read=PM_GetIfas_amp_max};
      __property complex<double> Ifas_amp[int ind_def][int ind_bar][int ind_fas] = {read=PM_GetIfas_amp, write=PM_SetIfas_amp};
      __property complex<double> Iseq_amp[int ind_def][int ind_bar][int ind_seq] = {read=PM_GetIseq_amp, write=PM_SetIseq_amp};
      __property complex<double> Iter_amp[int ind_def][int ind_bar]              = {read=PM_GetIter_amp};
      __property int             Sentido [int ind_def][int ind_bar]              = {read=PM_GetSentido,  write=PM_SetSentido};
      //potência de defeito em relacão a uma Barra
      __property complex<double> S_mva[int ind_def][int ind_bar] = {read=PM_GetS_mva, write=PM_SetS_mva};

   public:
                   __fastcall VTLigCC(void) {};
      virtual      __fastcall ~VTLigCC(void) {};
      virtual void __fastcall ReiniciaSentido(void) = 0;
      virtual void __fastcall ReiniciaValores(void) = 0;

   protected:   //métodos acessados via property
      virtual VTLigacao*      __fastcall PM_GetLigacao(void) = 0;
      virtual double          __fastcall PM_GetIfas_amp_max(int ind_def, int ind_bar) = 0;
      virtual complex<double> __fastcall PM_GetIfas_amp(int ind_def, int ind_bar, int ind_fas) = 0;
      virtual complex<double> __fastcall PM_GetIseq_amp(int ind_def, int ind_bar, int ind_seq) = 0;
      virtual complex<double> __fastcall PM_GetIter_amp(int ind_def, int ind_bar) = 0;
      virtual complex<double> __fastcall PM_GetS_mva(int ind_def, int ind_bar) = 0;
      virtual int             __fastcall PM_GetSentido(int ind_def, int ind_bar) = 0;
      virtual void            __fastcall PM_SetIfas_amp(int ind_def, int ind_bar, int ind_fas, complex<double> ifas) = 0;
      virtual void            __fastcall PM_SetIseq_amp(int ind_def, int ind_bar, int ind_seq, complex<double> iseq) = 0;
      virtual void            __fastcall PM_SetS_mva(int ind_def, int ind_bar, complex<double> s_mva) = 0;
      virtual void            __fastcall PM_SetSentido(int ind_def, int ind_bar, int sentido) = 0;
   };

//-----------------------------------------------------------------------------
VTLigCC* __fastcall NewObjLigCC(VTLigacao *ligacao);

#endif
//-----------------------------------------------------------------------------
// eof
