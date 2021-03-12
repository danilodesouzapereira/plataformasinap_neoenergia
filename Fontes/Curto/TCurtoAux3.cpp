//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TCurtoAux3.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Curto\TBarraDefeito.h"
#include "..\Comum13\VTGeralC.h"
#include "..\Rede3\TBarra.h"
#include "..\Rede3\TFase.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TCurtoAux3 * __fastcall NewObjCurtoAux3(VTApl *apl,
                                        int   num_bar_def)
   {
   return(new TCurtoAux3(apl, num_bar_def));
   }

//---------------------------------------------------------------------------
__fastcall TCurtoAux3::TCurtoAux3(VTApl *apl,
                                  int   num_bar_def)
                      :TCurtoAux(apl)
   {
   // Inicialização
   this->num_bar_def    = 0;
   lisBAR_DEF           = NULL;
   s3f_mva              = NULL;
   sft_mva              = NULL;
   zent0_ohm            = NULL;
   zent1_ohm            = NULL;
   zent0_so_trechos_ohm = NULL;
   zent1_so_trechos_ohm = NULL;
   idef_assim           = NULL;
   idef                 = NULL;

   // Aloca memória
   AlocaMemoria(num_bar_def);
   }

//---------------------------------------------------------------------------
__fastcall TCurtoAux3::~TCurtoAux3(void)
   {
   // Libera memória
   LiberaMemoria();
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoAux3::AddBarraDefeito(TBarra *barra,
                                            int    tipo_curto,
                                            int    fases_curto)
{
// Variáveis locais
TBarraDefeito *bar_def;

// Proteção
if(lisBAR_DEF->Count >= num_bar_def) return(false);  // Erro

// Cria barra de defeito e adiciona  à lista
bar_def = new TBarraDefeito(barra, tipo_curto, fases_curto);
lisBAR_DEF->Add(bar_def);

// Retorna
return(true);
}

//---------------------------------------------------------------------------
void __fastcall TCurtoAux3::AlocaMemoria(int num_bar_def)
{
// Libera memória
LiberaMemoria();

// Proteção
if(num_bar_def == 0) return;

// Salva contador de barras
this->num_bar_def = num_bar_def;

// Cria lista de barras para defeito
lisBAR_DEF = new TList;

// Aloca memória para vetores
s3f_mva              = new complex<double>   [num_bar_def];
sft_mva              = new complex<double>   [num_bar_def];
zent0_ohm            = new complex<double>   [num_bar_def];
zent1_ohm            = new complex<double>   [num_bar_def];
zent0_so_trechos_ohm = new complex<double>   [num_bar_def];
zent1_so_trechos_ohm = new complex<double>   [num_bar_def];
idef_assim           = new double          * [num_bar_def];
idef                 = new complex<double> * [num_bar_def];

// Matrizes
for(int nb=0; nb < num_bar_def; nb++)
   {
   s3f_mva[nb]              = czero;
   sft_mva[nb]              = czero;
   zent0_ohm[nb]            = czero;
   zent1_ohm[nb]            = czero;
   zent0_so_trechos_ohm[nb] = czero;
   zent1_so_trechos_ohm[nb] = czero;
   idef_assim[nb]           = new double          [DIM2];
   idef[nb]                 = new complex<double> [DIM1];
   }

// Reinicia correntes
ReiniciaCorrentes();
}

//---------------------------------------------------------------------------
void __fastcall TCurtoAux3::CalculaCorrentesAssimetricas(void)
{
// Variáveis locais
bool            flag_3F, flag_FT;
int             fases_curto;
double          fmult;
complex<double> zent;
TBarraDefeito   *bar_def;

// Loop de barras
for(int nb=0; nb < lisBAR_DEF->Count; nb++)
   {
   // Zera correntes assimétricas
   idef_assim[nb][0] = 0.;
   idef_assim[nb][1] = 0.;
   idef_assim[nb][2] = 0.;

   // Analisa fases do defeito
   bar_def     = (TBarraDefeito *) lisBAR_DEF->Items[nb];
   fases_curto = bar_def->fases_curto;
   flag_3F     = IsComplexZero(zdef_fase_ohm) &&
                    ((fases_curto == ctABC)  || (fases_curto == ctABCN) ||
                     (fases_curto == ctABCT));
   flag_FT     = IsComplexZero(zdef_terra_ohm) &&
                    ((fases_curto == ctAN) || (fases_curto == ctAT));

   // Calcula correntes assimétricas (somente defeitos trifásico e fase-terra FRANCOS)
   if(flag_3F)  // Curto 3F
      {
      zent              = zent1_ohm[nb];
      fmult             = FatorAssimetria(zent);
      idef_assim[nb][0] = Abs(idef[nb][0]) * fmult;
      idef_assim[nb][1] = Abs(idef[nb][1]) * fmult;
      idef_assim[nb][2] = Abs(idef[nb][2]) * fmult;
      }
   if(flag_FT)  // Curto FT
      {
      zent              = (2. * zent1_ohm[nb]) + zent0_ohm[nb];
      fmult             = FatorAssimetria(zent);
      idef_assim[nb][0] = Abs(idef[nb][0]) * fmult;
      }
   } // for(nb)
}

//---------------------------------------------------------------------------
bool __fastcall TCurtoAux3::CalculaCorrentesDefeito(VTGeralC *geralC)
{
// Local variables
int             fases_curto, naux1, naux2, naux3, naux4;
complex<double> i1, i2, i3, v1, v2, v3;
TBarra          *barra;
TBarraDefeito   *bar_def;
TFase           *fases;

// Reinicia correntes
ReiniciaCorrentes();

// Loop de barras
for(int nb=0; nb < lisBAR_DEF->Count; nb++)
   {
   // Obtém barra de defeito (objeto TBarraDefeito)
   bar_def = (TBarraDefeito *) lisBAR_DEF->Items[nb];
   if(bar_def == NULL) continue;  // Proteção

   // Obtém barra (objeto TBarra) e fases de curto
   barra       = bar_def->barra;
   fases_curto = bar_def->fases_curto;

   // Obtém fases da barra
   fases = barra->fases;

   // Calcula corrente de defeito e atualiza potência de carga,
   // em função do tipo de defeito e do número de fases
   switch(fases_curto)
      {
      case ctAN:
         naux1 = fases->IndiceA();
         naux2 = fases->IndiceN();
         if((naux1 < 0) || (naux2 < 0)) return(false);
         v1                   = barra->vet_tensao_pu[naux1] - barra->vet_tensao_pu[naux2];
         v1                  *= 1000. * barra->VnomFase_kv(faseA); // Tensão   em [V]
         i1                   = v1 * ydef_terra_S;                 // Corrente em [A]
         idef[nb][0]          =  i1;
         idef[nb][3]          = -i1;
         geralC->carga_total += 0.001 * v1 * conj(i1);          // Atual. pot. de carga [kVA]
         break;

      case ctBN:
         naux1 = fases->IndiceB();
         naux2 = fases->IndiceN();
         if((naux1 < 0) || (naux2 < 0)) return(false);
         v1                   = barra->vet_tensao_pu[naux1] - barra->vet_tensao_pu[naux2];
         v1                  *= 1000. * barra->VnomFase_kv(faseB);
         i1                   = v1 * ydef_terra_S;
         idef[nb][1]          =  i1;
         idef[nb][3]          = -i1;
         geralC->carga_total += 0.001 * v1 * conj(i1);
         break;

      case ctCN:
         naux1 = fases->IndiceC();
         naux2 = fases->IndiceN();
         if((naux1 < 0) || (naux2 < 0)) return(false);
         v1                   = barra->vet_tensao_pu[naux1] - barra->vet_tensao_pu[naux2];
         v1                  *= 1000. * barra->VnomFase_kv(faseC);
         i1                   = v1 * ydef_terra_S;
         idef[nb][2]          =  i1;
         idef[nb][3]          = -i1;
         geralC->carga_total += 0.001 * v1 * conj(i1);
         break;

      case ctAT:
         naux1 = fases->IndiceA();
         if(naux1 < 0) return(false);
         v1                   = barra->vet_tensao_pu[naux1];
         v1                  *= 1000. * barra->VnomFase_kv(faseA);
         i1                   = v1 * ydef_terra_S;
         idef[nb][0]          = i1;
         idef[nb][4]          = i1;
         geralC->carga_total += 0.001 * v1 * conj(i1);
         break;

      case ctBT:
         naux1 = fases->IndiceB();
         if(naux1 < 0) return(false);
         v1                   = barra->vet_tensao_pu[naux1];
         v1                  *= 1000. * barra->VnomFase_kv(faseB);
         i1                   = v1 * ydef_terra_S;
         idef[nb][1]          = i1;
         idef[nb][4]          = i1;
         geralC->carga_total += 0.001 * v1 * conj(i1);
         break;

      case ctCT:
         naux1 = fases->IndiceC();
         if(naux1 < 0) return(false);
         v1                   = barra->vet_tensao_pu[naux1];
         v1                  *= 1000. * barra->VnomFase_kv(faseC);
         i1                   = v1 * ydef_terra_S;
         idef[nb][2]          = i1;
         idef[nb][4]          = i1;
         geralC->carga_total += 0.001 * v1 * conj(i1);
         break;

      case ctAB:
         naux1 = fases->IndiceA();
         naux2 = fases->IndiceB();
         if((naux1 < 0) || (naux2 < 0)) return(false);
         v1                   = barra->vet_tensao_pu[naux1] - barra->vet_tensao_pu[naux2];
         v1                  *= 1000. * barra->VnomFase_kv(faseA);
         v2                   = czero;
         i1                   = v1 * ydef_fase_S;
         i2                   = czero;
         idef[nb][0]          =  i1;
         idef[nb][1]          = -i1;
         geralC->carga_total += 0.001 * v1 * conj(i1);
         break;

      case ctBC:
         naux1 = fases->IndiceB();
         naux2 = fases->IndiceC();
         if((naux1 < 0) || (naux2 < 0)) return(false);
         v1                   = barra->vet_tensao_pu[naux1] - barra->vet_tensao_pu[naux2];
         v1                  *= 1000. * barra->VnomFase_kv(faseB);
         v2                   = czero;
         i1                   = v1 * ydef_fase_S;
         i2                   = czero;
         idef[nb][1]          =  i1;
         idef[nb][2]          = -i1;
         geralC->carga_total += 0.001 * v1 * conj(i1);
         break;

      case ctCA:
         naux1 = fases->IndiceC();
         naux2 = fases->IndiceA();
         if((naux1 < 0) || (naux2 < 0)) return(false);
         v1                   = barra->vet_tensao_pu[naux1] - barra->vet_tensao_pu[naux2];
         v1                  *= 1000. * barra->VnomFase_kv(faseC);
         v2                   = czero;
         i1                   = v1 * ydef_fase_S;
         i2                   = czero;
         idef[nb][2]          =  i1;
         idef[nb][0]          = -i1;
         geralC->carga_total += 0.001 * v1 * conj(i1);
         break;

      case ctABN:
         naux1 = fases->IndiceA();
         naux2 = fases->IndiceB();
         naux3 = fases->IndiceN();
         if((naux1 < 0) || (naux2 < 0) || (naux3 < 0)) return(false);
         v1                   = barra->vet_tensao_pu[naux1] - barra->vet_tensao_pu[naux3];
         v2                   = barra->vet_tensao_pu[naux2] - barra->vet_tensao_pu[naux3];
         v1                  *= 1000. * barra->VnomFase_kv(faseA);
         v2                  *= 1000. * barra->VnomFase_kv(faseB);
         i1                   = v1 * ydef_terra_S;
         i2                   = v2 * ydef_terra_S;
         idef[nb][0]          = i1;
         idef[nb][1]          = i2;
         idef[nb][3]          = -(i1 + i2);
         geralC->carga_total += 0.001 * (v1*conj(i1) + v2*conj(i2));
         break;

      case ctBCN:
         naux1 = fases->IndiceB();
         naux2 = fases->IndiceC();
         naux3 = fases->IndiceN();
         if((naux1 < 0) || (naux2 < 0) || (naux3 < 0)) return(false);
         v1                   = barra->vet_tensao_pu[naux1] - barra->vet_tensao_pu[naux3];
         v2                   = barra->vet_tensao_pu[naux2] - barra->vet_tensao_pu[naux3];
         v1                  *= 1000. * barra->VnomFase_kv(faseB);
         v2                  *= 1000. * barra->VnomFase_kv(faseC);
         i1                   = v1 * ydef_terra_S;
         i2                   = v2 * ydef_terra_S;
         idef[nb][1]          = i1;
         idef[nb][2]          = i2;
         idef[nb][3]          = -(i1 + i2);
         geralC->carga_total += 0.001 * (v1*conj(i1) + v2*conj(i2));
         break;

      case ctCAN:
         naux1 = fases->IndiceC();
         naux2 = fases->IndiceA();
         naux3 = fases->IndiceN();
         if((naux1 < 0) || (naux2 < 0) || (naux3 < 0)) return(false);
         v1                   = barra->vet_tensao_pu[naux1] - barra->vet_tensao_pu[naux3];
         v2                   = barra->vet_tensao_pu[naux2] - barra->vet_tensao_pu[naux3];
         v1                  *= 1000. * barra->VnomFase_kv(faseC);
         v2                  *= 1000. * barra->VnomFase_kv(faseA);
         i1                   = v1 * ydef_terra_S;
         i2                   = v2 * ydef_terra_S;
         idef[nb][2]          = i1;
         idef[nb][0]          = i2;
         idef[nb][3]          = -(i1 + i2);
         geralC->carga_total += 0.001 * (v1*conj(i1) + v2*conj(i2));
         break;

      case ctABT:
         naux1 = fases->IndiceA();
         naux2 = fases->IndiceB();
         if((naux1 < 0) || (naux2 < 0)) return(false);
         v1                   = barra->vet_tensao_pu[naux1];
         v2                   = barra->vet_tensao_pu[naux2];
         v1                  *= 1000. * barra->VnomFase_kv(faseA);
         v2                  *= 1000. * barra->VnomFase_kv(faseB);
         v3                   = v1 - v2;
         i3                   = v3 * yfase_fase_S;
         i1                   = (v1 * yfase_terra_S) + i3;
         i2                   = (v2 * yfase_terra_S) - i3;
         idef[nb][0]          = i1;
         idef[nb][1]          = i2;
         idef[nb][4]          = i1 + i2;
         geralC->carga_total += 0.001 * (v1*conj(i1) + v2*conj(i2));
         break;

      case ctBCT:
         naux1 = fases->IndiceB();
         naux2 = fases->IndiceC();
         if((naux1 < 0) || (naux2 < 0)) return(false);
         v1                   = barra->vet_tensao_pu[naux1];
         v2                   = barra->vet_tensao_pu[naux2];
         v1                  *= 1000. * barra->VnomFase_kv(faseB);
         v2                  *= 1000. * barra->VnomFase_kv(faseC);
         v3                   = v1 - v2;
         i3                   = v3 * yfase_fase_S;
         i1                   = (v1 * yfase_terra_S) + i3;
         i2                   = (v2 * yfase_terra_S) - i3;
         idef[nb][1]          = i1;
         idef[nb][2]          = i2;
         idef[nb][4]          = i1 + i2;
         geralC->carga_total += 0.001 * (v1*conj(i1) + v2*conj(i2));
         break;

      case ctCAT:
         naux1 = fases->IndiceC();
         naux2 = fases->IndiceA();
         if((naux1 < 0) || (naux2 < 0)) return(false);
         v1                   = barra->vet_tensao_pu[naux1];
         v2                   = barra->vet_tensao_pu[naux2];
         v1                  *= 1000. * barra->VnomFase_kv(faseC);
         v2                  *= 1000. * barra->VnomFase_kv(faseA);
         v3                   = v1 - v2;
         i3                   = v3 * yfase_fase_S;
         i1                   = (v1 * yfase_terra_S) + i3;
         i2                   = (v2 * yfase_terra_S) - i3;
         idef[nb][2]          = i1;
         idef[nb][0]          = i2;
         idef[nb][4]          = i1 + i2;
         geralC->carga_total += 0.001 * (v1*conj(i1) + v2*conj(i2));
         break;

      case ctABC:
         naux1 = fases->IndiceA();
         naux2 = fases->IndiceB();
         naux3 = fases->IndiceC();
         if((naux1 < 0) || (naux2 < 0) || (naux3 < 0)) return(false);
         v1                   = barra->vet_tensao_pu[naux1] - barra->vet_tensao_pu[naux2];
         v2                   = barra->vet_tensao_pu[naux2] - barra->vet_tensao_pu[naux3];
         v3                   = barra->vet_tensao_pu[naux3] - barra->vet_tensao_pu[naux1];
         v1                  *= 1000. * barra->VnomFase_kv(faseA);
         v2                  *= 1000. * barra->VnomFase_kv(faseB);
         v3                  *= 1000. * barra->VnomFase_kv(faseC);
         i1                   = v1 * ydef_fase_S;
         i2                   = v2 * ydef_fase_S;
         i3                   = v3 * ydef_fase_S;
         idef[nb][0]          = i1 - i3;
         idef[nb][1]          = i2 - i1;
         idef[nb][2]          = i3 - i2;
         geralC->carga_total += 0.001 * (v1*conj(i1) + v2*conj(i2) + v3*conj(i3));
         break;

      case ctABCN:
         naux1 = fases->IndiceA();
         naux2 = fases->IndiceB();
         naux3 = fases->IndiceC();
         naux4 = fases->IndiceN();
         if((naux1 < 0) || (naux2 < 0) || (naux3 < 0) || (naux4 < 0)) return(false);
         v1                   = barra->vet_tensao_pu[naux1] - barra->vet_tensao_pu[naux4];
         v2                   = barra->vet_tensao_pu[naux2] - barra->vet_tensao_pu[naux4];
         v3                   = barra->vet_tensao_pu[naux3] - barra->vet_tensao_pu[naux4];
         v1                  *= 1000. * barra->VnomFase_kv(faseA);
         v2                  *= 1000. * barra->VnomFase_kv(faseB);
         v3                  *= 1000. * barra->VnomFase_kv(faseC);
         i1                   = v1 * ydef_fase_S;
         i2                   = v2 * ydef_fase_S;
         i3                   = v3 * ydef_fase_S;
         idef[nb][0]          = i1;
         idef[nb][1]          = i2;
         idef[nb][2]          = i3;
         idef[nb][3]          = -(i1 + i2 + i3);
         geralC->carga_total += 0.001 * (v1*conj(i1) + v2*conj(i2) + v3*conj(i3));
         break;

      case ctABCT:
         naux1 = fases->IndiceA();
         naux2 = fases->IndiceB();
         naux3 = fases->IndiceC();
         if((naux1 < 0) || (naux2 < 0) || (naux3 < 0)) return(false);
         v1                   = barra->vet_tensao_pu[naux1];
         v2                   = barra->vet_tensao_pu[naux2];
         v3                   = barra->vet_tensao_pu[naux3];
         v1                  *= 1000. * barra->VnomFase_kv(faseA);
         v2                  *= 1000. * barra->VnomFase_kv(faseB);
         v3                  *= 1000. * barra->VnomFase_kv(faseC);
         i1                   = v1 * ydef_fase_S;
         i2                   = v2 * ydef_fase_S;
         i3                   = v3 * ydef_fase_S;
         idef[nb][0]          = i1;
         idef[nb][1]          = i2;
         idef[nb][2]          = i3;
         idef[nb][4]          = i1 + i2 + i3;
         geralC->carga_total += 0.001 * (v1*conj(i1) + v2*conj(i2) + v3*conj(i3));
         break;

      default:  // Erro
         break;
      }  // switch()
   } // for(nb)

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TCurtoAux3::CalculaPotenciasCurtoCircuito(VTGeralC *geralC)
{
// Variáveis locais
double          vnom_linha_kv, ybase;
complex<double> z0_pu, z1_pu;
TBarra          *bar_def;
TBarraDefeito   *bar_defeito;

// Loop de barras para defeito
for(int nb=0; nb < lisBAR_DEF->Count; nb++)
   {
   bar_defeito = (TBarraDefeito *) lisBAR_DEF->Items[nb];
   if(! bar_defeito->calculavel) continue; // Zentrada não pode ser calculada
   bar_def = bar_defeito->barra;
   if(bar_def->fases->NumFasesSemNT() != 3) continue; // Não foi calculada Zentrada para esta barra

   // Calcula potências de curto-circuito
   vnom_linha_kv = bar_def->VnomLinha_kv();
   ybase         = geralC->Sbase / (vnom_linha_kv * vnom_linha_kv);
   z1_pu         = zent1_ohm[nb] * ybase;
   z0_pu         = zent0_ohm[nb] * ybase;
   s3f_mva[nb]   = geralC->Sbase / conj(z1_pu);
   sft_mva[nb]   = 3. * geralC->Sbase / conj((2. * z1_pu) + z0_pu);
   } // for(nb)
}

//---------------------------------------------------------------------------
complex<double> __fastcall TCurtoAux3::CompSeqDireta(complex<double> va,
                                                     complex<double> vb,
                                                     complex<double> vc)
{
return((va + (calfa * vb) + (calfa2 * vc)) / 3.); // Componente de sequência direta
}

//---------------------------------------------------------------------------
complex<double> __fastcall TCurtoAux3::CompSeqInversa(complex<double> va,
                                                      complex<double> vb,
                                                      complex<double> vc)
{
return((va + (calfa2 * vb) + (calfa * vc)) / 3.); // Componente de sequência inversa
}

//---------------------------------------------------------------------------
complex<double> __fastcall TCurtoAux3::CompSeqZero(complex<double> va,
                                                   complex<double> vb,
                                                   complex<double> vc)
{
return((va + vb + vc) / 3.);  // Componente de sequência zero
}

//---------------------------------------------------------------------------
void __fastcall TCurtoAux3::LiberaMemoria(void)
{
// Variáveis locais
TBarraDefeito *bar_def;

// Proteção
if(num_bar_def == 0) return;

// Destroi 'lisBAR_DEF' e seus elementos
for(int nb=0; nb < lisBAR_DEF->Count; nb++)
   {
   bar_def = (TBarraDefeito *) lisBAR_DEF->Items[nb];
   delete bar_def;
   }
delete lisBAR_DEF;
lisBAR_DEF = NULL;

// Matrizes
for(int nb=0; nb < num_bar_def; nb++)
   {
   delete[] idef_assim[nb];
   delete[] idef[nb];
   }

// Vetores
delete[] s3f_mva;              s3f_mva              = NULL;
delete[] sft_mva;              sft_mva              = NULL;
delete[] zent0_ohm;            zent0_ohm            = NULL;
delete[] zent1_ohm;            zent1_ohm            = NULL;
delete[] zent0_so_trechos_ohm; zent0_so_trechos_ohm = NULL;
delete[] zent1_so_trechos_ohm; zent1_so_trechos_ohm = NULL;
delete[] idef_assim;           idef_assim           = NULL;
delete[] idef;                 idef                 = NULL;

// Contador de barras
num_bar_def = 0;
}

//---------------------------------------------------------------------------
void __fastcall TCurtoAux3::Reinicia(int num_bar_def)
{
// Aloca memória
AlocaMemoria(num_bar_def);
}

//---------------------------------------------------------------------------
void __fastcall TCurtoAux3::ReiniciaCorrentes(void)
   {
   for(int nb=0; nb < num_bar_def; nb++)
      {
      for(int n=0; n < DIM1; n++)
         {
         idef[nb][n] = czero;
         }
      for(int n=0; n < DIM2; n++)
         {
         idef_assim[nb][n] = 0.;
         }
      }
   }

//---------------------------------------------------------------------------
int __fastcall TCurtoAux3::TipoFasesCurto(int fases_sinap,
                                          int *fases_curto)
{
// Variáveis locais
int tipo_curto;

// Inicialização
tipo_curto   = -1;
*fases_curto = -1;

// Analisa as fases do curto
if     (fases_sinap == faseABC)  {tipo_curto = cuTRI; *fases_curto = ctABC;}
else if(fases_sinap == faseABCN) {tipo_curto = cuTRI; *fases_curto = ctABCN;}
else if(fases_sinap == faseABCT) {tipo_curto = cuTRI; *fases_curto = ctABCT;}

else if(fases_sinap == faseAB)   {tipo_curto = cu2F;  *fases_curto = ctAB;}
else if(fases_sinap == faseBC)   {tipo_curto = cu2F;  *fases_curto = ctBC;}
else if(fases_sinap == faseCA)   {tipo_curto = cu2F;  *fases_curto = ctCA;}

else if(fases_sinap == faseAN)   {tipo_curto = cuFTZ; *fases_curto = ctAN;}
else if(fases_sinap == faseBN)   {tipo_curto = cuFTZ; *fases_curto = ctBN;}
else if(fases_sinap == faseCN)   {tipo_curto = cuFTZ; *fases_curto = ctCN;}
else if(fases_sinap == faseAT)   {tipo_curto = cuFTZ; *fases_curto = ctAT;}
else if(fases_sinap == faseBT)   {tipo_curto = cuFTZ; *fases_curto = ctBT;}
else if(fases_sinap == faseCT)   {tipo_curto = cuFTZ; *fases_curto = ctCT;}

else if(fases_sinap == faseABN)  {tipo_curto = cu2FT; *fases_curto = ctABN;}
else if(fases_sinap == faseBCN)  {tipo_curto = cu2FT; *fases_curto = ctBCN;}
else if(fases_sinap == faseCAN)  {tipo_curto = cu2FT; *fases_curto = ctCAN;}
else if(fases_sinap == faseABT)  {tipo_curto = cu2FT; *fases_curto = ctABT;}
else if(fases_sinap == faseBCT)  {tipo_curto = cu2FT; *fases_curto = ctBCT;}
else if(fases_sinap == faseCAT)  {tipo_curto = cu2FT; *fases_curto = ctCAT;}

// Retorna
return(tipo_curto);
}

//---------------------------------------------------------------------------


