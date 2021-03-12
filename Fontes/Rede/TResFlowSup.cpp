//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TResFlowSup.h"
#include "TResFlowBar.h"
#include "VTBarra.h"
#include "VTEqbar.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Const.h"
#include "..\Constante\Trifasico.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTResFlowSup* __fastcall NewObjResFlowSup(VTEqbar *eqbar, int num_pat)
   {
   try{
      return(new TResFlowSup(eqbar, num_pat));
      }catch(Exception &e)
         {}
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TResFlowSup::TResFlowSup(VTEqbar *eqbar, int num_pat)
   {
   //salva ponteiro p/ Eqbar e dimensão do vetor strPOTENCIA
   PD.eqbar = eqbar;
   Max_pat  = num_pat;
   }

//---------------------------------------------------------------------------
__fastcall TResFlowSup::~TResFlowSup(void)
   {
   //libera memória alocada p/ vetor de strPOTENCIA
   VetorDelete();
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowSup::CopiaAtributosDe(VTResFlowSup &ref)
	{
   //variáveis locais
   TResFlowSup &resflow = (TResFlowSup&)ref;

   //CUIDADO: NÃO copia dados acessados via property pois existe ponteiro p/ Eqbar
   //PD = resflow.PD;

   //proteção: verifica se vetores tem mesma dimensão
   if (Max_pat != ref.Max_pat)
      {//reinicia vetores
      Max_pat = ref.Max_pat;;
      }
   //copia vetores cargto, diag e strPOTENCIA
   memcpy(cargto,   resflow.cargto,   (Max_pat * sizeof(float)));
   memcpy(diag,     resflow.diag,     (Max_pat * sizeof(int)));
   memcpy(potencia, resflow.potencia, (Max_pat * sizeof(strPOTENCIA)));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowSup::Corrente_amp(complex<double>sf_mva, complex<double>vf_kv)
   {
   //proteção: verifica se tensão nula
   if (Abs(vf_kv) == 0.) return(0.);
   //converte potência p/ kVA e calcula corrente em A
   return(conj((1000. * sf_mva)/vf_kv));
   }

//---------------------------------------------------------------------------
bool __fastcall TResFlowSup::ExisteDiagnostico(int tipo_diag)
   {
   for (int np = 0; np < Max_pat; np++)
      {
      if (diag[np] == tipo_diag) return(true);
      }
   return(true);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::FatorPotencia(complex<double> s)
   {
   //variáveis locais
   double mod_s;

   //determina mód. da pot.aparente e verifica se é nulo
   if ((mod_s = Abs(s)) == 0.) return(0.);
   //retorna fator de potência = P/abs(S)
   return (s.real() / mod_s);
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowSup::Inicia(int np)
	{
   //inicia vertores de 1 dimensão (patamar)
   Diagnostico[np]  = diagISOLADO;
   Cargto_pu[np] = 0.;
   //inicia vetor strPOTENCIA
   Sa_mva[np] = CZero();
   Sb_mva[np] = CZero();
   Sc_mva[np] = CZero();
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetCargto(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_GetCarregamento() - patamar inválido");
   //retorna carregamento
   return(Cargto_pu[np] * 100.);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetCargto_pu(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_GetCarregamento() - patamar inválido");
   //retorna carregamento
   return(cargto[np]);
   }

//---------------------------------------------------------------------------
int __fastcall TResFlowSup::PM_GetDiagnostico(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_GetDiagnostico() - patamar inválido");
   //retorna carregamento
   return(diag[np]);
   }

//---------------------------------------------------------------------------
VTEqbar* __fastcall TResFlowSup::PM_GetEqbar(void)
   {
   return(PD.eqbar);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetFPa(int np)
   {
   //retorna fator de potência da fase A
   return(FatorPotencia(PM_GetSa_mva(np)));
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetFPb(int np)
   {
   //retorna fator de potência da fase B
   return(FatorPotencia(PM_GetSb_mva(np)));
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetFPc(int np)
   {
   //retorna fator de potência da fase C
   return(FatorPotencia(PM_GetSc_mva(np)));
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetFPf(int np, int nf)
   {
   //retorna fator de potência da fase indicada
   return(FatorPotencia(PM_GetSf_mva(np, nf)));
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetFPf_max(int np)
   {
   //variáveis locais
   double fp, fp_max;

   //assume que o fator de potência máximo é o da fase A
   fp_max = PM_GetFPa(np);
   //determina maior fator de potência entre as fases A e B
   if ((fp = PM_GetFPb(np)) > fp_max) fp_max = fp;
   //determina maior fator de potência entre as fases A, B e C
   if ((fp = PM_GetFPc(np)) > fp_max) fp_max = fp;
   //retorna fator de potência máximo
   return(fp_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetFPf_min(int np)
   {
   //variáveis locais
   double fp, fp_min;

   //assume que o fator de potência mínimo é o da fase A
   fp_min = PM_GetFPa(np);
   //determina menor fator de potência entre as fases A e B
   if ((fp = PM_GetFPb(np)) < fp_min) fp_min = fp;
   //determina menor fator de potência entre as fases A, B e C
   if ((fp = PM_GetFPc(np)) < fp_min) fp_min = fp;
   //retorna fator de potência máximo
   return(fp_min);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetFPn(int np)
   {
   //retorna fator de potência do neutro
   return(FatorPotencia(PM_GetSn_mva(np)));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowSup::PM_GetIa_amp(int np)
   {
   //retorna corrente da fase A (em A)
   return(Corrente_amp(PM_GetSa_mva(np), Eqbar->pbarra->resflow->Van_kv[np]));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowSup::PM_GetIb_amp(int np)
   {
   //retorna corrente da fase B (em A)
   return(Corrente_amp(PM_GetSb_mva(np), Eqbar->pbarra->resflow->Vbn_kv[np]));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowSup::PM_GetIc_amp(int np)
   {
   //retorna corrente da fase C (em A)
   return(Corrente_amp(PM_GetSc_mva(np), Eqbar->pbarra->resflow->Vcn_kv[np]));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowSup::PM_GetIf_amp(int np, int nf)
   {
   //retorna corrente da fase indicada (em A)
   return(Corrente_amp(PM_GetSf_mva(np, nf), Eqbar->pbarra->resflow->Vfn_kv[np][nf]));
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetIf_amp_max(int np)
   {
   //variáveis locais
   double cor, cor_max;

   //assume que a corrente da fase A é maior
   cor_max = Abs(PM_GetIa_amp(np));
   //determina a maior corrente entre as fases A e B
   cor = Abs(PM_GetIb_amp(np));
   if (cor > cor_max) cor_max = cor;
   //determina a maior corrente entre as fases A, B e C
   cor = Abs(PM_GetIc_amp(np));
   if (cor > cor_max) cor_max = cor;
   return(cor_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetIf_amp_min(int np)
   {
   //variáveis locais
   double cor, cor_min;

   //assume que a corrente da fase A é maior
   cor_min = Abs(PM_GetIa_amp(np));
   //determina a maior corrente entre as fases A e B
   cor = Abs(PM_GetIb_amp(np));
   if (cor < cor_min) cor_min = cor;
   //determina a maior corrente entre as fases A, B e C
   cor = Abs(PM_GetIc_amp(np));
   if (cor < cor_min) cor_min = cor;
   return(cor_min);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowSup::PM_GetIn_amp(int np)
   {
   //retorna corrente da fase indicada (em A)
   return(Corrente_amp(PM_GetSn_mva(np), Eqbar->pbarra->resflow->Vnt_kv[np]));
   }

//---------------------------------------------------------------------------
int __fastcall TResFlowSup::PM_GetMaxPat(void)
   {
   return(PD.max_pat);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetP_mw(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_GetP_mva() - patamar inválido");
   return(S_mva[np].real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetPa_mw(int np)
   {
   return(PM_GetSa_mva(np).real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetPb_mw(int np)
   {
   return(PM_GetSb_mva(np).real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetPc_mw(int np)
   {
   return(PM_GetSc_mva(np).real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetPf_mw(int np, int nf)
   {
   return(PM_GetSf_mva(np, nf).real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetPf_mw_max(int np)
   {
   //variáveis locais
   double p, p_max;

   //assume que a Pot.Ativa da fase A é maior
   p_max = Abs(PM_GetPa_mw(np));
   //determina a maior Pot.Ativa entre as fases A e B
   p = Abs(PM_GetPb_mw(np));
   if (p > p_max) p_max = p;
   //determina a maior Pot.Ativa entre as fases A, B e C
   p = Abs(PM_GetPc_mw(np));
   if (p > p_max) p_max = p;
   return(p_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetPf_mw_min(int np)
   {
   //variáveis locais
   double p, p_min;

   //assume que a Pot.Ativa da fase A é maior
   p_min = Abs(PM_GetPa_mw(np));
   //determina a maior Pot.Ativa entre as fases A e B
   p = Abs(PM_GetPb_mw(np));
   if (p < p_min) p_min = p;
   //determina a maior Pot.Ativa entre as fases A, B e C
   p = Abs(PM_GetPc_mw(np));
   if (p < p_min) p_min = p;
   return(p_min);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetPn_mw(int np)
   {
   return(PM_GetSn_mva(np).real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetQ_mvar(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_GetQ_mva() - patamar inválido");
   return(S_mva[np].imag());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetQa_mvar(int np)
   {
   return(PM_GetSa_mva(np).imag());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetQb_mvar(int np)
   {
   return(PM_GetSb_mva(np).imag());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetQc_mvar(int np)
   {
   return(PM_GetSc_mva(np).imag());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetQf_mvar(int np, int nf)
   {
   return(PM_GetSf_mva(np, nf).imag());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetQf_mvar_max(int np)
   {
   //variáveis locais
   double q, q_max;

   //assume que a Pot.Ativa da fase A é maior
   q_max = Abs(PM_GetQa_mvar(np));
   //determina a maior Pot.Ativa entre as fases A e B
   q = Abs(PM_GetQb_mvar(np));
   if (q > q_max) q_max = q;
   //determina a maior Pot.Ativa entre as fases A, B e C
   q = Abs(PM_GetQc_mvar(np));
   if (q > q_max) q_max = q;
   return(q_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetQf_mvar_min(int np)
   {
   //variáveis locais
   double q, q_min;

   //assume que a Pot.Ativa da fase A é maior
   q_min = Abs(PM_GetQa_mvar(np));
   //determina a maior Pot.Ativa entre as fases A e B
   q = Abs(PM_GetQb_mvar(np));
   if (q < q_min) q_min = q;
   //determina a maior Pot.Ativa entre as fases A, B e C
   q = Abs(PM_GetQc_mvar(np));
   if (q < q_min) q_min = q;
   return(q_min);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetQn_mvar(int np)
   {
   return(PM_GetSn_mva(np).imag());
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowSup::PM_GetS_mva(int np)
   {
   //retorna somatória da potência nas 3 fases
   return(PM_GetSa_mva(np) + PM_GetSb_mva(np) + PM_GetSc_mva(np));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowSup::PM_GetSa_mva(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_GetSa_mva() - patamar inválido");
   //retorna potência na fase A relativa à barra nb
   return(potencia[np].sa);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowSup::PM_GetSb_mva(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_GetSb_mva() - patamar inválido");
   //retorna potência na fase B relativa à barra nb
   return(potencia[np].sb);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowSup::PM_GetSc_mva(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_GetSc_mva() - patamar inválido");
   //retorna potência na fase C relativa à barra nb
   return(potencia[np].sc);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowSup::PM_GetSf_mva(int np, int nf)
   {
   switch(nf)
      {
      case 0: return(PM_GetSa_mva(np));
      case 1: return(PM_GetSb_mva(np));
      case 2: return(PM_GetSc_mva(np));
      case 3: return(PM_GetSn_mva(np));
      }
   throw("TResFlowSup::PM_GetSf_mva() - fase inválida");
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetSf_mva_max(int np)
   {
   //variáveis locais
   double sf, sf_max;

   //assume maior potência igual à da primeira fase (fase A)
   sf_max = Abs(PM_GetSa_mva(np));
   //determina maior potência entre as fases A e B
   sf = Abs(PM_GetSb_mva(np));
   if (sf > sf_max) sf_max = sf;
   //determina maior potência entre as fases A, B e C
   sf = Abs(PM_GetSc_mva(np));
   if (sf > sf_max) sf_max = sf;
   return(sf_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowSup::PM_GetSf_mva_min(int np)
   {
   //variáveis locais
   double sf, sf_min;

   //assume maior potência igual à da primeira fase (fase A)
   sf_min = Abs(PM_GetSa_mva(np));
   //determina maior potência entre as fases A e B
   sf = Abs(PM_GetSb_mva(np));
   if (sf < sf_min) sf_min = sf;
   //determina maior potência entre as fases A, B e C
   sf = Abs(PM_GetSc_mva(np));
   if (sf < sf_min) sf_min = sf;
   return(sf_min);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowSup::PM_GetSn_mva(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_GetSn_mva() - patamar inválido");
   //retorna potência no neutro relativa à barra nb
   return(potencia[np].sn);
   }

//---------------------------------------------------------------------------
int __fastcall TResFlowSup::PM_GetTipoSup(void)
   {
   return(PD.tipo_sup);
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowSup::PM_SetCargto_pu(int np, double valor)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_SetCarregamento() - patamar inválido");
   //atualiza carregamento
   cargto[np] = valor;
   }
   
//---------------------------------------------------------------------------
void __fastcall TResFlowSup::PM_SetDiagnostico(int np, int valor)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_SetDiagnostico()");
   //atualiza diagnóstico
   diag[np] = valor;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowSup::PM_SetMaxPat(int max_pat)
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
void __fastcall TResFlowSup::PM_SetS_mva(int np, complex<double> s_mva)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_SetS_mva()");
   //divide a  Potência total entre as 3 fases
   Sa_mva[np] = s_mva / 3.;
   Sb_mva[np] = s_mva / 3.;
   Sc_mva[np] = s_mva / 3.;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowSup::PM_SetSa_mva(int np, complex<double> s_mva)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_SetSa_mva()");
   //zera valor muito pequeno
   s_mva = RoundZero(s_mva);
   //salva valor
   potencia[np].sa = s_mva;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowSup::PM_SetSb_mva(int np, complex<double> s_mva)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_SetSb_mva()");
   //zera valor muito pequeno
   s_mva = RoundZero(s_mva);
   //salva valor
   potencia[np].sb = s_mva;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowSup::PM_SetSc_mva(int np, complex<double> s_mva)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_SetSc_mva()");
   //zera valor muito pequeno
   s_mva = RoundZero(s_mva);
   //salva valor
   potencia[np].sc = s_mva;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowSup::PM_SetSf_mva(int np, int nf, complex<double> s_mva)
   {
   switch(nf)
      {
      case 0:  Sa_mva[np] = s_mva; break;
      case 1:  Sb_mva[np] = s_mva; break;
      case 2:  Sc_mva[np] = s_mva; break;
      case 3:  Sn_mva[np] = s_mva; break;
      default: throw("TResFlowSup::PM_SetSf_mva() - fase inválida");
      }
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowSup::PM_SetSn_mva(int np, complex<double> s_mva)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowSup::PM_SetSn_mva()");
   //zera valor muito pequeno
   s_mva = RoundZero(s_mva);
   //salva valor
   potencia[np].sn = s_mva;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowSup::PM_SetTipoSup(int tipo_sup)
   {
   PD.tipo_sup = tipo_sup;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowSup::Reinicia(void)
   {
   for (int np = 0; np < Max_pat; np++) Inicia(np);
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowSup::VetorDelete(void)
   {
   if (cargto)   {delete[] cargto;   cargto   = NULL;}
   if (diag)     {delete[] diag;     diag     = NULL;}
   if (potencia) {delete[] potencia; potencia = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowSup::VetorNew(void)
   {
   //cria vetores cargto, diag e sentido
   cargto  = new float[Max_pat];
   diag    = new int[Max_pat];
   //cria vetor de strPOTENCIA
   potencia = new strPOTENCIA[Max_pat];
   //inicia valores
   Reinicia();
   }

//---------------------------------------------------------------------------
//eof


