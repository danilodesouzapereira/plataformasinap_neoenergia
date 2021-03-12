//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include "TResFlowLig.h"
#include "TResFlowBar.h"
#include "VTBarra.h"
#include "VTFases.h"
#include "VTLigacao.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Const.h"
#include "..\Constante\Trifasico.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTResFlowLig* __fastcall NewObjResFlowLig(VTLigacao *pliga, int num_bar, int num_pat)
   {
   try{
      return(new TResFlowLig(pliga, num_bar, num_pat));
      }catch(Exception &e)
         {}
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TResFlowLig::TResFlowLig(VTLigacao *pliga, int num_bar, int num_pat)
   {
   //salva ponteiro p/ Ligacao, número de barras e dimensão do vetor strPOTENCIA
   PD.ligacao = pliga;
   PD.max_bar = num_bar;
   Max_pat    = num_pat;
   //inicia dados
   perda_diaria_mwh    = 0;
   perda_diaria_fe_mwh = 0;
   }

//---------------------------------------------------------------------------
__fastcall TResFlowLig::~TResFlowLig(void)
   {
   //libera memória alocada p/ vetor de strPOTENCIA
   VetorDelete();
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::CopiaAtributosDe(VTResFlowLig &ref)
	{
	//variáveis locais
	TResFlowLig &resflow = (TResFlowLig&)ref;

	//CUIDADO: NÃO copia dados acessados via property pois existe ponteiro p/ Ligacao
	//PD = resflow.PD;

	//proteção: verifica se vetores tem mesma dimensão
	if (Max_pat != ref.Max_pat)
		{//reinicia vetores
		Max_pat = ref.Max_pat;;
		}
	//copia vetores de 1 dimensão: cargto e diag
	memcpy(cargto,  resflow.cargto, (Max_pat * sizeof(float)));
	memcpy(diag,    resflow.diag,   (Max_pat * sizeof(int)));
	//copia vetores de 2 dimensões: sentido e strCORRENTE
	memcpy(sentido, resflow.sentido,   (Max_bar * Max_pat * sizeof(int)));
	memcpy(corrente, resflow.corrente, (Max_bar * Max_pat * sizeof(strCORRENTE)));
	}

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::CorrenteFase_amp(complex<double>sf_mva, complex<double>vf_kv)
   {
   //proteção: verifica se tensão nula
   if (Abs(vf_kv) == 0.) return(0.);
   //verifica se a potência ativa é nula
   if (sf_mva.real() < 0.)
      {//fluxo invertido: inverte sinal
      sf_mva *= -1.;
      }
   //converte potência p/ kVA e calcula corrente em A
   return(conj((1000. * sf_mva)/vf_kv));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::CorrenteNeutro_amp(complex<double>sf_mva, complex<double>vf_kv)
   {
   return(CorrenteFase_amp(sf_mva, vf_kv) * -1.);
   }

//---------------------------------------------------------------------------
bool __fastcall TResFlowLig::ExisteDiagnostico(int tipo_diag)
   {
   for (int np = 0; np < Max_pat; np++)
      {
      if (diag[np] == tipo_diag) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::FatorPotencia(complex<double> s)
   {
   //variáveis locais
   double mod_s;

   //determina mód. da pot.aparente e verifica se é nulo
   if ((mod_s = Abs(s)) == 0.) return(0.);
   //retorna fator de potência = P/abs(S)
   return (fabs(s.real()) / mod_s);
   }

//---------------------------------------------------------------------------
int __fastcall TResFlowLig::IndexOf(int nb, int np)
   {
   //proteção
   if ((nb < 0)||(nb >= Max_bar)) throw("TResFlowLig::IndexOf() - barra inválida");
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowLig::IndexOf() - patamar inválido");
   return((nb * Max_pat) + np);
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::Inicia(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::Inicia() - patamar inválido");
   //inicia vetores de 1 dimensão (patamar)
   Diagnostico[np]  = diagISOLADO;
   Cargto_pu[np]    = 0.;
   //inicia vetores de 2 dimensões (barra, patamar)
   for (int nb = 0; nb < Max_bar; nb++)
      {
      SentidoP[nb][np] = fluxoNENHUM;
      SentidoQ[nb][np] = fluxoNENHUM;
      Ia_amp[nb][np]   = CZero();
      Ib_amp[nb][np]   = CZero();
      Ic_amp[nb][np]   = CZero();
      In_amp[nb][np]   = CZero();
      }
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetCargto(int np)
   {
   return(Cargto_pu[np] * 100.);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetCargto_pu(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowLig::PM_GetCarregamento() - patamar inválido");
   //retorna carregamento
   return(cargto[np]);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetCargtoMax_pu(void)
   {
   //variáveis locais
   double cargto_max;

   cargto_max = cargto[0];
   for (int np = 1; np < Max_pat; np++)
      {
      if (cargto_max < cargto[np]) cargto_max = cargto[np];
      }
   return(cargto_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetCargtoMin_pu(void)
   {
   //variáveis locais
   double cargto_min;

   cargto_min = cargto[0];
   for (int np = 1; np < Max_pat; np++)
      {
      if (cargto_min > cargto[np]) cargto_min = cargto[np];
      }
   return(cargto_min);
   }

//---------------------------------------------------------------------------
int __fastcall TResFlowLig::PM_GetDiagnostico(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowLig::PM_GetDiagnostico() - patamar inválido");
   //retorna carregamento
   return(diag[np]);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetEnergiaDiaria_mwh(int nb)
   {
   //variáveis locais
   double p_mw        = 0;
   double periodo_pat = double(24.)/double(Max_pat);

   //soma potência ativa em todos os patamares
   for (int np = 0; np < Max_pat; np++)
      {
      p_mw += P_mw[nb][np];
      }
   //retorna valor de energia
   return(p_mw * periodo_pat);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetFP_pu(int nb, int np)
   {
   //retorna fator de potência total
   return(FatorPotencia(PM_GetS_mva(nb, np)));
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetFPa_pu(int nb, int np)
   {
   //retorna fator de potência da fase A
   return(FatorPotencia(PM_GetSa_mva(nb, np)));
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetFPb_pu(int nb, int np)
   {
   //retorna fator de potência da fase B
   return(FatorPotencia(PM_GetSb_mva(nb, np)));
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetFPc_pu(int nb, int np)
   {
   //retorna fator de potência da fase C
   return(FatorPotencia(PM_GetSc_mva(nb, np)));
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetFPf_pu(int nb, int np, int nf)
   {
   //retorna fator de potência da fase indicada
   return(FatorPotencia(PM_GetSf_mva(nb, np, nf)));
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetFPf_pu_max(int nb, int np)
   {
   //variáveis locais
   double   fp, fp_max;
   VTBarra *barra = PD.ligacao->Barra(nb);
   int      fases = barra->resflow->Fases;

   //inicia fp_max c/ o fator de potência de uma fase existente
   if      ((fases & faseA) == faseA) fp_max = PM_GetFPa_pu(nb, np);
   else if ((fases & faseB) == faseB) fp_max = PM_GetFPb_pu(nb, np);
   else if ((fases & faseC) == faseC) fp_max = PM_GetFPc_pu(nb, np);
   else return(0.);
   //determaxa menor fator de potência
   if ((fases & faseA) == faseA) fp_max = max(fp_max, PM_GetFPa_pu(nb, np));
   if ((fases & faseB) == faseB) fp_max = max(fp_max, PM_GetFPb_pu(nb, np));
   if ((fases & faseC) == faseC) fp_max = max(fp_max, PM_GetFPc_pu(nb, np));
   return(fp_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetFPf_pu_min(int nb, int np)
   {
   //variáveis locais
   double   fp, fp_min;
   VTBarra *barra = PD.ligacao->Barra(nb);
   int      fases = barra->resflow->Fases;

   //inicia fp_min c/ o fator de potência de uma fase existente
   if      ((fases & faseA) == faseA) fp_min = PM_GetFPa_pu(nb, np);
   else if ((fases & faseB) == faseB) fp_min = PM_GetFPb_pu(nb, np);
   else if ((fases & faseC) == faseC) fp_min = PM_GetFPc_pu(nb, np);
   else return(0.);
   //determina menor fator de potência
   if ((fases & faseA) == faseA) fp_min = min(fp_min, PM_GetFPa_pu(nb, np));
   if ((fases & faseB) == faseB) fp_min = min(fp_min, PM_GetFPb_pu(nb, np));
   if ((fases & faseC) == faseC) fp_min = min(fp_min, PM_GetFPc_pu(nb, np));
   return(fp_min);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetFPn_pu(int nb, int np)
   {
   //retorna fator de potência do neutro
   return(FatorPotencia(PM_GetSn_mva(nb, np)));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::PM_GetIa_amp(int nb, int np)
   {
   //retorna corrente da fase A (em A)
   return(corrente[IndexOf(nb, np)].ia_amp);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::PM_GetIb_amp(int nb, int np)
   {
   //retorna corrente da fase B (em A)
   return(corrente[IndexOf(nb, np)].ib_amp);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::PM_GetIc_amp(int nb, int np)
   {
   //retorna corrente da fase C (em A)
   return(corrente[IndexOf(nb, np)].ic_amp);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::PM_GetIf_amp(int nb, int np, int nf)
   {
   //retorna corrente da fase indicada (em A)
   switch(nf)
      {
      case 0: return(PM_GetIa_amp(nb, np));
      case 1: return(PM_GetIb_amp(nb, np));
      case 2: return(PM_GetIc_amp(nb, np));
      case 3: return(PM_GetIn_amp(nb, np));
      default: throw("TResFlowLig::PM_GetIf_amp() - fase inválida");
      }
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetIf_amp_max(int nb, int np)
   {
   //variáveis locais
   double cor, cor_max;

   //assume que a corrente da fase A é maior
   cor_max = Abs(PM_GetIa_amp(nb, np));
   //determina a maior corrente entre as fases A e B
   cor = Abs(PM_GetIb_amp(nb, np));
   if (cor > cor_max) cor_max = cor;
   //determina a maior corrente entre as fases A, B e C
   cor = Abs(PM_GetIc_amp(nb, np));
   if (cor > cor_max) cor_max = cor;
   return(cor_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetIf_amp_min(int nb, int np)
   {
   //variáveis locais
   double cor, cor_min;

   //assume que a corrente da fase A é menor
   cor_min = Abs(PM_GetIa_amp(nb, np));
   //determina a menor corrente entre as fases A e B
   cor = Abs(PM_GetIb_amp(nb, np));
   if (cor < cor_min) cor_min = cor;
   //determina a menor corrente entre as fases A, B e C
   cor = Abs(PM_GetIc_amp(nb, np));
   if (cor < cor_min) cor_min = cor;
   return(cor_min);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::PM_GetIn_amp(int nb, int np)
   {
   //retorna corrente do neutro (em A)
   return(corrente[IndexOf(nb, np)].in_amp);
   }

//---------------------------------------------------------------------------
VTLigacao* __fastcall TResFlowLig::PM_GetLigacao(void)
   {
   return(PD.ligacao);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetIndiceBal(int nb, int np)
   {
   //variáveis locais
   double sf_max_mva = Abs(Sf_mva_max[nb][np]);
   double s_mva      = Abs(S_mva[nb][np]);

   //proteção
   if (IsDoubleZero(s_mva)) return(0.);
   return(((3. * sf_max_mva) / s_mva) * 100.);
   }

//---------------------------------------------------------------------------
int __fastcall TResFlowLig::PM_GetMaxBar(void)
   {
   return(PD.max_bar);
   }

//---------------------------------------------------------------------------
int __fastcall TResFlowLig::PM_GetMaxPat(void)
   {
   return(PD.max_pat);
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::PM_SetMaxPat(int max_pat)
   {
   //verifica se o número de patamares é diferente do valor anterior
   if (PD.max_pat == max_pat) return;
   //libera memória alocada p/ vetores
   VetorDelete();
   //salva número máximo de patamares
   PD.max_pat = max_pat;
   //aloca memória p/ vetores
   VetorNew();
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetPerda_mw(int np)
   {
   //variáveis locais
   double perda_mw = 0.;

   //calcula perda como o resultado da soma das potências nas barras
   for (int nb = 0; nb < Max_bar; nb++)
      {
      perda_mw += PM_GetP_mw(nb, np);
      }
   //proteção contra valores pequenos (menor que 1 W)
   if (fabs(perda_mw) < 1.0E-8) perda_mw = 0.;

   return(perda_mw);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetPerdaDiaria_mwh(void)
   {
   return(perda_diaria_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetPerdaDiariaFerro_mwh(void)
   {
   return(perda_diaria_fe_mwh);
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::PM_SetPerdaDiaria_mwh(double perda_mwh)
   {
   perda_diaria_mwh = perda_mwh;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::PM_SetPerdaDiariaFerro_mwh(double perda_mwh)
   {
   perda_diaria_fe_mwh = perda_mwh;
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetP_mw(int nb, int np)
   {
   return(S_mva[nb][np].real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetPa_mw(int nb, int np)
   {
   return(PM_GetSa_mva(nb, np).real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetPb_mw(int nb, int np)
   {
   return(PM_GetSb_mva(nb, np).real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetPc_mw(int nb, int np)
   {
   return(PM_GetSc_mva(nb, np).real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetPf_mw(int nb, int np, int nf)
   {
   return(PM_GetSf_mva(nb, np, nf).real());
   }
/*
//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetPf_mw_max(int nb, int np)
   {
   //variáveis locais
   double p, p_max;

   //assume que a Pot.Ativa da fase A é maior
   p_max = Abs(PM_GetPa_mw(nb, np));
   //determina a maior Pot.Ativa entre as fases A e B
   p = Abs(PM_GetPb_mw(nb, np));
   if (p > p_max) p_max = p;
   //determina a maior Pot.Ativa entre as fases A, B e C
   p = Abs(PM_GetPc_mw(nb, np));
   if (p > p_max) p_max = p;
   return(p_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetPf_mw_min(int nb, int np)
   {
   //variáveis locais
   double p, p_min;

   //assume que a Pot.Ativa da fase A é maior
   p_min = Abs(PM_GetPa_mw(nb, np));
   //determina a maior Pot.Ativa entre as fases A e B
   p = Abs(PM_GetPb_mw(nb, np));
   if (p < p_min) p_min = p;
   //determina a maior Pot.Ativa entre as fases A, B e C
   p = Abs(PM_GetPc_mw(nb, np));
   if (p < p_min) p_min = p;
   return(p_min);
   }
*/

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetPf_mw_max(int nb, int np)
   {
   //variáveis locais
   double p, p_max;

   //assume que a Pot.Ativa da fase A é maior
   p_max = PM_GetPa_mw(nb, np);
   //determina a maior Pot.Ativa entre as fases A e B
   p = PM_GetPb_mw(nb, np);
   if (fabs(p) > fabs(p_max)) p_max = p;
   //determina a maior Pot.Ativa entre as fases A, B e C
   p = PM_GetPc_mw(nb, np);
   if (fabs(p) > fabs(p_max)) p_max = p;
   return(p_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetPf_mw_min(int nb, int np)
   {
   //variáveis locais
   double p, p_min;

   //assume que a Pot.Ativa da fase A é maior
   p_min = PM_GetPa_mw(nb, np);
   //determina a menor Pot.Ativa entre as fases A e B
   p = PM_GetPb_mw(nb, np);
   if (fabs(p) < fabs(p_min)) p_min = p;
   //determina a menor Pot.Ativa entre as fases A, B e C
   p = PM_GetPc_mw(nb, np);
   if (fabs(p) < fabs(p_min)) p_min = p;
   return(p_min);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetPn_mw(int nb, int np)
   {
   return(PM_GetSn_mva(nb, np).real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetQ_mvar(int nb, int np)
   {
   return(S_mva[nb][np].imag());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetQa_mvar(int nb, int np)
   {
   return(PM_GetSa_mva(nb, np).imag());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetQb_mvar(int nb, int np)
   {
   return(PM_GetSb_mva(nb, np).imag());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetQc_mvar(int nb, int np)
   {
   return(PM_GetSc_mva(nb, np).imag());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetQf_mvar(int nb, int np, int nf)
   {
   return(PM_GetSf_mva(nb, np, nf).imag());
   }
/*
//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetQf_mvar_max(int nb, int np)
   {
   //variáveis locais
   double q, q_max;

   //assume que a Pot.Ativa da fase A é maior
   q_max = Abs(PM_GetQa_mvar(nb, np));
   //determina a maior Pot.Ativa entre as fases A e B
   q = Abs(PM_GetQb_mvar(nb, np));
   if (q > q_max) q_max = q;
   //determina a maior Pot.Ativa entre as fases A, B e C
   q = Abs(PM_GetQc_mvar(nb, np));
   if (q > q_max) q_max = q;
   return(q_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetQf_mvar_min(int nb, int np)
   {
   //variáveis locais
   double q, q_min;

   //assume que a Pot.Ativa da fase A é maior
   q_min = Abs(PM_GetQa_mvar(nb, np));
   //determina a maior Pot.Ativa entre as fases A e B
   q = Abs(PM_GetQb_mvar(nb, np));
   if (q < q_min) q_min = q;
   //determina a maior Pot.Ativa entre as fases A, B e C
   q = Abs(PM_GetQc_mvar(nb, np));
   if (q < q_min) q_min = q;
   return(q_min);
   }
*/

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetQf_mvar_max(int nb, int np)
   {
   //variáveis locais
   double q, q_max;

   //assume que a Pot.Reativa da fase A é maior
   q_max = PM_GetQa_mvar(nb, np);
   //determina a maior Pot.Reativa entre as fases A e B
   q = PM_GetQb_mvar(nb, np);
   if (fabs(q) > fabs(q_max)) q_max = q;
   //determina a maior Pot.Reativa entre as fases A, B e C
   q = PM_GetQc_mvar(nb, np);
   if (fabs(q) > fabs(q_max)) q_max = q;
   return(q_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetQf_mvar_min(int nb, int np)
   {
   //variáveis locais
   double q, q_min;

   //assume que a Pot.Reativa da fase A é menor
   q_min = PM_GetQa_mvar(nb, np);
   //determina a menor Pot.Reativa entre as fases A e B
   q = PM_GetQb_mvar(nb, np);
   if (fabs(q) < fabs(q_min)) q_min = q;
   //determina a menor Pot.Reativa entre as fases A, B e C
   q = PM_GetQc_mvar(nb, np);
   if (fabs(q) < fabs(q_min)) q_min = q;
   return(q_min);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetQn_mvar(int nb, int np)
   {
   return(PM_GetSn_mva(nb, np).imag());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetQuedaVfn_pu_max(int np)
   {
   //variáveis locais
   double dv_pu[MAX_FASE], dv_pu_max;

   for (int nf = indFASE_A; nf <= indFASE_C; nf++)
      {
      //dv_pu[nf] = Abs(PD.ligacao->pbarra1->resflow->Vfn_pu[np][nf] - Abs(PD.ligacao->pbarra2->resflow->Vfn_pu[np][nf]);
      dv_pu[nf] = fabs(Abs(PD.ligacao->pbarra1->resflow->Vfn_pu[np][nf]) - Abs(PD.ligacao->pbarra2->resflow->Vfn_pu[np][nf]));
      }
   dv_pu_max = max(dv_pu[indFASE_A], dv_pu[indFASE_B]);
   dv_pu_max = max(dv_pu_max, dv_pu[indFASE_C]);

   return(dv_pu_max);
   }

//---------------------------------------------------------------------------
int __fastcall TResFlowLig::PM_GetSentidoP(int nb, int np)
   {
   //variáveis locais
   int index = IndexOf(nb, np);

   //retorna sentido da pot.ativa em relação à barra nb
   return(sentido[index] & 0x0f);
   }

//---------------------------------------------------------------------------
int __fastcall TResFlowLig::PM_GetSentidoQ(int nb, int np)
   {
   //variáveis locais
   int index = IndexOf(nb, np);

   //retorna sentido da pot.reativa em relação à barra nb
   return((sentido[index] & 0xf0) >> 4);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::PM_GetS_mva(int nb, int np)
   {
   //retorna somatória da potência nas 3 fases e neutro
   return(PM_GetSa_mva(nb, np) + PM_GetSb_mva(nb, np) +
          PM_GetSc_mva(nb, np) + PM_GetSn_mva(nb, np));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::PM_GetSa_mva(int nb, int np)
   {
//double vft_kv = Abs(TensaoFaseTerra(nb, np, indFASE_A));
//double if_amp = Abs(PM_GetIa_amp(nb, np));
//double s_kva  = vft_kv * if_amp;
//double s_mva  = Abs((TensaoFaseTerra(nb, np, indFASE_A) * conj(PM_GetIa_amp(nb, np))) * 1.e-3);

   //retorna potência da fase A em relação à barra indicada
   return((TensaoFaseTerra(nb, np, indFASE_A) * conj(PM_GetIa_amp(nb, np))) * 1.e-3);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::PM_GetSb_mva(int nb, int np)
   {
   //retorna potência da fase B em relação à barra indicada
   return((TensaoFaseTerra(nb, np, indFASE_B) * conj(PM_GetIb_amp(nb, np))) * 1.e-3);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::PM_GetSc_mva(int nb, int np)
   {
   //retorna potência da fase C em relação à barra indicada
   return((TensaoFaseTerra(nb, np, indFASE_C) * conj(PM_GetIc_amp(nb, np))) * 1.e-3);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::PM_GetSf_mva(int nb, int np, int nf)
   {
   switch(nf)
      {
      case 0: return(PM_GetSa_mva(nb, np));
      case 1: return(PM_GetSb_mva(nb, np));
      case 2: return(PM_GetSc_mva(nb, np));
      case 3: return(PM_GetSn_mva(nb, np));
      default: throw("TResFlowLig::PM_GetSf_mva() - fase inválida");
      }
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetSf_mva_max(int nb, int np)
   {
   //variáveis locais
   double sf, sf_max;

   //assume maior potência igual à da primeira fase (fase A)
   sf_max = Abs(PM_GetSa_mva(nb, np));
   //determina maior potência entre as fases A e B
   sf = Abs(PM_GetSb_mva(nb, np));
   if (sf > sf_max) sf_max = sf;
   //determina maior potência entre as fases A, B e C
   sf = Abs(PM_GetSc_mva(nb, np));
   if (sf > sf_max) sf_max = sf;
   //se fluxo negativo, inverte o sinal
   if (SentidoP[nb][np] == fluxoNEG) {sf_max = - sf_max;}
   return(sf_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowLig::PM_GetSf_mva_min(int nb, int np)
   {
   //variáveis locais
   double sf, sf_min;

   //assume menor potência igual à da primeira fase (fase A)
   sf_min = Abs(PM_GetSa_mva(nb, np));
   //determina menor potência entre as fases A e B
   sf = Abs(PM_GetSb_mva(nb, np));
   if (sf < sf_min) sf_min = sf;
   //determina menor potência entre as fases A, B e C
   sf = Abs(PM_GetSc_mva(nb, np));
   if (sf < sf_min) sf_min = sf;
   //se fluxo negativo, inverte o sinal
   if (SentidoP[nb][np] == fluxoNEG) {sf_min = - sf_min;}
   return(sf_min);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::PM_GetSn_mva(int nb, int np)
   {
   //retorna potência do neutro relativa à barra nb
   return((TensaoFaseTerra(nb, np, indNEUTRO) * conj(PM_GetIn_amp(nb, np))) * 1.e-3);
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::PM_SetCargto_pu(int np, double valor)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowLig::PM_SetCarregamento() - patamar inválido");
   //atualiza carregamento
   cargto[np] = valor;
   }
   
//---------------------------------------------------------------------------
void __fastcall TResFlowLig::PM_SetDiagnostico(int np, int valor)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowLig::PM_SetDiagnostico()");
   //atualiza diagnóstico
   diag[np] = valor;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::PM_SetIa_amp(int nb, int np, complex<double> i_amp)
   {
   //salva corrente da fase A (em A)
   corrente[IndexOf(nb, np)].ia_amp = i_amp;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::PM_SetIb_amp(int nb, int np, complex<double> i_amp)
   {
   //salva corrente da fase B (em A)
   corrente[IndexOf(nb, np)].ib_amp = i_amp;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::PM_SetIc_amp(int nb, int np, complex<double> i_amp)
   {
   //salva corrente da fase C (em A)
   corrente[IndexOf(nb, np)].ic_amp = i_amp;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::PM_SetIf_amp(int nb, int np, int nf, complex<double> i_amp)
   {
   //salva corrente da fase indicada (em A)
   switch(nf)
      {
      case 0: return(PM_SetIa_amp(nb, np, i_amp));
      case 1: return(PM_SetIb_amp(nb, np, i_amp));
      case 2: return(PM_SetIc_amp(nb, np, i_amp));
      case 3: return(PM_SetIn_amp(nb, np, i_amp));
      default: throw("TResFlowLig::PM_SetIf_amp() - fase inválida");
      }
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::PM_SetIn_amp(int nb, int np, complex<double> i_amp)
   {
   //salva corrente de neutro (em A)
   corrente[IndexOf(nb, np)].in_amp = i_amp;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::PM_SetSentidoP(int nb, int np, int valor)
   {
   //variáveis locais
   int index = IndexOf(nb, np);

   //atualiza sentido da pot.ativa em relação à barra nb
   sentido[index] = (sentido[index] & 0xfff0) | (valor & 0x0f);
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::PM_SetSentidoQ(int nb, int np, int valor)
   {
   //variáveis locais
   int index = IndexOf(nb, np);

   //atualiza sentido da pot.ativa em relação à barra nb
   sentido[index] = (sentido[index] & 0xff0f) | ((valor & 0x0f) << 4);
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::Reinicia(void)
   {
   //inicia perdas
   perda_diaria_mwh    = 0;
   perda_diaria_fe_mwh = 0;
   //inicia dados por patamar
   for (int np = 0; np < Max_pat; np++) Inicia(np);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::S_mva_max_allpat(int nb)
   {
   //variáveis locais
   int    ind_pat;
   double s_mva, s_mva_max;

   //assume valor máximo como sendo do primeiro patamar
   ind_pat   = 0;
   s_mva_max = Abs(S_mva[nb][ind_pat]);
   //loop p/ todos os patamares
   for (int np = 1; np < Max_pat; np++)
      {
      s_mva = Abs(S_mva[nb][np]);
      if (s_mva > s_mva_max)
         {
         ind_pat   = np;
         s_mva_max = s_mva;
         }
      }
   return(S_mva[nb][ind_pat]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::S_mva_med_allpat(int nb)
   {
   //variáveis locais
   complex<double> s_mva = CZero();

   //loop p/ todos os patamares
   for (int np = 0; np < Max_pat; np++)
      {
      s_mva += S_mva[nb][np];
      }
   return(s_mva / double(Max_pat));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::S_mva_min_allpat(int nb)
   {
   //variáveis locais
   int    ind_pat;
   double s_mva, s_mva_min;

   //assume valor mínimo como sendo do primeiro patamar
   ind_pat   = 0;
   s_mva_min = Abs(S_mva[nb][0]);
   //loop p/ todos os patamares
   for (int np = 0; np < Max_pat; np++)
      {
      s_mva = Abs(S_mva[nb][np]);
      if (s_mva < s_mva_min)
         {
         ind_pat   = np;
         s_mva_min = s_mva;
         }
      }
   return(S_mva[nb][ind_pat]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::TensaoFaseTerra(int nb, int np, int nf)
   {
   //retorna tensão da fase nf da Barra nb no patamar np
   return((Ligacao->Barra(nb))->resflow->Vft_kv[np][nf]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowLig::TensaoNeutro(int nb, int np)
   {
   //retorna tensão da fase nf da Barra nb no patamar np
   return((Ligacao->Barra(nb))->resflow->Vnt_kv[np]);
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::VetorDelete(void)
   {
   if (cargto)   {delete[] cargto;   cargto   = NULL;}
   if (diag)     {delete[] diag;     diag     = NULL;}
   if (sentido)  {delete[] sentido;  sentido  = NULL;}
   if (corrente) {delete[] corrente; corrente = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowLig::VetorNew(void)
   {
   //cria vetores de 1 dimensão: cargto e diag
   cargto   = new float[Max_pat];
   diag     = new int[Max_pat];
   //cria vetores de 2 dimensões: strPOTENCIA e sentido
   sentido  = new int[Max_bar * Max_pat];
   corrente = new strCORRENTE[Max_bar * Max_pat];
  //inicia valores
   Reinicia();
   }

//---------------------------------------------------------------------------
//eof


