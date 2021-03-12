//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TReg32.h"
#include ".\TBarra.h"
#include ".\TFase.h"
#include "..\VTData.h"
#include "..\Comum13\TCurvaENERQCT.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Matriz\VTMatriz.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TReg32::TReg32(int    tipo,
                          int    num_fases_pri,
                          int    num_fases_sec,
                          TBarra *bar1,
                          TBarra *bar2)
                  :TTrafo32(tipo, num_fases_pri, num_fases_sec, bar1, bar2)
   {
   // Variáveis locais
   int num_taps = NumTaps();

   // Inicialização
   flag_reg32         = true;
   banda_morta_pu     = 0.;
   banda_morta_inv_pu = 0.;
   toler_tap_pu       = 1.e-10;  // Default
   bar_ref_12         = NULL;
   bar_ref_21         = NULL;
   curva_tap          = NULL;
   modelo             = regINDEF1;
   sub_modelo         = regINDEF2;

   // Aloca e inicializa memória (dados específicos de TReg32)
   vet_tap_bloqueado = new bool   [num_taps];
   vet_passo         = new int    [num_taps];
   vet_passo_prev    = new int    [num_taps];
   vet_tap_prev      = new double [num_taps];
   for(int n=0; n < num_taps; n++)
      {
      vet_passo[n]      = 0;
      vet_passo_prev[n] = 0;
      vet_tap_prev[n]   = 0.;
      }

   // Libera taps
   LiberaTaps();
   }

//---------------------------------------------------------------------------
__fastcall TReg32::~TReg32(void)
   {
   // Libera memória (dados específicos de TReg32)
   delete   curva_tap;
   delete[] vet_tap_bloqueado;
   delete[] vet_passo;
   delete[] vet_passo_prev;
   delete[] vet_tap_prev;
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TReg32::AUTODF_PADRAO(complex<double> *vet_TC)
{
// Variáveis locais
int             ind1, ind2, iter_max, linha1, linha2, linha3, num_modif,
                num_taps, passo_int;
double          aux1, aux2, passo_double, relacao, tap1, tap2, toler_tap,
                var_unit, vm1, vm2, vmod1, vmod2, vr1, vr2, vref_V,
                *vetor1, *vetor2;
complex<double> vlinha1, vlinha2;
VTMatriz        *matriz, *matriz_inv;

// Versão 1: ajusta cada tap (AB, BC, CA) de forma a conduzir as tensões de
//           linha do SECUNDÁRIO do autotrafo ao valor de referência da barra
//           PQV. Quando a barra PQV está distante do secundário, em geral este
//           ajuste resulta insuficiente para acertar as tensões na barra PQV
//           (vide Versão 2 desta função).

// Retorna se o modelo não for PADRAO_1 (demais modelos não implementados ainda)
if(modelo != reguPADRAO_1) return(false);

// Retorna se algum dos taps estiver bloqueado
num_taps = NumTaps();
for(int nt=0; nt < num_taps; nt++)
   {
   if(vet_tap_bloqueado[nt]) return(false);     // 'false': não modificou nenhum tap
   }

// Inicialização
iter_max   = 10;                                 // Em estudo
toler_tap  = 1.e-5;                              // Em estudo
vref_V     = vref * 1000. * bar1->VnomLinha();   // [V]
var_unit   = var_tensao_pu / double(num_passo);  // pu/passo
relacao    = vnom2 / vnom1;
matriz_inv = NULL;

// Cria matriz e vetores (termo conhecido e solução)
matriz = NewObjMatrizD(num_taps, num_taps);
vetor1 = new double[num_taps];
vetor2 = new double[num_taps];

// Salva valores atuais de passo e de tap
for(int nt=0; nt < num_taps; nt++)
   {
   vet_passo_prev[nt] = vet_passo[nt];
   vet_tap_prev[nt]   = vet_tap[nt];   
   }

// Loop de iterações (NR)
for(int iter=0; iter < iter_max; iter++)
   {
   // Monta matriz de derivadas (jacobiano) e termo conhecido
   // Loop de taps (fases)
   for(int nt=0; nt < num_taps; nt++)
      {
      switch(nt)
         {
         case 0:  // Tap AB
            ind1   = 0;
            ind2   = 1;
            linha1 = bar1->fases->LinhaFase(faseA);
            linha2 = bar1->fases->LinhaFase(faseB);
            linha3 = bar1->fases->LinhaFase(faseC);
            break;
         case 1:  // Tap BC
            ind1   = 1;
            ind2   = 2;
            linha1 = bar1->fases->LinhaFase(faseB);
            linha2 = bar1->fases->LinhaFase(faseC);
            linha3 = bar1->fases->LinhaFase(faseA);
            break;
         default:  // Tap CA
            ind1   = 2;
            ind2   = 0;
            linha1 = bar1->fases->LinhaFase(faseC);
            linha2 = bar1->fases->LinhaFase(faseA);
            linha3 = bar1->fases->LinhaFase(faseB);
            break;
         }
      vlinha1                =   vet_TC[linha1] - vet_TC[linha2];
      vlinha2                =   vet_TC[linha2] - vet_TC[linha3];
      vr1                    =   vlinha1.real();
      vm1                    =   vlinha1.imag();
      vmod1                  =   Abs(vlinha1);
      vr2                    =   vlinha2.real();
      vm2                    =   vlinha2.imag();
      vmod2                  =   Abs(vlinha2);
      tap1                   =   vet_tap[ind1];
      tap2                   =   vet_tap[ind2];
      aux1                   =   tap2 - 1.;
      aux2                   =   (vr1 * vr2) + (vm1 * vm2);
      matriz->D2[ind1][ind1] =   (tap1 * vmod1 * vmod1) - (aux1 * aux2);
      matriz->D2[ind1][ind2] = - (tap1 * aux2) + (aux1 * vmod2 * vmod2);
      vetor1[nt]             =   0.5 * (  (vref_V * vref_V)
                                        - (tap1 * tap1 * vmod1 * vmod1)
                                        + (2. * tap1 * aux1 * aux2)
                                        - (aux1 * aux1 * vmod2 * vmod2));
      }

   // Inverte matriz e determina incógnitas (correção nos taps)
   delete matriz_inv;
   matriz_inv = matriz->Inverte();
   if(matriz_inv == NULL) break;  // Erro
   matriz_inv->ProdutoMV(vetor1, vetor2);

   // Atualiza taps e verifica convergência
   num_modif = 0;
   for(int nt=0; nt < num_taps; nt++)
      {
      tap1         = vetor2[nt];
      vet_tap[nt] += tap1;
      if(fabs(tap1) > toler_tap) num_modif++;
      }
   if(num_modif == 0) break;
   } // for(iter)

// Discretiza taps
num_modif = 0;
if(matriz_inv != NULL)  // Sem erro anterior
   {
   for(int nt=0; nt < num_taps; nt++)
      {
      tap1         = vet_tap[nt] / relacao;  // Valor em pu
      passo_double = (tap1 - 1.) / var_unit;
      if     (passo_double > 0.) passo_int = int(passo_double + 0.5);
      else if(passo_double < 0.) passo_int = int(passo_double - 0.5);
      else                       passo_int = 0;
      if     (passo_int >   num_passo)  passo_int =  num_passo;
      else if(passo_int < (-num_passo)) passo_int = -num_passo;
      // Salva valores
      vet_passo[nt] = passo_int;
      tap1          = 1. + (double(passo_int) * var_unit);
      vet_tap[nt]   = tap1 * relacao;
      // Verifica alteração de tap
      // IMPORTANTE: por causa do NR3, deve-se usar o tap na comparação (e não o passo)
      if(vet_tap[nt] != vet_tap_prev[nt]) num_modif++;  // Houve alteração
      } // for(nt)
   } // if(matriz_inv)

// Libera memória
delete   matriz;
delete   matriz_inv;
delete[] vetor1;
delete[] vetor2;

// Retorna
if(num_modif == 0) return(false);  // Não houve alteração de tap
else               return(true);   // Houve alteração de tap
}
*/

//---------------------------------------------------------------------------
bool __fastcall TReg32::AjustaTap(complex<double> *vet_TC)
{
// Variáveis locais
bool   sucesso;
int    num_modif, num_taps, passo_int, sentido;
double relacao, tap_pu;

// Inicialização
num_modif = 0;
num_taps  = NumTaps();
relacao   = vnom2_kv / vnom1_kv;

// Regulador DOIS_SENTIDOS: determina sentido do fluxo de potência ativa
if(IsDOIS_SENTIDOS())
   {
   //sentido = SentidoPotencia(tpPOTENCIA_REATIVA, vet_TC);
   sentido = SentidoPotencia(tpPOTENCIA_ATIVA, vet_TC);
   if(sentido < 0) return(false);  // Erro
   }

// Loop de taps
for(int nt=0; nt < num_taps; nt++)
   {
   if(vet_tap_bloqueado[nt]) continue;  // Descarta tap bloqueado
   vet_passo_prev[nt] = vet_passo[nt];  // Salva valor atual de passo
   vet_tap_prev[nt]   = vet_tap[nt];    // Salva valor atual de tap
   sucesso            = false;          // Inicialização

   // Ajusta tap em função do modelo
   if(IsDOIS_SENTIDOS())
      {
      if(tipo == AUTOTRAFO_DF) sucesso = AjustaTapAUTODF(sentido, nt, vet_TC,
                                                         &passo_int, &tap_pu);
      else                     sucesso = AjustaTapDemaisTipos(sentido, nt, vet_TC,
                                                              &passo_int, &tap_pu);
      }
   else if(IsENERQCT())
      {
      // ENERQCT: AUTOTRAFODF não pode ser ENERQCT
      if(tipo != AUTOTRAFO_DF) sucesso = AjustaTapENERQCT(nt, vet_TC,
                                                          &passo_int, &tap_pu);
      }
   else if(IsPADRAO())
      {
      // PADRAO: impõe sentido 1 -> 2
      if(tipo == AUTOTRAFO_DF) sucesso = AjustaTapAUTODF(12, nt, vet_TC,
                                                         &passo_int, &tap_pu);
      else                     sucesso = AjustaTapDemaisTipos(12, nt, vet_TC,
                                                              &passo_int, &tap_pu);
      }
   if(! sucesso) return(false);  // Erro

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
bool __fastcall TReg32::AjustaTapAUTODF(int             sentido,
                                        int             nt,
                                        complex<double> *vet_TC,
                                        int             *passo_int,
                                        double          *tap_pu)
{
// Variáveis locais
int    linha1, linha2;
double vf_pu;
TFase  *fases;

// Versão 2: ajusta cada tap (AB, BC, CA) proporcionalmente ao desvio
//           de tensão de linha na barra PQV (AB, BC, CA) em relação
//           ao valor de referência

// Determina nó controlado, sua tensão de referência e sua tensão atual
if(sentido == 12) fases = bar_ref_12->fases;
else              fases = bar_ref_21->fases;

// Determina linha dos nós em [Ynodal]
if(nt == 0)
   {
   linha1 = fases->LinhaFase(faseA);
   linha2 = fases->LinhaFase(faseB);
   }
else if(nt == 1)
   {
   linha1 = fases->LinhaFase(faseB);
   linha2 = fases->LinhaFase(faseC);
   }
else
   {
   linha1 = fases->LinhaFase(faseC);
   linha2 = fases->LinhaFase(faseA);
   }
if((linha1 < 0) || (linha2 < 0)) return(false);
vf_pu = Abs(vet_TC[linha1] - vet_TC[linha2]) / sqrt(3.); // Tensões nodais em pu...

// Calcula tap
CalculaTap(sentido, nt, vf_pu, passo_int, tap_pu);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TReg32::AjustaTapDemaisTipos(int             sentido,
                                             int             nt,
                                             complex<double> *vet_TC,
                                             int             *passo_int,
                                             double          *tap_pu)
{
// Variáveis locais
int    linha;
double vf_pu;

// Determina nó controlado e sua tensão atual
if(sentido == 12) linha = bar_ref_12->fases->LinhaIndice(nt);
else              linha = bar_ref_21->fases->LinhaIndice(nt);
if(linha < 0) return(false);
vf_pu = Abs(vet_TC[linha]);

// Calcula tap
CalculaTap(sentido, nt, vf_pu, passo_int, tap_pu);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TReg32::AjustaTapENERQCT(int             nt,
                                         complex<double> *vet_TC,
                                         int             *passo_int,
                                         double          *tap_pu)
{
// Variáveis locais
int    linha;
double var_unit, vf_pu;

// Inicialização
var_unit = var_tensao_pu / double(num_passo);

// Obtém tensão atual na barra controlada
linha = bar_ref_12->fases->LinhaIndice(nt);
if(linha < 0) return(false);
vf_pu = Abs(vet_TC[linha]);

// Cria objeto TCurvaENERQCT, se ele não foi criado ainda
if(curva_tap == NULL) curva_tap = new TCurvaENERQCT;

// Ajusta tap de acordo com o modelo
*tap_pu = curva_tap->AjustaTap(modelo, vf_pu, var_unit, bar_ref_12, bar2, passo_int);

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TReg32::AlterouTap(void)
{
// Variáveis locais
int num_taps;

num_taps = NumTaps();
for(int nt=0; nt < num_taps; nt++)
   {
   if(vet_tap_bloqueado[nt]) continue;  // Não analisa tap bloqueado
   if(TapDiferente(vet_tap[nt], vet_tap_prev[nt])) return(true); // Houve alteração de tap
   }

return(false);  // Não houve alteração de tap
}

//---------------------------------------------------------------------------
void __fastcall TReg32::AtualizaTapNR3(int    nfa,
                                       double delta_tap_pu)
{
// Variáveis locais
double delta_tap_kV, passo_double, relacao, tap_pu;

// Inicialização
relacao = vnom2_kv / vnom1_kv;  // [kV]/[kV]

// Atualiza tap
if((nfa >= 0) && (nfa < NumTaps()))
   {
   // Salva tap anterior e atualiza tap
   delta_tap_kV       = delta_tap_pu * relacao;
   vet_tap_prev[nfa]  = vet_tap[nfa];
   vet_tap[nfa]      += delta_tap_kV;

   // Calcula passo (apenas para retorno ao Sinap)
   tap_pu         = vet_tap[nfa] / relacao;
   passo_double   = (tap_pu - 1.) * double(num_passo) / var_tensao_pu;
   vet_passo[nfa] = int(passo_double + 0.0001);
   }
}

//---------------------------------------------------------------------------
void __fastcall TReg32::CalculaTap(int    sentido,
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
int __fastcall TReg32::DeltaPassoInteiro(double delta)
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
bool __fastcall TReg32::IsDOIS_SENTIDOS(void)
{
if(modelo == regDOIS_SENTIDOS) return(true);
else                           return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TReg32::IsENERQCT(void)
{
if((modelo == regENERQCT1) ||
   (modelo == regENERQCT2) ||
   (modelo == regENERQCT3)) return(true);
else                        return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TReg32::IsPADRAO(void)
{
if(modelo == regPADRAO) return(true);
else                    return(false);
}

//---------------------------------------------------------------------------
void __fastcall TReg32::LiberaTaps(void)
{
for(int n=0; n < NumTaps(); n++)
   {
   vet_tap_bloqueado[n] = false;
   }
}

//---------------------------------------------------------------------------
int __fastcall TReg32::PM_GetDeltaPasso(int i)
{
if((i >= 0) && (i < NumTaps())) return(vet_passo[i] - vet_passo_prev[i]);
else                            return(0);
}

//---------------------------------------------------------------------------
bool __fastcall TReg32::PM_GetTapBloqueado(int i)
{
if((i >= 0) && (i < NumTaps())) return(vet_tap_bloqueado[i]);
else                            return(true);
}

//---------------------------------------------------------------------------
void __fastcall TReg32::PM_SetTapBloqueado(int  i,
                                           bool flag_bloqueio)
{
if((i >= 0) && (i < NumTaps())) vet_tap_bloqueado[i] = flag_bloqueio;
}

//---------------------------------------------------------------------------
int __fastcall TReg32::SentidoPotencia(int             tipo_potencia,
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
for(int nfa=0; nfa < fases1->NumFases(); nfa++)
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
bool __fastcall TReg32::TapDiferente(double tap,
                                     double tap_prev)
{
if(fabs(tap - tap_prev) > toler_tap_pu) return(true);  // Houve alteração de tap
else                                    return(false); // Não houve alteração de tap
}

//---------------------------------------------------------------------------
bool __fastcall TReg32::TipoImplementadoNR3(void)
{
switch(tipo)
   {
   case AUTOTRAFO_DF:
   case AUTOTRAFO_Y:
   case TRAFO_DD:
   case TRAFO_DY1:
   case TRAFO_DY11:
   case TRAFO_YD1:
   case TRAFO_YD11:
   case TRAFO_YY:
      return(true);

   default:
      return(false);
   }
}

//---------------------------------------------------------------------------


