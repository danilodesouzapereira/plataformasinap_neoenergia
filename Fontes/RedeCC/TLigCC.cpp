//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TLigCC.h"
#include "..\Complexo\Complexo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTLigCC* __fastcall NewObjLigCC(VTLigacao *ligacao)
   {
   return(new TLigCC(ligacao));
   }

//-----------------------------------------------------------------------------
__fastcall TLigCC::TLigCC(VTLigacao *ligacao)
   {
   //salva ponteiro p/ objeto
   PD.ligacao = ligacao;
   //associa Ligacao com esta LigCC
   if (ligacao != NULL) ligacao->Obj = this;
   }

//-----------------------------------------------------------------------------
__fastcall TLigCC::~TLigCC(void)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
VTLigacao* __fastcall TLigCC::PM_GetLigacao(void)
   {
   return(PD.ligacao);
   }

//-----------------------------------------------------------------------------
complex<double> __fastcall TLigCC::PM_GetIfas_amp(int ind_def, int ind_bar, int ind_fas)
   {
   //proteção
   if ((ind_def < 0)||(ind_def >= NUM_MAX_DEF)) return(CZero());
   if ((ind_bar < 0)||(ind_bar >= NBAR_LIG))    return(CZero());
   if ((ind_fas < 0)||(ind_fas >= NUM_MAX_FAS)) return(CZero());
   //retorna tensão de fas
   return(PD.ifas[ind_def][ind_bar][ind_fas]);
   }

//-----------------------------------------------------------------------------
double __fastcall TLigCC::PM_GetIfas_amp_max(int ind_def, int ind_bar)
   {
   //variáveis locais
   double cor, cor_max;

   //assume que a corrente da fase A é maior
   cor_max = Abs(Ifas_amp[ind_def][ind_bar][0]);
   //determina a maior corrente entre as fases A e B
   cor = Abs(Ifas_amp[ind_def][ind_bar][1]);
   if (cor > cor_max) cor_max = cor;
   //determina a maior corrente entre as fases A, B e C
   cor = Abs(Ifas_amp[ind_def][ind_bar][2]);
   if (cor > cor_max) cor_max = cor;
   return(cor_max);
   }

//-----------------------------------------------------------------------------
complex<double> __fastcall TLigCC::PM_GetIseq_amp(int ind_def, int ind_bar, int ind_seq)
   {
   //proteção
   if ((ind_def < 0)||(ind_def >= NUM_MAX_DEF)) return(CZero());
   if ((ind_bar < 0)||(ind_bar >= NBAR_LIG))    return(CZero());
   if ((ind_seq < 0)||(ind_seq >= NUM_MAX_SEQ)) return(CZero());
   //retorna tensão de sequência
   return(PD.iseq[ind_def][ind_bar][ind_seq]);
   }
//-----------------------------------------------------------------------------
complex<double> __fastcall TLigCC::PM_GetIter_amp(int ind_def, int ind_bar)
   {
   //corrente de terra := 3 * Iseq0
   return(3. * Iseq_amp[ind_def][ind_bar][0]);
   }

//-----------------------------------------------------------------------------
complex<double> __fastcall TLigCC::PM_GetS_mva(int ind_def, int ind_bar)
   {
   return(PD.sfas[ind_def][ind_bar]);
   }

//-----------------------------------------------------------------------------
int __fastcall TLigCC::PM_GetSentido(int ind_def, int ind_bar)
   {
   return(PD.sentido[ind_def][ind_bar]);
   }

//-----------------------------------------------------------------------------
void __fastcall TLigCC::PM_SetIfas_amp(int ind_def, int ind_bar, int ind_fas, complex<double> ifas)
   {
   //proteção
   if ((ind_def < 0)||(ind_def >= NUM_MAX_DEF)) return;
   if ((ind_bar < 0)||(ind_bar >= NBAR_LIG))    return;
   if ((ind_fas < 0)||(ind_fas >= NUM_MAX_FAS)) return;
   //proteção p/ valores muito pequenos
   if (IsDoubleZero(Abs(ifas), VAL_MIN)) ifas = CZero();
   //salva tensão de fas
   PD.ifas[ind_def][ind_bar][ind_fas] = ifas;
   }

//-----------------------------------------------------------------------------
void __fastcall TLigCC::PM_SetIseq_amp(int ind_def, int ind_bar, int ind_seq, complex<double> iseq)
   {
   //proteção
   if ((ind_def < 0)||(ind_def >= NUM_MAX_DEF)) return;
   if ((ind_bar < 0)||(ind_bar >= NBAR_LIG))    return;
   if ((ind_seq < 0)||(ind_seq >= NUM_MAX_SEQ)) return;
   //proteção p/ valores muito pequenos
   if (IsDoubleZero(Abs(iseq), VAL_MIN)) iseq = CZero();
   //salva tensão de sequência
   PD.iseq[ind_def][ind_bar][ind_seq] = iseq;
   }

//-----------------------------------------------------------------------------
void __fastcall TLigCC::PM_SetS_mva(int ind_def, int ind_bar, complex<double> s_mva)
   {
   PD.sfas[ind_def][ind_bar] = s_mva;
   }

//-----------------------------------------------------------------------------
void __fastcall TLigCC::PM_SetSentido(int ind_def, int ind_bar, int sentido)
   {
   PD.sentido[ind_def][ind_bar] = sentido;
   }

//-----------------------------------------------------------------------------
void __fastcall TLigCC::ReiniciaSentido(void)
   {
   for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         Sentido[ind_def][ind_bar] = fluxoNENHUM;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TLigCC::ReiniciaValores(void)
   {
   //loop p/ todos defeitos
   for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {
      //loop p/ todas Barras da ligacao
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {//loop p/ fases
         for (int ind_fas = 0; ind_fas < NUM_MAX_FAS; ind_fas++)
            {
            PD.ifas[ind_def][ind_bar][ind_fas] = CZero();
            }
         //loop p/ sequencia
         for (int ind_seq = 0; ind_seq < NUM_MAX_SEQ; ind_seq++)
            {
            PD.iseq[ind_def][ind_bar][ind_seq] = CZero();
            }
         //potência e sentido do fluxo
         PD.sfas[ind_def][ind_bar]    = CZero();
         PD.sentido[ind_def][ind_bar] = fluxoNENHUM;
         }
      }
   }

//---------------------------------------------------------------------------
//eof