//---------------------------------------------------------------------------
#ifndef TLigCC_H
#define TLigCC_H

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTLigCC.h"
#include "ResCurto.h"
#include "..\Rede\VTLigacao.h"

//---------------------------------------------------------------------------
class TLigCC : public VTLigCC
   {
   public:
           __fastcall TLigCC(VTLigacao *ligacao);
           __fastcall ~TLigCC(void);
      void __fastcall ReiniciaSentido(void);
      void __fastcall ReiniciaValores(void);

   private: //métodos acessados via property
      VTLigacao*      __fastcall PM_GetLigacao(void);
      double          __fastcall PM_GetIfas_amp_max(int ind_def, int ind_bar);
      complex<double> __fastcall PM_GetIfas_amp(int ind_def, int ind_bar, int ind_fas);
      complex<double> __fastcall PM_GetIseq_amp(int ind_def, int ind_bar, int ind_seq);
      complex<double> __fastcall PM_GetIter_amp(int ind_def, int ind_bar);
      complex<double> __fastcall PM_GetS_mva(int ind_def, int ind_bar);
      int             __fastcall PM_GetSentido(int ind_def, int ind_bar);
      void            __fastcall PM_SetIfas_amp(int ind_def, int ind_bar, int ind_fas, complex<double> ifas);
      void            __fastcall PM_SetIseq_amp(int ind_def, int ind_bar, int ind_seq, complex<double> iseq);
      void            __fastcall PM_SetS_mva(int ind_def, int ind_bar, complex<double> s_mva);
      void            __fastcall PM_SetSentido(int ind_def, int ind_bar, int sentido);

   private: //dados acessados via property
      #define  VAL_MIN 0.5
      struct{//objeto externo
            VTLigacao *ligacao;
            //dados locais
            int             sentido[NUM_MAX_DEF][NBAR_LIG];
            complex<float> ifas[NUM_MAX_DEF][NBAR_LIG][NUM_MAX_FAS];
            complex<float> iseq[NUM_MAX_DEF][NBAR_LIG][NUM_MAX_SEQ];
            complex<float> sfas[NUM_MAX_DEF][NBAR_LIG];
            }PD;
   };

#endif
//-----------------------------------------------------------------------------
// eof
