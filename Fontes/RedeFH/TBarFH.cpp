//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TBarFH.h"
#include "..\Constante\Fases.h"
#include "..\Matriz\VTMatriz.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTBarFH* __fastcall NewObjBarFH(VTBarra *barra,
                                int     num_pat,
                                int     num_freq)
   {
   return(new TBarFH(barra, num_pat, num_freq));
   }

//-----------------------------------------------------------------------------
__fastcall TBarFH::TBarFH(VTBarra *barra,
                          int     num_pat,
                          int     num_freq)
   {
   // Variáveis locais
   int num_fases_m1;

   // Salva dados
   PD.barra    = barra;
   PD.num_pat  = num_pat;
   PD.num_freq = num_freq;

   // Aloca memória
   num_fases_m1  = MAX_FASE + 1; // Inclui a corrente no aterramento (fases ABCN + T)
   PD.dht_corr   = NewObjMatrizD(num_pat, num_fases_m1);
   PD.dht_tensao = NewObjMatrizD(num_pat, MAX_FASE);
   PD.ief_amp    = NewObjMatrizD(num_pat, num_fases_m1);
   PD.vef_pu     = NewObjMatrizD(num_pat, MAX_FASE);

   PD.dh_corr    = NewObjMatrizD(num_pat, num_fases_m1, num_freq);
   PD.dh_tensao  = NewObjMatrizD(num_pat, MAX_FASE,     num_freq);
   PD.iamp       = NewObjMatrizC(num_pat, num_fases_m1, num_freq);
   PD.vpu        = NewObjMatrizC(num_pat, MAX_FASE,     num_freq);
   }

//-----------------------------------------------------------------------------
__fastcall TBarFH::~TBarFH(void)
   {
   // Libera memória
   delete PD.dht_corr;
   delete PD.dht_tensao;
   delete PD.ief_amp;
   delete PD.vef_pu;

   delete PD.dh_corr;
   delete PD.dh_tensao;
   delete PD.iamp;
   delete PD.vpu;
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TBarFH::PM_GetBarra(void)
   {
   return(PD.barra);
   }

//---------------------------------------------------------------------------
double __fastcall TBarFH::PM_GetDH_Corr(int ind_pat, int ind_fase, int ind_freq)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase > MAX_FASE) || (ind_freq >= PD.num_freq)) return(0.);
   return(PD.dh_corr->D3[ind_pat][ind_fase][ind_freq]);
   }

//---------------------------------------------------------------------------
double __fastcall TBarFH::PM_GetDH_Tensao(int ind_pat, int ind_fase, int ind_freq)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= MAX_FASE) || (ind_freq >= PD.num_freq)) return(0.);
   return(PD.dh_tensao->D3[ind_pat][ind_fase][ind_freq]);
   }

//---------------------------------------------------------------------------
double __fastcall TBarFH::PM_GetDHT_Corr(int ind_pat, int ind_fase)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase > MAX_FASE)) return(0.);
   return(PD.dht_corr->D2[ind_pat][ind_fase]);
   }

//---------------------------------------------------------------------------
double __fastcall TBarFH::PM_GetDHT_Tensao(int ind_pat, int ind_fase)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= MAX_FASE)) return(0.);
   return(PD.dht_tensao->D2[ind_pat][ind_fase]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TBarFH::PM_GetIamp(int ind_pat, int ind_fase, int ind_freq)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase > MAX_FASE) || (ind_freq >= PD.num_freq)) return(complex<double>(0., 0.));
   return(PD.iamp->C3[ind_pat][ind_fase][ind_freq]);
   }

//---------------------------------------------------------------------------
double __fastcall TBarFH::PM_GetIef_amp(int ind_pat, int ind_fase)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase > MAX_FASE)) return(0.);
   return(PD.ief_amp->D2[ind_pat][ind_fase]);
   }

//---------------------------------------------------------------------------
double __fastcall TBarFH::PM_GetVef_pu(int ind_pat, int ind_fase)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= MAX_FASE)) return(0.);
   return(PD.vef_pu->D2[ind_pat][ind_fase]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TBarFH::PM_GetVpu(int ind_pat, int ind_fase, int ind_freq)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= MAX_FASE) || (ind_freq >= PD.num_freq)) return(complex<double>(0., 0.));
   return(PD.vpu->C3[ind_pat][ind_fase][ind_freq]);
   }

//---------------------------------------------------------------------------
void __fastcall TBarFH::PM_SetBarra(VTBarra *barra)
	{
   PD.barra = barra;
   }

//---------------------------------------------------------------------------
void __fastcall TBarFH::PM_SetDH_Corr(int ind_pat, int ind_fase, int ind_freq, double dh)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase > MAX_FASE) || (ind_freq >= PD.num_freq)) return;
   PD.dh_corr->D3[ind_pat][ind_fase][ind_freq] = dh;
   }

//---------------------------------------------------------------------------
void __fastcall TBarFH::PM_SetDH_Tensao(int ind_pat, int ind_fase, int ind_freq, double dh)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= MAX_FASE) || (ind_freq >= PD.num_freq)) return;
   PD.dh_tensao->D3[ind_pat][ind_fase][ind_freq] = dh;
   }

//---------------------------------------------------------------------------
void __fastcall TBarFH::PM_SetDHT_Corr(int ind_pat, int ind_fase, double dht)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase > MAX_FASE)) return;
   PD.dht_corr->D2[ind_pat][ind_fase] = dht;
   }

//---------------------------------------------------------------------------
void __fastcall TBarFH::PM_SetDHT_Tensao(int ind_pat, int ind_fase, double dht)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= MAX_FASE)) return;
   PD.dht_tensao->D2[ind_pat][ind_fase] = dht;
   }

//---------------------------------------------------------------------------
void __fastcall TBarFH::PM_SetIamp(int ind_pat, int ind_fase, int ind_freq, complex<double> iamp)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase > MAX_FASE) || (ind_freq >= PD.num_freq)) return;
   PD.iamp->C3[ind_pat][ind_fase][ind_freq] = iamp;
   }

//---------------------------------------------------------------------------
void __fastcall TBarFH::PM_SetIef_amp(int ind_pat, int ind_fase, double ief_amp)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase > MAX_FASE)) return;
   PD.ief_amp->D2[ind_pat][ind_fase] = ief_amp;
   }

//---------------------------------------------------------------------------
void __fastcall TBarFH::PM_SetVef_pu(int ind_pat, int ind_fase, double vef_pu)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= MAX_FASE)) return;
   PD.vef_pu->D2[ind_pat][ind_fase] = vef_pu;
   }

//---------------------------------------------------------------------------
void __fastcall TBarFH::PM_SetVpu(int ind_pat, int ind_fase, int ind_freq, complex<double> vpu)
   {
   if((ind_pat >= PD.num_pat) || (ind_fase >= MAX_FASE) || (ind_freq >= PD.num_freq)) return;
   PD.vpu->C3[ind_pat][ind_fase][ind_freq] = vpu;
   }

//---------------------------------------------------------------------------
void __fastcall TBarFH::ReiniciaValores(void)
   {

   }
   
//---------------------------------------------------------------------------
//eof