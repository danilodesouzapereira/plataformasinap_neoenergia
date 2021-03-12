//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <Math.hpp>
#include "TResFlowBar.h"
#include "VTBarra.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Const.h"
#include "..\Constante\Fases.h"
#include "..\Constante\Trifasico.h"
#include "..\Zona\VTZona.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTResFlowBar* __fastcall NewObjResFlowBar(VTBarra *pbarra, int num_pat)
   {
   try{
      return(new TResFlowBar(pbarra, num_pat));
      }catch(Exception &e)
         {}
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TResFlowBar::TResFlowBar(VTBarra *pbarra, int num_pat)
   {
   //salva ponteiro p/ Barra e dimensão do vetor strTENSAO
   PD.pbarra = pbarra;
   Max_pat   = num_pat;
   }

//---------------------------------------------------------------------------
__fastcall TResFlowBar::~TResFlowBar(void)
   {
   //libera memória alocada p/ vetores
   VetorDelete();
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::CopiaAtributosDe(VTResFlowBar &ref)
   {
   //variáveis locais
   TResFlowBar &resflow = (TResFlowBar&)ref;

   //CUIDADO: NÃO copia dados acessados via property pois existe ponteiro p/ Barra
   //PD = resflow.PD;

   //proteção: verifica se vetores tem mesma dimensão
   if (Max_pat != ref.Max_pat)
      {//reinicia vetores
      Max_pat = ref.Max_pat;;
      }
   //copia vetor diag
   memcpy(diag, resflow.diag, (Max_pat * sizeof(int)));
   //copia vetor strTENSAO
   memcpy(tensao, resflow.tensao, (Max_pat * sizeof(strTENSAO)));
   //copia vetor strCORRENTE
   memcpy(corrente, resflow.corrente, (Max_pat * sizeof(strCORRENTE)));
   }

//---------------------------------------------------------------------------
bool __fastcall TResFlowBar::ExisteDiagnostico(int tipo_diag)
   {
   for (int np = 0; np < Max_pat; np++)
      {
      if (diag[np] == tipo_diag) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::Inicia(int np)
   {
   //diagnóstico
   Diagnostico[np] = diagISOLADO;
   //tensão
   Van_pu[np] = CZero();
   Vbn_pu[np] = CZero();
   Vcn_pu[np] = CZero();
   Vnt_pu[np] = CZero();
   //potência injetada
   Ia_amp[np] = CZero();
   Ib_amp[np] = CZero();
   Ic_amp[np] = CZero();
   In_amp[np] = CZero();
   //fases
   Fases      = 0;
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TResFlowBar::PM_GetBarra(void)
   {
   return(PD.pbarra);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetDesequilibrio(int np)
   {
   //variáveis locais
   double                numerador, denominador;
   double                deseq = 0.;
   const complex<double> alfa  = polar(1.,  (2.*M_PI)/3.);
   const complex<double> alfa2 = polar(1., -(2.*M_PI)/3.);

   //verifica se Zona trifásica
   if ((PD.pbarra->zona == NULL)||(! PD.pbarra->zona->Trifasica)) return(0.);
   numerador   = Abs(Van_pu[np] + (alfa2 * Vbn_pu[np]) + (alfa  * Vcn_pu[np]));
   denominador = Abs(Van_pu[np] + (alfa  * Vbn_pu[np]) + (alfa2 * Vcn_pu[np]));
   if (denominador > 0.) deseq = (numerador / denominador) * 100.;
   return(RoundTo(deseq, -5));
   }

//---------------------------------------------------------------------------
int __fastcall TResFlowBar::PM_GetDiagnostico(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_GetDiagnostico()");

   return(diag[np]);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetDRA(void)
   {
   //variáveis locais
   double dra = 0;

   //loop p/ todos patamares
   for (int np = 0; np < Max_pat; np++)
      {
      if (Diagnostico[np] == diagBOM) dra += 1.;
      }
   return(dra/Max_pat);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetDRC(void)
   {
   //variáveis locais
   double drc = 0;

   //loop p/ todos patamares
   for (int np = 0; np < Max_pat; np++)
      {
      if (Diagnostico[np] == diagRUIM) drc += 1.;
      }
   return(drc/Max_pat);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetDRP(void)
   {
   //variáveis locais
   double drp = 0;

   //loop p/ todos patamares
   for (int np = 0; np < Max_pat; np++)
      {
      if (Diagnostico[np] == diagREGULAR) drp += 1.;
      }
   return(drp/Max_pat);
   }

//---------------------------------------------------------------------------
int __fastcall TResFlowBar::PM_GetFases(void)
   {
   return(PD.fases);
   }

//---------------------------------------------------------------------------
int __fastcall TResFlowBar::PM_GetMaxPat(void)
   {
   return(PD.max_pat);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetIa_amp(int np)
   {
   //retorna potência na fase A
   return(corrente[np].ia_amp);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetIb_amp(int np)
   {
   //retorna potência na fase B
   return(corrente[np].ib_amp);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetIc_amp(int np)
   {
   //retorna potência na fase C
   return(corrente[np].ic_amp);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetIf_amp(int np, int nf)
   {
   switch(nf)
      {
      case 0: return(PM_GetIa_amp(np));
      case 1: return(PM_GetIb_amp(np));
      case 2: return(PM_GetIc_amp(np));
      case 3: return(PM_GetIn_amp(np));
      default: throw("TResFlowBar::PM_GetIf_amp() - fase inválida");
      }
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetIn_amp(int np)
   {
   //retorna potência no neutro
   return(corrente[np].in_amp);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetSf_mva(int np, int nf)
   {
   return((Vft_kv[np][nf] * conj(If_amp[np][nf])) * 1.e-3);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVab_kv(int np)
   {
   //proteção
   if (PD.pbarra->zona == NULL) return(CZero());
   //retorna tensão de linha Vab em kv
   return(PM_GetVab_pu(np) * PD.pbarra->zona->Vff_kv);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVab_pu(int np)
   {
   //variáveis locais
   complex<double> van_kv, vbn_kv;
   complex<double> vab_pu = CZero();

   //proteção
   //if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_GetVab_pu()");
   if ((np < 0)||(np >= Max_pat)||(PD.pbarra->zona == NULL)) return(vab_pu);
   //determina fator de relação entre tensão de fase e de linha da Zona
   van_kv = Tensao_pu(np, indFASE_A) * PD.pbarra->zona->Vfn_kv[indFASE_A];
   vbn_kv = Tensao_pu(np, indFASE_B) * PD.pbarra->zona->Vfn_kv[indFASE_B];
   vab_pu = (van_kv - vbn_kv) / PD.pbarra->zona->Vff_kv;

   return(vab_pu);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVan_kv(int np)
   {
   //proteção
   if (PD.pbarra->zona == NULL) return(CZero());
   //retorna tensão de fase Van em kv
   return(PM_GetVan_pu(np) * PD.pbarra->zona->Vfn_kv[indFASE_A]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVan_pu(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_GetVan_pu()");
   //retorna tensão da fase A em relação ao Neutro
   return(tensao[np].van);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVat_kv(int np)
   {
   //proteção
   if (PD.pbarra->zona == NULL) return(CZero());
   //retorna tensão da fase A em relação a terra em kV
   return(PM_GetVat_pu(np) * PD.pbarra->zona->Vfn_kv[indFASE_A]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVat_pu(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_GetVat_pu()");
   //retorna tensão da fase A em relação a terra em pu
   return(tensao[np].van + tensao[np].vnt);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVbc_kv(int np)
   {
   //proteção
   if (PD.pbarra->zona == NULL) return(CZero());
   //retorna tensão de linha Vbc em kv
   return(PM_GetVbc_pu(np) * PD.pbarra->zona->Vff_kv);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVbc_pu(int np)
   {
   //variáveis locais
   complex<double> vbn_kv, vcn_kv;
   complex<double> vbc_pu = CZero();

   //proteção
   //if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_GetVbc_pu()");
   if ((np < 0)||(np >= Max_pat)||(PD.pbarra->zona == NULL)) return(vbc_pu);
   vbn_kv = Tensao_pu(np, indFASE_B) * PD.pbarra->zona->Vfn_kv[indFASE_B];
   vcn_kv = Tensao_pu(np, indFASE_C) * PD.pbarra->zona->Vfn_kv[indFASE_C];
   vbc_pu = (vbn_kv - vcn_kv) / PD.pbarra->zona->Vff_kv;

   return(vbc_pu);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVbn_kv(int np)
   {
   //proteção
   if (PD.pbarra->zona == NULL) return(CZero());
   //retorna tensão de fase Vbn em kv
   return(PM_GetVbn_pu(np) * PD.pbarra->zona->Vfn_kv[indFASE_B]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVbn_pu(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_GetVbn_pu()");
   //retorna tensão da fase A em relação ao Neutro
   return(tensao[np].vbn);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVbt_kv(int np)
   {
   //proteção
   if (PD.pbarra->zona == NULL) return(CZero());
   //retorna tensão da fase B em relação a terra em kV
   return(PM_GetVbt_pu(np) * PD.pbarra->zona->Vfn_kv[indFASE_B]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVbt_pu(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_GetVbt_pu()");
   //retorna tensão da fase A em relação a terra emm pu
   return(tensao[np].vbn + tensao[np].vnt);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVca_kv(int np)
   {
   //proteção
   if (PD.pbarra->zona == NULL) return(CZero());
   //retorna tensão de linha Vca em kv
   return(PM_GetVca_pu(np) * PD.pbarra->zona->Vff_kv);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVca_pu(int np)
   {
   //variáveis locais
   complex<double> van_kv, vcn_kv;
   complex<double> vca_pu = CZero();

   //proteção
   //if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_GetVab_pu()");
   if ((np < 0)||(np >= Max_pat)||(PD.pbarra->zona == NULL)) return(vca_pu);
   //determina fator de relação entre tensão de fase e de linha da Zona
   van_kv = Tensao_pu(np, indFASE_A) * PD.pbarra->zona->Vfn_kv[indFASE_A];
   vcn_kv = Tensao_pu(np, indFASE_C) * PD.pbarra->zona->Vfn_kv[indFASE_C];
   vca_pu = (vcn_kv - van_kv) / PD.pbarra->zona->Vff_kv;

   return(vca_pu);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVcn_kv(int np)
   {
   //proteção
   if (PD.pbarra->zona == NULL) return(CZero());
   //retorna tensão de fase Vbn em kv
   return(PM_GetVcn_pu(np) * PD.pbarra->zona->Vfn_kv[indFASE_C]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVcn_pu(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_GetVcn_pu()");
   //retorna tensão da fase A em relação ao Neutro em pu
   return(tensao[np].vcn);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVct_kv(int np)
   {
   //proteção
   if (PD.pbarra->zona == NULL) return(CZero());
   //retorna tensão da fase C em relação a terra em kV
   return(PM_GetVct_pu(np) * PD.pbarra->zona->Vfn_kv[indFASE_C]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVct_pu(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_GetVct_pu()");
   //retorna tensão da fase A em relação a terra em pu
   return(tensao[np].vcn + tensao[np].vnt);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVff_kv(int np, int nf)
   {
   //proteção
   if (PD.pbarra->zona == NULL) return(CZero());
   //retorna tensão de linha indicada em kv
   return(PM_GetVff_pu(np, nf) * PD.pbarra->zona->Vff_kv);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVff_pu(int np, int nf)
   {
   switch(nf)
      {
      case 0: return(Vab_pu[np]);
      case 1: return(Vbc_pu[np]);
      case 2: return(Vca_pu[np]);
      default: throw("PM_GetVff_pu[][]");
      }
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVff_kv_max(int np)
   {
   //proteção
   if (PD.pbarra->zona == NULL) return(0.);
   //retorna tensão de linha máxima em kV
   return(PM_GetVff_pu_max(np) * PD.pbarra->zona->Vff_kv);
   }
/*
//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVff_pu_max(int np)
   {
   //variáveis locais
   double vpu_max;

   //proteção
   if      (PD.pbarra->zona == NULL) return(0.);
   //inicia vpu_max c/ a Vff de uma fase existente
   if      (PD.pbarra->zona->ExisteFase(faseAB)) vpu_max = Abs(Vab_pu[np]);
   else if (PD.pbarra->zona->ExisteFase(faseBC)) vpu_max = Abs(Vbc_pu[np]);
   else if (PD.pbarra->zona->ExisteFase(faseCA)) vpu_max = Abs(Vca_pu[np]);
   else return(0.);
   //determaxa Vff máxima em pu
   if (PD.pbarra->zona->ExisteFase(faseAB)) vpu_max = max(vpu_max, Abs(Vab_pu[np]));
   if (PD.pbarra->zona->ExisteFase(faseBC)) vpu_max = max(vpu_max, Abs(Vbc_pu[np]));
   if (PD.pbarra->zona->ExisteFase(faseCA)) vpu_max = max(vpu_max, Abs(Vca_pu[np]));
   return(vpu_max);
   }
*/
//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVff_pu_max(int np)
   {
   //variáveis locais
   double vpu_max = 0.;

   //proteção
   if      (PD.pbarra->zona == NULL) return(0.);
   //inicia vpu_max c/ a Vff de uma fase existente
   if      ((Fases & faseAB) == faseAB) vpu_max = Abs(Vab_pu[np]);
   else if ((Fases & faseBC) == faseBC) vpu_max = Abs(Vbc_pu[np]);
   else if ((Fases & faseCA) == faseCA) vpu_max = Abs(Vca_pu[np]);
   else return(0.);
   //determaxa Vff máxima em pu
   if ((Fases & faseAB) == faseAB) vpu_max = max(vpu_max, Abs(Vab_pu[np]));
   if ((Fases & faseBC) == faseBC) vpu_max = max(vpu_max, Abs(Vbc_pu[np]));
   if ((Fases & faseCA) == faseCA) vpu_max = max(vpu_max, Abs(Vca_pu[np]));
   return(vpu_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVff_kv_min(int np)
   {
   //proteção
   if (PD.pbarra->zona == NULL) return(0.);
   //retorna tensão de linha mínima em kV
   return(PM_GetVff_pu_min(np) * PD.pbarra->zona->Vff_kv);
   }
/*
//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVff_pu_min(int np)
   {
   //variáveis locais
   double vpu_min;

   //proteção
   if      (PD.pbarra->zona == NULL) return(0.);
   //inicia vpu_min c/ uma tensão de linha existente
   if      (PD.pbarra->zona->ExisteFase(faseAB)) vpu_min = Abs(Vab_pu[np]);
   else if (PD.pbarra->zona->ExisteFase(faseBC)) vpu_min = Abs(Vbc_pu[np]);
   else if (PD.pbarra->zona->ExisteFase(faseCA)) vpu_min = Abs(Vca_pu[np]);
   else return(0.);
   //determina tensão de linha mínima em pu
   if (PD.pbarra->zona->ExisteFase(faseAB)) vpu_min = min(vpu_min, Abs(Vab_pu[np]));
   if (PD.pbarra->zona->ExisteFase(faseBC)) vpu_min = min(vpu_min, Abs(Vbc_pu[np]));
   if (PD.pbarra->zona->ExisteFase(faseCA)) vpu_min = min(vpu_min, Abs(Vca_pu[np]));
   return(vpu_min);
   }
*/
//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVff_pu_min(int np)
   {
   //variáveis locais
   double vpu_min = 0.;

   //proteção
   if      (PD.pbarra->zona == NULL) return(0.);
   //inicia vpu_min c/ uma tensão de linha existente
   if      ((Fases & faseAB) == faseAB) vpu_min = Abs(Vab_pu[np]);
   else if ((Fases & faseBC) == faseBC) vpu_min = Abs(Vbc_pu[np]);
   else if ((Fases & faseCA) == faseCA) vpu_min = Abs(Vca_pu[np]);
   //determina tensão de linha mínima em pu
   if ((Fases & faseAB) == faseAB) vpu_min = min(vpu_min, Abs(Vab_pu[np]));
   if ((Fases & faseBC) == faseBC) vpu_min = min(vpu_min, Abs(Vbc_pu[np]));
   if ((Fases & faseCA) == faseCA) vpu_min = min(vpu_min, Abs(Vca_pu[np]));
   return(vpu_min);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVfn_kv(int np, int nf)
   {
   //proteção
   if (PD.pbarra->zona == NULL) return(CZero());
   //retorna tensão da fase indicada em kV
   return(PM_GetVfn_pu(np, nf) * PD.pbarra->zona->Vfn_kv[nf]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVfn_pu(int np, int nf)
   {
   switch(nf)
      {
      case 0: return(Van_pu[np]);
      case 1: return(Vbn_pu[np]);
      case 2: return(Vcn_pu[np]);
      case 3: return(Vnt_pu[np]);
      default: throw("PM_GetVfn_pu[][]");
      }
   }
/*
//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVfn_kv_max(int np)
   {
   //variáveis locais
   double vkv_max;
   double van_kv, vbn_kv, vcn_kv;
   //proteção
   if (PD.pbarra->zona == NULL) return(0.);

   //inicia vkv_max c/ a tensão de uma fase existente
   if      (PD.pbarra->zona->ExisteFase(faseA)) vkv_max = Abs(Van_kv[np]);
   else if (PD.pbarra->zona->ExisteFase(faseB)) vkv_max = Abs(Vbn_kv[np]);
   else if (PD.pbarra->zona->ExisteFase(faseC)) vkv_max = Abs(Vcn_kv[np]);
   else return(0.);
   //determina tensão de fase/neutro máxima em kV
   if (PD.pbarra->zona->ExisteFase(faseA)) vkv_max = max(vkv_max, Abs(Van_kv[np]));
   if (PD.pbarra->zona->ExisteFase(faseB)) vkv_max = max(vkv_max, Abs(Vbn_kv[np]));
   if (PD.pbarra->zona->ExisteFase(faseC)) vkv_max = max(vkv_max, Abs(Vcn_kv[np]));

   return(vkv_max);
   }
*/
//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVfn_kv_max(int np)
   {
   //variáveis locais
   double vkv_max;
   double van_kv, vbn_kv, vcn_kv;
   //proteção
   if (PD.pbarra->zona == NULL) return(0.);

   //inicia vkv_max c/ a tensão de uma fase existente
   if      ((Fases & faseA) == faseA) vkv_max = Abs(Van_kv[np]);
   else if ((Fases & faseB) == faseB) vkv_max = Abs(Vbn_kv[np]);
   else if ((Fases & faseC) == faseC) vkv_max = Abs(Vcn_kv[np]);
   else return(0.);
   //determina tensão de fase/neutro máxima em kV
   if ((Fases & faseA) == faseA) vkv_max = max(vkv_max, Abs(Van_kv[np]));
   if ((Fases & faseB) == faseB) vkv_max = max(vkv_max, Abs(Vbn_kv[np]));
   if ((Fases & faseC) == faseC) vkv_max = max(vkv_max, Abs(Vcn_kv[np]));

   return(vkv_max);
   }

/*
//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVfn_pu_max(int np)
   {
   //variáveis locais
   double vpu_max;

   //proteção
   if      (PD.pbarra->zona == NULL) return(0.);
   //inicia vpu_max c/ a Vfn de uma fase existente
   if      (PD.pbarra->zona->ExisteFase(faseA)) vpu_max = Abs(Van_pu[np]);
   else if (PD.pbarra->zona->ExisteFase(faseB)) vpu_max = Abs(Vbn_pu[np]);
   else if (PD.pbarra->zona->ExisteFase(faseC)) vpu_max = Abs(Vcn_pu[np]);
   else return(0.);
   //determaxa Vfn máxima em pu
   if (PD.pbarra->zona->ExisteFase(faseA)) vpu_max = max(vpu_max, Abs(Van_pu[np]));
   if (PD.pbarra->zona->ExisteFase(faseB)) vpu_max = max(vpu_max, Abs(Vbn_pu[np]));
   if (PD.pbarra->zona->ExisteFase(faseC)) vpu_max = max(vpu_max, Abs(Vcn_pu[np]));
   return(vpu_max);
   }
*/
//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVfn_pu_max(int np)
   {
   //variáveis locais
   double vpu_max;

   //proteção
   if      (PD.pbarra->zona == NULL) return(0.);
   //inicia vpu_max c/ a Vfn de uma fase existente
   if      ((Fases & faseA) == faseA) vpu_max = Abs(Van_pu[np]);
   else if ((Fases & faseB) == faseB) vpu_max = Abs(Vbn_pu[np]);
   else if ((Fases & faseC) == faseC) vpu_max = Abs(Vcn_pu[np]);
   else return(0.);
   //determaxa Vfn máxima em pu
   if ((Fases & faseA) == faseA) vpu_max = max(vpu_max, Abs(Van_pu[np]));
   if ((Fases & faseB) == faseB) vpu_max = max(vpu_max, Abs(Vbn_pu[np]));
   if ((Fases & faseC) == faseC) vpu_max = max(vpu_max, Abs(Vcn_pu[np]));
   return(vpu_max);
   }

/*
//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVfn_kv_min(int np)
   {
   //variáveis locais
   double vkv_min;
   double van_kv, vbn_kv, vcn_kv;
   //ptoteção
   if (PD.pbarra->zona == NULL) return(0.);

   //inicia vkv_min c/ a tensão de uma fase existente
   if      (PD.pbarra->zona->ExisteFase(faseA)) vkv_min = Abs(Van_kv[np]);
   else if (PD.pbarra->zona->ExisteFase(faseB)) vkv_min = Abs(Vbn_kv[np]);
   else if (PD.pbarra->zona->ExisteFase(faseC)) vkv_min = Abs(Vcn_kv[np]);
   else return(0.);
   //determina tensão de fase/neutro mínima em kV
   if (PD.pbarra->zona->ExisteFase(faseA)) vkv_min = min(vkv_min, Abs(Van_kv[np]));
   if (PD.pbarra->zona->ExisteFase(faseB)) vkv_min = min(vkv_min, Abs(Vbn_kv[np]));
   if (PD.pbarra->zona->ExisteFase(faseC)) vkv_min = min(vkv_min, Abs(Vcn_kv[np]));

   return(vkv_min);
   }
*/
//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVfn_kv_min(int np)
   {
   //variáveis locais
   double vkv_min;
   double van_kv, vbn_kv, vcn_kv;
   //ptoteção
   if (PD.pbarra->zona == NULL) return(0.);
   //inicia vkv_min c/ a tensão de uma fase existente
   if      ((Fases & faseA) == faseA) vkv_min = Abs(Van_kv[np]);
   else if ((Fases & faseB) == faseB) vkv_min = Abs(Vbn_kv[np]);
   else if ((Fases & faseC) == faseC) vkv_min = Abs(Vcn_kv[np]);
   else return(0.);
   //determina tensão de fase/neutro mínima em kV
   if ((Fases & faseA) == faseA) vkv_min = min(vkv_min, Abs(Van_kv[np]));
   if ((Fases & faseB) == faseB) vkv_min = min(vkv_min, Abs(Vbn_kv[np]));
   if ((Fases & faseC) == faseC) vkv_min = min(vkv_min, Abs(Vcn_kv[np]));

   return(vkv_min);
   }


/*
//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVfn_pu_min(int np)
   {
   //variáveis locais
   double vpu_min;

   //proteção
   if      (PD.pbarra->zona == NULL) return(0.);
   //inicia vpu_min c/ a tensão de uma fase existente
   if      (PD.pbarra->zona->ExisteFase(faseA)) vpu_min = Abs(Van_pu[np]);
   else if (PD.pbarra->zona->ExisteFase(faseB)) vpu_min = Abs(Vbn_pu[np]);
   else if (PD.pbarra->zona->ExisteFase(faseC)) vpu_min = Abs(Vcn_pu[np]);
   else return(0.);
   //determina tensão de fase/neutro mínima em pu
   if (PD.pbarra->zona->ExisteFase(faseA)) vpu_min = min(vpu_min, Abs(Van_pu[np]));
   if (PD.pbarra->zona->ExisteFase(faseB)) vpu_min = min(vpu_min, Abs(Vbn_pu[np]));
   if (PD.pbarra->zona->ExisteFase(faseC)) vpu_min = min(vpu_min, Abs(Vcn_pu[np]));
   return(vpu_min);
   }
*/
//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVfn_pu_min(int np)
   {
   //variáveis locais
   double vpu_min;

   //proteção
   if      (PD.pbarra->zona == NULL) return(0.);
   //inicia vpu_min c/ a tensão de uma fase existente
   if      ((Fases & faseA) == faseA) vpu_min = Abs(Van_pu[np]);
   else if ((Fases & faseB) == faseB) vpu_min = Abs(Vbn_pu[np]);
   else if ((Fases & faseC) == faseC) vpu_min = Abs(Vcn_pu[np]);
   else return(0.);
   //determina tensão de fase/neutro mínima em pu
   if ((Fases & faseA) == faseA) vpu_min = min(vpu_min, Abs(Van_pu[np]));
   if ((Fases & faseB) == faseB) vpu_min = min(vpu_min, Abs(Vbn_pu[np]));
   if ((Fases & faseC) == faseC) vpu_min = min(vpu_min, Abs(Vcn_pu[np]));
   return(vpu_min);
   }


//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVft_kv(int np, int nf)
   {
   //proteção
   if (PD.pbarra->zona == NULL) return(CZero());
   //retorna tensão fase/terra indicada em kV
   return(PM_GetVft_pu(np, nf) * PD.pbarra->zona->Vfn_kv[nf]);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVft_kv_max(int np)
   {
   //variáveis locais
   double vkv_max;

   //proteção
   if      (PD.pbarra->zona == NULL) return(0.);
   //inicia vkv_max c/ a Vfn de uma fase existente
   if      (PD.pbarra->zona->ExisteFase(faseA)) vkv_max = Abs(Vat_kv[np]);
   else if (PD.pbarra->zona->ExisteFase(faseB)) vkv_max = Abs(Vbt_kv[np]);
   else if (PD.pbarra->zona->ExisteFase(faseC)) vkv_max = Abs(Vct_kv[np]);
   else return(0.);
   //determaxa Vft máxima em pu
   if (PD.pbarra->zona->ExisteFase(faseA)) vkv_max = max(vkv_max, Abs(Vat_kv[np]));
   if (PD.pbarra->zona->ExisteFase(faseB)) vkv_max = max(vkv_max, Abs(Vbt_kv[np]));
   if (PD.pbarra->zona->ExisteFase(faseC)) vkv_max = max(vkv_max, Abs(Vct_kv[np]));

   return(vkv_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVft_kv_min(int np)
   {
   //variáveis locais
   double vkv_min;

   //proteção
   if      (PD.pbarra->zona == NULL) return(0.);
   //inicia vkv_min c/ a Vft de uma fase existente
   if      (PD.pbarra->zona->ExisteFase(faseA)) vkv_min = Abs(Vat_kv[np]);
   else if (PD.pbarra->zona->ExisteFase(faseB)) vkv_min = Abs(Vbt_kv[np]);
   else if (PD.pbarra->zona->ExisteFase(faseC)) vkv_min = Abs(Vct_kv[np]);
   else return(0.);
   //determina tensão de fase/neutro mínima em pu
   if (PD.pbarra->zona->ExisteFase(faseA)) vkv_min = min(vkv_min, Abs(Vat_kv[np]));
   if (PD.pbarra->zona->ExisteFase(faseB)) vkv_min = min(vkv_min, Abs(Vbt_kv[np]));
   if (PD.pbarra->zona->ExisteFase(faseC)) vkv_min = min(vkv_min, Abs(Vct_kv[np]));
   return(vkv_min);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVft_pu(int np, int nf)
   {
   //determina tensão fase/terra indicada em pu
   switch(nf)
      {
      case 0: return(Vat_pu[np]);
      case 1: return(Vbt_pu[np]);
      case 2: return(Vct_pu[np]);
      case 3: return(Vnt_pu[np]);
      default: throw("PM_GetVft_pu[][]");
      }
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVft_pu_max(int np)
   {
   //variáveis locais
   double vpu_max;

   //proteção
   if      (PD.pbarra->zona == NULL) return(0.);
   //inicia vpu_max c/ a Vfn de uma fase existente
   if      (PD.pbarra->zona->ExisteFase(faseA)) vpu_max = Abs(Vat_pu[np]);
   else if (PD.pbarra->zona->ExisteFase(faseB)) vpu_max = Abs(Vbt_pu[np]);
   else if (PD.pbarra->zona->ExisteFase(faseC)) vpu_max = Abs(Vct_pu[np]);
   else return(0.);
   //determaxa Vft máxima em pu
   if (PD.pbarra->zona->ExisteFase(faseA)) vpu_max = max(vpu_max, Abs(Vat_pu[np]));
   if (PD.pbarra->zona->ExisteFase(faseB)) vpu_max = max(vpu_max, Abs(Vbt_pu[np]));
   if (PD.pbarra->zona->ExisteFase(faseC)) vpu_max = max(vpu_max, Abs(Vct_pu[np]));
   return(vpu_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowBar::PM_GetVft_pu_min(int np)
   {
   //variáveis locais
   double vpu_min;

   //proteção
   if      (PD.pbarra->zona == NULL) return(0.);
   //inicia vpu_min c/ a Vft de uma fase existente
   if      (PD.pbarra->zona->ExisteFase(faseA)) vpu_min = Abs(Vat_pu[np]);
   else if (PD.pbarra->zona->ExisteFase(faseB)) vpu_min = Abs(Vbt_pu[np]);
   else if (PD.pbarra->zona->ExisteFase(faseC)) vpu_min = Abs(Vct_pu[np]);
   else return(0.);
   //determina tensão de fase/neutro mínima em pu
   if (PD.pbarra->zona->ExisteFase(faseA)) vpu_min = min(vpu_min, Abs(Vat_pu[np]));
   if (PD.pbarra->zona->ExisteFase(faseB)) vpu_min = min(vpu_min, Abs(Vbt_pu[np]));
   if (PD.pbarra->zona->ExisteFase(faseC)) vpu_min = min(vpu_min, Abs(Vct_pu[np]));
   return(vpu_min);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVnt_kv(int np)
   {
   //proteção
   if (PD.pbarra->zona == NULL) return(CZero());
   //retorna tensão neutro/terra em kV
   return(PM_GetVnt_pu(np) * PD.pbarra->zona->Vfn_kv[indFASE_A]);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::PM_GetVnt_pu(int np)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_GetVnt_pu()");
   //retorna tensão da fase A em relação ao Neutro
   return(tensao[np].vnt);
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::PM_SetDiagnostico(int np, int valor)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_SetDiagnostico()");
   diag[np] = valor;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::PM_SetFases(int fases)
   {
   PD.fases = fases;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::PM_SetMaxPat(int max_pat)
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
void __fastcall TResFlowBar::PM_SetIa_amp(int np, complex<double> i_amp)
   {
   //zera valor muito pequeno
   i_amp = RoundZero(i_amp);
   //salva valor
   corrente[np].ia_amp = i_amp;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::PM_SetIb_amp(int np, complex<double> i_amp)
   {
   //zera valor muito pequeno
   i_amp = RoundZero(i_amp);
   //salva valor
   corrente[np].ib_amp = i_amp;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::PM_SetIc_amp(int np, complex<double> i_amp)
   {
   //zera valor muito pequeno
   i_amp = RoundZero(i_amp);
   //salva valor
   corrente[np].ic_amp = i_amp;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::PM_SetIf_amp(int np, int nf, complex<double> i_amp)
   {
   //salva valor
   switch(nf)
      {
      case 0: Ia_amp[np] = i_amp; break;
      case 1: Ib_amp[np] = i_amp; break;
      case 2: Ic_amp[np] = i_amp; break;
      case 3: In_amp[np] = i_amp; break;
      default: throw("TResFlowBar::i_amp() - fase inválida");
      }
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::PM_SetIn_amp(int np, complex<double> i_amp)
   {
   //zera valor muito pequeno
   i_amp = RoundZero(i_amp);
   //salva valor
   corrente[np].in_amp = i_amp;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::PM_SetVan_pu(int np, complex<double> v_pu)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_SetVan_pu()");
   //zera valor muito pequeno
   v_pu = RoundZero(v_pu, 1.e-10);
   //salva valor
   //salva tensão da fase A em relação ao Neutro
   tensao[np].van = v_pu;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::PM_SetVbn_pu(int np, complex<double> v_pu)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_SetVbn_pu()");
   //zera valor muito pequeno
   v_pu = RoundZero(v_pu, 1.e-10);
   //salva tensão da fase B em relação ao Neutro
   tensao[np].vbn = v_pu;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::PM_SetVcn_pu(int np, complex<double> v_pu)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_SetVcn_pu()");
   //zera valor muito pequeno
   v_pu = RoundZero(v_pu, 1.e-10);
   //salva tensão da fase C em relação ao Neutro
   tensao[np].vcn = v_pu;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::PM_SetVfn_pu(int np, int nf, complex<double> v_pu)
   {
   switch(nf)
      {
      case 0: Van_pu[np] = v_pu; break;
      case 1: Vbn_pu[np] = v_pu; break;
      case 2: Vcn_pu[np] = v_pu; break;
      case 3: Vnt_pu[np] = v_pu; break;
      default: throw("PM_SetVfn_pu[][]");
      }
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::PM_SetVnt_pu(int np, complex<double> v_pu)
   {
   //proteção
   if ((np < 0)||(np >= Max_pat)) throw("TResFlowBar::PM_SetVnt_pu()");
   //zera valor muito pequeno
   v_pu = RoundZero(v_pu, 1.e-10);
   //salva tensão de Neutro em relação ao terra
   tensao[np].vnt = v_pu;
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::Reinicia(void)
   {
   for (int np = 0; np < Max_pat; np++) Inicia(np);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowBar::Tensao_pu(int np, int nf)
   {
   switch(nf)
      {
      case indFASE_A: return(complex<double>(tensao[np].van));
      case indFASE_B: return(complex<double>(tensao[np].vbn));
      case indFASE_C: return(complex<double>(tensao[np].vcn));
      case indNEUTRO: return(complex<double>(tensao[np].vnt));
      default:        return(CZero());
      }
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowBar::Vfn_pu_max_allpat(void)
   {
   //variáveis locais
   double vfn_pu, vfn_pu_max;

   //inicia valor máximo c/ o primeiro patamar
   vfn_pu_max = Vfn_pu_max[0];
   //loop p/ todos patamares
   for (int np = 0; np < Max_pat; np++)
      {
      vfn_pu = Vfn_pu_max[np];
      if (vfn_pu > vfn_pu_max) vfn_pu_max = vfn_pu;
      }
   return(vfn_pu_max);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowBar::Vfn_pu_min_allpat(void)
   {
   //variáveis locais
   double vfn_pu, vfn_pu_min;

   //inicia valor mínimo c/ o primeiro patamar
   vfn_pu_min = Vfn_pu_min[0];
   //loop p/ todos patamares
   for (int np = 0; np < Max_pat; np++)
      {
      vfn_pu = Vfn_pu_min[np];
      if (vfn_pu < vfn_pu_min) vfn_pu_min = vfn_pu;
      }
   return(vfn_pu_min);
   }
   
//---------------------------------------------------------------------------
void __fastcall TResFlowBar::VetorDelete(void)
   {
   if (diag)     {delete[] diag;     diag   = NULL;}
   if (tensao)   {delete[] tensao;   tensao = NULL;}
   if (corrente) {delete[] corrente; corrente = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowBar::VetorNew(void)
   {
   //cria vetor diag
   diag = new int[Max_pat];
   //cria vetor de strTENSAO
   tensao = new strTENSAO[Max_pat];
   //cria vetor de strCORRENTE
   corrente = new strCORRENTE[Max_pat];
   //inicia valores
   Reinicia();
   }

//---------------------------------------------------------------------------
//eof


