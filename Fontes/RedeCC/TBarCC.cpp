//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TBarCC.h"
#include "..\Complexo\Complexo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTBarCC* __fastcall NewObjBarCC(VTBarra *barra)
   {
   return(new TBarCC(barra));
   }

//-----------------------------------------------------------------------------
__fastcall TBarCC::TBarCC(VTBarra *barra)
   {
   //salva ponteiro p/ Barra
   PD.barra = barra;
   //associa Barra com esta BarCC
   if (barra != NULL) barra->Obj = this;
   }

//-----------------------------------------------------------------------------
__fastcall TBarCC::~TBarCC(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TBarCC::PM_GetBarra(void)
   {
   return(PD.barra);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TBarCC::PM_GetAssim_Ifas_amp(int ind_def, int ind_fas)
   {
   //proteção
   if ((ind_def < 0)||(ind_def >= NUM_MAX_DEF)) return(CZero());
   if ((ind_fas < 0)||(ind_fas >= NUM_MAX_FAS)) return(CZero());
   //retorna tensão de fas
   return(PD.assimetrico.ifas[ind_def][ind_fas]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TBarCC::PM_GetIfas_amp(int ind_def, int ind_fas)
   {
   //proteção
   if ((ind_def < 0)||(ind_def >= NUM_MAX_DEF)) return(CZero());
   if ((ind_fas < 0)||(ind_fas >= NUM_MAX_FAS)) return(CZero());
   //retorna tensão de fas
   return(PD.ifas[ind_def][ind_fas]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TBarCC::PM_GetIseq_amp(int ind_def, int ind_seq)
   {
   //proteção
   if ((ind_def < 0)||(ind_def >= NUM_MAX_DEF)) return(CZero());
   if ((ind_seq < 0)||(ind_seq >= NUM_MAX_SEQ)) return(CZero());
   //retorna tensão de sequência
   return(PD.iseq[ind_def][ind_seq]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TBarCC::PM_GetS_mva(int ind_def)
   {
   //variáveis locais
   complex<double> s_kva = CZero();

   //calcula potência injetada na Barra em kVA
   for (int ind_fas = 0; ind_fas < NUM_MAX_FAS; ind_fas++)
      {
      s_kva += Vfas_kv[ind_def][ind_fas] * conj(Ifas_amp[ind_def][ind_fas]);
      }
   return(s_kva / 1000.);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TBarCC::PM_GetVfas_kv(int ind_def, int ind_fas)
   {
   //retorna tensão de fase em kV
   return(Vfas_pu[ind_def][ind_fas] * Barra->vnom / sqrt(3.));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TBarCC::PM_GetVfas_pu(int ind_def, int ind_fas)
   {
   //proteção
   if ((ind_def < 0)||(ind_def >= NUM_MAX_DEF)) return(CZero());
   if ((ind_fas < 0)||(ind_fas >= NUM_MAX_FAS)) return(CZero());
   //retorna tensão de fas
   return(PD.vfas[ind_def][ind_fas]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TBarCC::PM_GetVseq_kv(int ind_def, int ind_seq)
   {
   //retorna tensão de sequência em kV
   return(Vseq_pu[ind_def][ind_seq] * Barra->vnom / sqrt(3.));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TBarCC::PM_GetVseq_pu(int ind_def, int ind_seq)
   {
   //proteção
   if ((ind_def < 0)||(ind_def >= NUM_MAX_DEF)) return(CZero());
   if ((ind_seq < 0)||(ind_seq >= NUM_MAX_SEQ)) return(CZero());
   //retorna tensão de sequência em pu
   return(PD.vseq[ind_def][ind_seq]);
   }

//---------------------------------------------------------------------------
void __fastcall TBarCC::PM_SetBarra(VTBarra *barra)
   {
   PD.barra = barra;
   }

//---------------------------------------------------------------------------
void __fastcall TBarCC::PM_SetAssim_Ifas_amp(int ind_def, int ind_fas, complex<double> ifas)
   {
   //proteção
   if ((ind_def < 0)||(ind_def >= NUM_MAX_DEF)) return;
   if ((ind_fas < 0)||(ind_fas >= NUM_MAX_FAS)) return;
   //salva tensão de fas
   PD.assimetrico.ifas[ind_def][ind_fas] = ifas;
   }

//---------------------------------------------------------------------------
void __fastcall TBarCC::PM_SetIfas_amp(int ind_def, int ind_fas, complex<double> ifas)
   {
   //proteção
   if ((ind_def < 0)||(ind_def >= NUM_MAX_DEF)) return;
   if ((ind_fas < 0)||(ind_fas >= NUM_MAX_FAS)) return;
   //salva tensão de fas
   PD.ifas[ind_def][ind_fas] = ifas;
   }

//---------------------------------------------------------------------------
void __fastcall TBarCC::PM_SetIseq_amp(int ind_def, int ind_seq, complex<double> iseq)
   {
   //proteção
   if ((ind_def < 0)||(ind_def >= NUM_MAX_DEF)) return;
   if ((ind_seq < 0)||(ind_seq >= NUM_MAX_SEQ)) return;
   //salva tensão de sequência
   PD.iseq[ind_def][ind_seq] = iseq;
   }

//---------------------------------------------------------------------------
void __fastcall TBarCC::PM_SetVfas_pu(int ind_def, int ind_fas, complex<double> vfas)
   {
   //proteção
   if ((ind_def < 0)||(ind_def >= NUM_MAX_DEF)) return;
   if ((ind_fas < 0)||(ind_fas >= NUM_MAX_FAS)) return;
   //salva tensão de fas
   PD.vfas[ind_def][ind_fas] = vfas;
   }

//---------------------------------------------------------------------------
void __fastcall TBarCC::PM_SetVseq_pu(int ind_def, int ind_seq, complex<double> vseq)
   {
   //proteção
   if ((ind_def < 0)||(ind_def >= NUM_MAX_DEF)) return;
   if ((ind_seq < 0)||(ind_seq >= NUM_MAX_SEQ)) return;
   //salva tensão de sequência
   PD.vseq[ind_def][ind_seq] = vseq;
   }

//---------------------------------------------------------------------------
void __fastcall TBarCC::ReiniciaValores(void)
   {
   //loop p/ todos defeitos
   for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {
      //loop p/ fases
      for (int ind_fas = 0; ind_fas < NUM_MAX_FAS; ind_fas++)
         {
         PD.ifas[ind_def][ind_fas] = CZero();
         PD.vfas[ind_def][ind_fas] = CZero();
         }
      //loop p/ sequencia
      for (int ind_seq = 0; ind_seq < NUM_MAX_SEQ; ind_seq++)
         {
         PD.iseq[ind_def][ind_seq] = CZero();
         PD.vseq[ind_def][ind_seq] = CZero();
         }
      }
   }
//---------------------------------------------------------------------------
bool __fastcall TBarCC::Vfase_pu(int ind_def, int fases_def, double v_pu[MAX_FASE])
   {
   //variáveis locais
   double vpu_aux;

   //determina os valores de tensão por fase
   v_pu[indFASE_A] = Abs(Vfas_pu[ind_def][indFASE_A]);
   v_pu[indFASE_B] = Abs(Vfas_pu[ind_def][indFASE_B]);
   v_pu[indFASE_C] = Abs(Vfas_pu[ind_def][indFASE_C]);
   //verifica se defeito 3F
   if (ind_def == defeito3F)
      {//nada a fazer
      return(true);
      }
   //verifica se defeito 1F
   if ((ind_def == defeitoFT) || (ind_def == defeitoFTZ))
      {//verifica a fase do defeito
      if (fases_def == faseA)
         {//nada a fazer
         return(true);
         }
      else if (fases_def == faseB)
         {//rotaciona os valores do vetor para a direita: [Va,Vb,Vc] -> [Vc, Va, Vb]
         vpu_aux         = v_pu[indFASE_C];
         v_pu[indFASE_C] = v_pu[indFASE_B];
         v_pu[indFASE_B] = v_pu[indFASE_A];
         v_pu[indFASE_A] = vpu_aux;
         }
      else if (fases_def == faseC)
         {//rotaciona os valores do vetor para a esquerda: [Va,Vb,Vc] -> [Vb, Vc, Va]
         vpu_aux         = v_pu[indFASE_A];
         v_pu[indFASE_A] = v_pu[indFASE_B];
         v_pu[indFASE_B] = v_pu[indFASE_C];
         v_pu[indFASE_C] = vpu_aux;
         }
      else
         {//fase inválida
         return(false);
         }
      return(true);
      }
   //verifica se defeito 2F
   if ((ind_def == defeito2F) || (ind_def == defeito2FT))
      {//verifica a fase do defeito
      if (fases_def == faseBC)
         {//nada a fazer
         return(true);
         }
      else if (fases_def == faseAB)
         {//rotaciona os valores do vetor para a direita: [Va,Vb,Vc] -> [Vc, Va, Vb]
         vpu_aux         = v_pu[indFASE_C];
         v_pu[indFASE_C] = v_pu[indFASE_B];
         v_pu[indFASE_B] = v_pu[indFASE_A];
         v_pu[indFASE_A] = vpu_aux;
         }
      else if (fases_def == faseCA)
         {//rotaciona os valores do vetor para a esquerda: [Va,Vb,Vc] -> [Vb, Vc, Va]
         vpu_aux         = v_pu[indFASE_A];
         v_pu[indFASE_A] = v_pu[indFASE_B];
         v_pu[indFASE_B] = v_pu[indFASE_C];
         v_pu[indFASE_C] = vpu_aux;
         }
      else
         {//fase inválida
         return(false);
         }
      return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
//eof