#ifndef TBarCCH
#define TBarCCH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTBarCC.h"
#include "ResCurto.h"

//---------------------------------------------------------------------------
class TBarCC : public VTBarCC
   {
   public:
           __fastcall  TBarCC(VTBarra *barra);
           __fastcall ~TBarCC(void);
      void __fastcall  ReiniciaValores(void);
      bool __fastcall  Vfase_pu(int ind_def, int fases_def, double v_pu[MAX_FASE]);

   private: //métodos acessados via property
      VTBarra*        __fastcall PM_GetBarra(void);
      complex<double> __fastcall PM_GetAssim_Ifas_amp(int ind_def, int ind_fas);
      complex<double> __fastcall PM_GetIfas_amp(int ind_def, int ind_fas);
      complex<double> __fastcall PM_GetIseq_amp(int ind_def, int ind_seq);
      complex<double> __fastcall PM_GetS_mva(int ind_def);
      complex<double> __fastcall PM_GetVfas_kv (int ind_def, int ind_fas);
      complex<double> __fastcall PM_GetVfas_pu (int ind_def, int ind_fas);
      complex<double> __fastcall PM_GetVseq_kv (int ind_def, int ind_seq);
      complex<double> __fastcall PM_GetVseq_pu (int ind_def, int ind_seq);
      void            __fastcall PM_SetBarra(VTBarra *barra);
      void            __fastcall PM_SetAssim_Ifas_amp(int ind_def, int ind_fas, complex<double> ifas);
      void            __fastcall PM_SetIfas_amp(int ind_def, int ind_fas, complex<double> ifas);
      void            __fastcall PM_SetIseq_amp(int ind_def, int ind_seq, complex<double> iseq);
      void            __fastcall PM_SetVfas_pu (int ind_def, int ind_fas, complex<double> vfas);
      void            __fastcall PM_SetVseq_pu (int ind_def, int ind_seq, complex<double> vseq);

   private: //dados acessados via property
      struct{//objeto externo
            VTBarra  *barra;
            //dados locais
            complex<float> ifas[NUM_MAX_DEF][NUM_MAX_FAS];
            complex<float> iseq[NUM_MAX_DEF][NUM_MAX_SEQ];
            complex<float> vfas[NUM_MAX_DEF][NUM_MAX_FAS];
            complex<float> vseq[NUM_MAX_DEF][NUM_MAX_SEQ];
            //assimétrico
            struct   {
                     complex<float> ifas[NUM_MAX_DEF][NUM_MAX_FAS];
                     }assimetrico;
            }PD;
   };

#endif
//-----------------------------------------------------------------------------
// eof
