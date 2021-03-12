//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TLigFH.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Fases.h"
#include "..\Matriz\VTMatriz.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTLigFH* __fastcall NewObjLigFH(VTLigacao *ligacao,
                                int       num_pat,
                                int       num_freq)
   {
   return(new TLigFH(ligacao, num_pat, num_freq));
   }

//-----------------------------------------------------------------------------
__fastcall TLigFH::TLigFH(VTLigacao *ligacao,
                          int       num_pat,
                          int       num_freq)
   {
   // Variáveis locais
   int tamanho2;

   // Inicialização
   PD.bar_ref = NULL;
   if(ligacao->Tipo() == eqptoTRAFO3E) tamanho2 = 3 * MAX_FASE;
   else                                tamanho2 = 2 * MAX_FASE;

   // Salva dados
   PD.ligacao  = ligacao;
   PD.num_pat  = num_pat;
   PD.num_freq = num_freq;
   PD.tamanho2 = tamanho2;

   // Aloca memória
   PD.dht_corr  = NewObjMatrizD(num_pat, tamanho2);
   PD.ief_amp   = NewObjMatrizD(num_pat, tamanho2);

   PD.dh_corr   = NewObjMatrizD(num_pat, tamanho2, num_freq);
   PD.iamp      = NewObjMatrizC(num_pat, tamanho2, num_freq);
   PD.perda_kva = NewObjMatrizC(num_pat, MAX_FASE, num_freq);
   }

//-----------------------------------------------------------------------------
__fastcall TLigFH::~TLigFH(void)
   {
   // Libera memória
   delete PD.dht_corr;
   delete PD.ief_amp;

   delete PD.dh_corr;
   delete PD.iamp;
   delete PD.perda_kva;
   }

//---------------------------------------------------------------------------
VTEqpto * __fastcall TLigFH::PM_GetBarRef(void)
   {
   return(PD.bar_ref);
   }

//---------------------------------------------------------------------------
double __fastcall TLigFH::PM_GetDH_Corr(int ind_pat, int ind_fase, int ind_freq)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= PD.tamanho2) || (ind_freq >= PD.num_freq)) return(0.);
   return(PD.dh_corr->D3[ind_pat][ind_fase][ind_freq]);
   }

//---------------------------------------------------------------------------
double __fastcall TLigFH::PM_GetDHT_Corr(int ind_pat, int ind_fase)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= PD.tamanho2)) return(0.);
   return(PD.dht_corr->D2[ind_pat][ind_fase]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TLigFH::PM_GetIamp(int ind_pat, int ind_fase, int ind_freq)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= PD.tamanho2) || (ind_freq >= PD.num_freq)) return(complex<double>(0., 0.));
   return(PD.iamp->C3[ind_pat][ind_fase][ind_freq]);
   }

//---------------------------------------------------------------------------
double __fastcall TLigFH::PM_GetIef_amp(int ind_pat, int ind_fase)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= PD.tamanho2)) return(0.);
   return(PD.ief_amp->D2[ind_pat][ind_fase]);
   }

//-----------------------------------------------------------------------------
VTLigacao* __fastcall TLigFH::PM_GetLigacao(void)
   {
   return(PD.ligacao);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TLigFH::PM_GetPerda_kva(int ind_pat, int ind_fase, int ind_freq)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= MAX_FASE) || (ind_freq >= PD.num_freq)) return(complex<double>(0., 0.));
   return(PD.perda_kva->C3[ind_pat][ind_fase][ind_freq]);
   }

//---------------------------------------------------------------------------
void __fastcall TLigFH::PM_SetBarRef(VTEqpto *bar_ref)
   {
   PD.bar_ref = bar_ref;
   }

//---------------------------------------------------------------------------
void __fastcall TLigFH::PM_SetDH_Corr(int ind_pat, int ind_fase, int ind_freq, double dh)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= PD.tamanho2) || (ind_freq >= PD.num_freq)) return;
   PD.dh_corr->D3[ind_pat][ind_fase][ind_freq] = dh;
   }

//---------------------------------------------------------------------------
void __fastcall TLigFH::PM_SetDHT_Corr(int ind_pat, int ind_fase, double dht)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= PD.tamanho2)) return;
   PD.dht_corr->D2[ind_pat][ind_fase] = dht;
   }

//---------------------------------------------------------------------------
void __fastcall TLigFH::PM_SetIamp(int ind_pat, int ind_fase, int ind_freq, complex<double> iamp)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= PD.tamanho2) || (ind_freq >= PD.num_freq)) return;
   PD.iamp->C3[ind_pat][ind_fase][ind_freq] = iamp;
   }

//---------------------------------------------------------------------------
void __fastcall TLigFH::PM_SetIef_amp(int ind_pat, int ind_fase, double ief_amp)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= PD.tamanho2)) return;
   PD.ief_amp->D2[ind_pat][ind_fase] = ief_amp;
   }

//---------------------------------------------------------------------------
void __fastcall TLigFH::PM_SetPerda_kva(int ind_pat, int ind_fase, int ind_freq, complex<double> perda_kva)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= MAX_FASE) || (ind_freq >= PD.num_freq)) return;
   PD.perda_kva->C3[ind_pat][ind_fase][ind_freq] = perda_kva;
   }

//-----------------------------------------------------------------------------
void __fastcall TLigFH::ReiniciaValores(void)
   {

   }

//---------------------------------------------------------------------------
//eof