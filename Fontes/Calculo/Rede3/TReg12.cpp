//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TReg12.h"
#include ".\TBarra.h"
#include ".\TFase.h"
#include "..\VTData.h"
#include "..\Comum13\TCurvaENERQCT.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Complexo\Complexo.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TReg12::TReg12(VTGeralC *geralC,
                          TBarra   *bar1,
                          TBarra   *bar2,
                          int      fases_int_pri,
                          int      fases_int_sec)
                  :TTrafo12(geralC, bar1, bar2, fases_int_pri, fases_int_sec)
{
// Inicialização
flag_reg12         = true;
banda_morta_pu     = 0.;
banda_morta_inv_pu = 0.;
toler_tap_pu       = 1.e-10;  // Default
bar_ref_12         = NULL;
bar_ref_21         = NULL;
curva_tap          = NULL;
modelo             = regINDEF1;
sub_modelo         = regINDEF2;
for(int nt=0; nt < 2; nt++)
   {
   vet_passo[nt]    = 0;
   vet_tap_prev[nt] = 0.;
	}
}

//---------------------------------------------------------------------------
__fastcall TReg12::~TReg12(void)
{
delete curva_tap;
}

//---------------------------------------------------------------------------
bool __fastcall TReg12::AjustaTap(complex<double> *vet_TC)
{
// Variáveis locais
int    f1, f2, linha1, linha2, num_modif, passo_int, sentido;
double relacao, tap_pu, vf_pu;
TBarra *bar_ref;

// Inicialização
num_modif = 0;
relacao   = vnom2_kv / vnom1_kv;
sentido   = 12;  // Reguladores != DOIS_SENTIDOS

// Regulador DOIS_SENTIDOS: determina sentido do fluxo de potência ativa
if(IsDOIS_SENTIDOS())
   {
   sentido = SentidoPotencia(tpPOTENCIA_ATIVA, vet_TC);
   if(sentido < 0) return(false);  // Erro
   }

// Obtém barra de tensão controlada de acordo com o sentido de fluxo
if(sentido == 12) bar_ref = bar_ref_12;
else              bar_ref = bar_ref_21;

// Loop de taps (todos os tipos têm 1 tap só, exceto AUTOTRAFO6_2T que tem 2 taps)
for(int nt=0; nt < NumTaps; nt++)
   {
   passo_int        = vet_passo[nt];  // Obtém valor atual de passo
   vet_tap_prev[nt] = vet_tap[nt];    // Salva valor atual de tap

   // Obtém fases e linhas em [Ynodal]
   ObtemFasesLinhasY(nt, bar_ref, &f1, &f2, &linha1, &linha2);

   // Obtém tensão na barra controlada
   vf_pu = ObtemTensaoBarraControlada(bar_ref, f1, f2, linha1, linha2, vet_TC);

   // Ajusta tap em função do modelo
   if(IsDOIS_SENTIDOS())
      {
      CalculaTap(sentido, nt, vf_pu, &passo_int, &tap_pu);
      }
   else if(IsENERQCT())
      {
      tap_pu = AjustaTapENERQCT(vf_pu, &passo_int);
      }
   else if(IsPADRAO())
      {
      // PADRAO: impõe sentido 1 -> 2
      CalculaTap(12, nt, vf_pu, &passo_int, &tap_pu);
      }
   else  // Erro
      {
      return(false);
      }

   // Salva valores
   vet_passo[nt] = passo_int;
   vet_tap[nt]   = tap_pu * relacao;

   // Verifica alteração de tap
   if(TapDiferente(vet_tap[nt], vet_tap_prev[nt])) num_modif++; // Houve alteração
   } // for(nt)

// Retorna
if(num_modif == 0) return(false);  // Não houve alteração de tap
else               return(true);   // Houve alteração de tap
}

//---------------------------------------------------------------------------
double __fastcall TReg12::AjustaTapENERQCT(double vf_pu,
                                           int    *passo_int)
{
// Variáveis locais
double tap_pu, var_unit;

// Inicialização
var_unit = var_tensao_pu / double(num_passo);

// Cria objeto TCurvaENERQCT, se ele não foi criado ainda
if(curva_tap == NULL) curva_tap = new TCurvaENERQCT;

// Ajusta tap de acordo com o modelo
tap_pu = curva_tap->AjustaTap(modelo, vf_pu, var_unit, bar_ref_12, bar2, passo_int);

// Retorna
return(tap_pu);
}

//---------------------------------------------------------------------------
bool __fastcall TReg12::AlterouTap(void)
{
// Variáveis locais
double abs_dif;

for(int nt=0; nt < NumTaps; nt++)
   {
   if(TapDiferente(vet_tap[nt], vet_tap_prev[nt])) return(true); // Houve alteração
   }

return(false);  // Não houve alteração de tap
}

//---------------------------------------------------------------------------
void __fastcall TReg12::AtualizaTapNR3(double delta_tap_pu,
                                       int    indice)
{
// Variáveis locais
double delta_tap_kV, passo_double, relacao, tap_pu;

// Inicialização
relacao = vnom2_kv / vnom1_kv;  // [kV]/[kV]

// Atualiza tap
if((indice >= 0) && (indice < NumTaps))
	{
   // Salva tap anterior e atualiza tap
   delta_tap_kV          = delta_tap_pu * relacao;
	vet_tap_prev[indice]  = vet_tap[indice];
	vet_tap[indice]      += delta_tap_kV;

   // Calcula passo (apenas para retorno ao Sinap)
   tap_pu            = vet_tap[indice] / relacao;
   passo_double      = (tap_pu - 1.) * double(num_passo) / var_tensao_pu;
   vet_passo[indice] = int(passo_double + 0.0001);
	}
}

//---------------------------------------------------------------------------
void __fastcall TReg12::CalculaTap(int    sentido,
                                   int    nt,
                                   double vf_pu,
                                   int    *passo_int,
                                   double *tap_pu)
{
// Variáveis locais
int    delta_int;
double banda_pu, delta, insensibilidade_pu, var_unit, vref, vref2_pu;

// Verifica se o sentido é reverso (21) e se o sub-modelo é 'regAUTO_UNI'
// (Caso 4 da Copel): regulador deve ser bypassado (retorna tap 1)
if((sentido == 21) && (sub_modelo == regAUTO_UNI))
   {
   *passo_int = 0;
   *tap_pu    = 1.;
   return;
   }

// Inicialização
var_unit = var_tensao_pu / double(num_passo);

// Verifica se a tensão calculada na barra controlada está dentro da
// "dead band"; nesse caso, retorna sem alterar o tap ('delta_int' = 0)
if(sentido == 12)  // Sentido direto
   {
   banda_pu = banda_morta_pu;
   vref     = vref_12_pu;
   }
else  // Sentido reverso
   {
   banda_pu = banda_morta_inv_pu;
   vref     = vref_21_pu;
   }
insensibilidade_pu = 0.5 * banda_pu;
if(fabs(vf_pu - vref) < insensibilidade_pu)
   {
   *passo_int = vet_passo[nt];
   *tap_pu    = 1. + (double(*passo_int) * var_unit);
   return;
   }

// Tensão atual fora da banda morta - calcula tap
if(vf_pu < vref) vref2_pu = vref - insensibilidade_pu; // Solicitação da Copel (HPS, 2018.08.28)
else             vref2_pu = vref + insensibilidade_pu;
// delta = (vref - vf_pu) / var_unit; // Sentido 1 -> 2
delta = (vref2_pu - vf_pu) / var_unit; // Sentido 1 -> 2
// Se o sentido é 21 [ tap (1/a):1 ], inverte a relação entre DeltaTap e DeltaV,
// desde que o sub-modelo NÃO SEJA 'regAUTO_FIXA' (Caso 3 da Copel). HPS, 2018.03.21.
if((sentido == 21) && (sub_modelo != regAUTO_FIXA)) delta = -delta;
delta_int  = DeltaPassoInteiro(delta);
*passo_int = vet_passo[nt] + delta_int;
if     (*passo_int >   num_passo)  *passo_int =  num_passo;
else if(*passo_int < (-num_passo)) *passo_int = -num_passo;
*tap_pu = 1. + (double(*passo_int) * var_unit);
}

//---------------------------------------------------------------------------
int __fastcall TReg12::DeltaPassoInteiro(double delta)
{
// Variáveis locais
int delta_int;

// Discretiza 'delta'
if     (delta > 0.) delta_int = int(delta + 0.0001) + 1;
else if(delta < 0.) delta_int = int(delta - 0.0001) - 1;
else                delta_int = 0;

return(delta_int);
}

//---------------------------------------------------------------------------
bool __fastcall TReg12::IsDOIS_SENTIDOS(void)
{
if(modelo == regDOIS_SENTIDOS) return(true);
else                           return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TReg12::IsENERQCT(void)
{
if((modelo == regENERQCT1) ||
   (modelo == regENERQCT2) ||
   (modelo == regENERQCT3)) return(true);
else                        return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TReg12::IsPADRAO(void)
{
if(modelo == regPADRAO) return(true);
else                    return(false);
}

//---------------------------------------------------------------------------
void __fastcall TReg12::ObtemFasesLinhasY(int    nt,
                                          TBarra *bar_ref,
                                          int    *f1,
                                          int    *f2,
                                          int    *linha1,
                                          int    *linha2)
{
// Determina linhas em Ynodal, de acordo com o tipo de regulador
switch(tipo)
   {
   case AUTOTRAFO4:
      *f1 = vet_fases_ord[2];              // Fase do secundário
      *f2 = vet_fases_ord[1];              // Fase comum
      break;

   case AUTOTRAFO6_1T:
      *f1 = vet_fases_ord[3];              // Fase 1 do secundário
      *f2 = vet_fases_ord[4];              // Fase 2 do secundário
      break;

   case AUTOTRAFO6_2T:
      if(nt == 0) *f1 = vet_fases_ord[3];  // Fase 1 do secundário
      else        *f1 = vet_fases_ord[4];  // Fase 2 do secundário
      *f2 = vet_fases_ord[2];              // Fase comum
      break;

   case TRAFO4:
      *f1 = vet_fases_ord[2];              // Fase 1 do secundário
      *f2 = vet_fases_ord[3];              // Fase 2 do secundário
      break;

   default:  // TRAFO5
      *f1 = vet_fases_ord[2];              // Fase 1 do secundário
      *f2 = vet_fases_ord[3];              // Fase 2 do secundário
      break;
   }

// Obtém linhas em [Ynodal]
*linha1 = bar_ref->fases->LinhaFase(*f1);
*linha2 = bar_ref->fases->LinhaFase(*f2);
}

//---------------------------------------------------------------------------
double __fastcall TReg12::ObtemTensaoBarraControlada(TBarra          *bar_ref,
                                                     int             f1,
                                                     int             f2,
                                                     int             linha1,
                                                     int             linha2,
                                                     complex<double> *vet_TC)
{
// Variáveis locais
double          vf_pu, vnom1_kv, vnom2_kv;
complex<double> czero(0., 0.);
complex<double> v1, v2;

// Calcula valor atual da tensão controlada
v1 = czero;
v2 = czero;
if(linha1 >= 0) v1 = vet_TC[linha1];
if(linha2 >= 0) v2 = vet_TC[linha2];
vnom1_kv = bar_ref->VnomFase_kv(f1);
vnom2_kv = bar_ref->VnomFase_kv(f2);
vf_pu    = Abs((v1 * vnom1_kv) - (v2 * vnom2_kv)) / VbaseBarRef_kv();

return(vf_pu);
}

//---------------------------------------------------------------------------
int __fastcall TReg12::SentidoPotencia(int             tipo_potencia,
                                       complex<double> *vet_TC)
{
// Variáveis locais
int             linha;
double          x12;
complex<double> czero(0., 0.);
TFase           *fases1, *fases2;

// Inicialização
fases1 = bar1->fases;
fases2 = bar2->fases;
x12    = 0.;

// Atualiza tensão nas barras terminais, para poder calcular correntes e
// potências no regulador
for(int nfa=0; nfa < fases1->NumFases(); nfa++)
   {
   linha = fases1->LinhaIndice(nfa);
   if(linha >= 0) bar1->vet_tensao_pu[nfa] = vet_TC[linha];
   else           bar1->vet_tensao_pu[nfa] = czero;
   }
for(int nfa=0; nfa < fases2->NumFases(); nfa++)
   {
   linha = fases2->LinhaIndice(nfa);
   if(linha >= 0) bar2->vet_tensao_pu[nfa] = vet_TC[linha];
   else           bar2->vet_tensao_pu[nfa] = czero;
   }

// Calcula regulador (correntes e potências)
Calcula(-1, NULL);  // Cálculo sem acumular perdas

// Loop de fases do primário, acumulando potência no sentido pri. -> sec.
for(int nfa=0; nfa < fases_pri->NumFases(); nfa++)
   {
   if     (tipo_potencia == tpPOTENCIA_ATIVA)   x12 += vet_s_kva[nfa].real();
   else if(tipo_potencia == tpPOTENCIA_REATIVA) x12 += vet_s_kva[nfa].imag();
   }

// Proteção para evitar que valores muito pequenos fiquem trocando de
// sinal entre iterações consecutivas (regulador da Copel em vazio).
// HPS, 2018.09.05.
if(fabs(x12) < 0.1) x12 = 0.;  // Tolerância de 100 W / 100 VAr

// Retorna sentido de fluxo da potência trifásica (ativa ou reativa)
if(x12 >= 0.) return(12);  // Sentido 1 -> 2
else          return(21);  // Sentido 2 -> 1
}

//---------------------------------------------------------------------------
bool __fastcall TReg12::TapDiferente(double tap,
                                     double tap_prev)
{
if(fabs(tap - tap_prev) > toler_tap_pu) return(true);  // Houve alteração de tap
else                                    return(false); // Não houve alteração de tap
}

//---------------------------------------------------------------------------
bool __fastcall TReg12::TipoImplementadoNR3(void)
{
switch(tipo)
   {
   case AUTOTRAFO4:
   case AUTOTRAFO6_1T:
   case AUTOTRAFO6_2T:
      return(true);

   default:
      return(false);
   }
}

//---------------------------------------------------------------------------
double __fastcall TReg12::VbaseBarRef_kv()
   {
   double valor;

   switch(tipo)
      {
      case AUTOTRAFO4:
      case AUTOTRAFO6_2T:
      case TRAFO4:
         if(NeutroSec()) valor = bar_ref_12->VnomFase_kv(fases_sec->Fase(0));
         else            valor = bar_ref_12->VnomLinha_kv();
         break;

      default:  // AUTOTRAFO6_1T ou TRAFO5
         valor = bar_ref_12->VnomLinha_kv();
         break;
      }

   return(valor);
   }

//---------------------------------------------------------------------------


