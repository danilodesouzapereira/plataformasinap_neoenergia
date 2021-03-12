//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TReg2.h"
#include ".\TBarra.h"
#include "..\VTData.h"
#include "..\Comum13\TCurvaENERQCT.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Complexo\Complexo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TReg2::TReg2(TBarra *bar1,
                        TBarra *bar2,
                        double Sbase_mva)
                 :TTrafo2(bar1, bar2, Sbase_mva)
   {
   // Inicialização
   flag_regulador     = true;
   banda_morta_pu     = 0.;
   banda_morta_inv_pu = 0.;
   passo_int          = 0;
   passo_int_prev     = 0;
   tap_prev           = 1.;
   toler_tap_pu       = 1.e-10;  // Default
   tap_bloqueado      = false;
   bar_ref_12         = NULL;
   bar_ref_21         = NULL;
   curva_tap          = NULL;
   modelo             = regINDEF1;
   sub_modelo         = regINDEF2;
   }

//---------------------------------------------------------------------------
__fastcall TReg2::~TReg2(void)
   {
   delete curva_tap;
   }

//---------------------------------------------------------------------------
bool __fastcall TReg2::AjustaTap(complex<double> *vet_TC)
{
// Variáveis locais
bool sucesso;
int  sentido;

// Retorna se o tap estiver bloqueado para ajuste
if(tap_bloqueado) return(false);

// Inicialização
passo_int_prev = passo_int;  // Salva valor atual de passo
tap_prev       = tap;        // Salva valor atual de tap
sucesso        = false;      // Inicialização

// Ajusta tap em função do modelo
if(IsDOIS_SENTIDOS())
   {
   //sentido = SentidoPotencia(tpPOTENCIA_REATIVA, vet_TC);
   sentido = SentidoPotencia(tpPOTENCIA_ATIVA, vet_TC);
   if(sentido < 0) return(false);
   sucesso = CalculaTap(sentido, vet_TC);
   }
else if(IsENERQCT())
   {
   sucesso = AjustaTapENERQCT(vet_TC);
   }
else if(IsPADRAO())
   {
   // PADRAO: impõe sentido 1 -> 2
   sucesso = CalculaTap(12, vet_TC);
   }
if(! sucesso) return(false);

// Retorna
if(TapDiferente(tap, tap_prev)) return(true);   // Houve alteração
else                            return(false);  // Não houve alteração
}

//---------------------------------------------------------------------------
bool __fastcall TReg2::AjustaTapENERQCT(complex<double> *vet_TC)
{
// Variáveis locais
int    linha, passo_int_aux;
double tap_pu, var_unit, vf_pu;

// Inicialização
var_unit = var_tensao_pu / double(num_passo);

// Obtém tensão atual na barra controlada
linha = bar_ref_12->linhaY;
if(linha < 0) return(false);  // Barra desconexa
vf_pu = Abs(vet_TC[linha]);

// Cria objeto TCurvaENERQCT, se ele não foi criado ainda
if(curva_tap == NULL) curva_tap = new TCurvaENERQCT;

// Ajusta tap de acordo com o modelo
tap_pu = curva_tap->AjustaTap(modelo, vf_pu, var_unit, bar_ref_12, bar2,
                              &passo_int_aux);

// Salva valores de passo e de tap
passo_int = passo_int_aux;
tap       = tap_pu;

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TReg2::AlterouTap(void)
{
// Abandona a análise se o tap estiver bloqueado
if(tap_bloqueado) return(false);

if(TapDiferente(tap, tap_prev)) return(true);   // Houve alteração de tap
else                            return(false);  // Não houve alteração de tap
}

//---------------------------------------------------------------------------
void __fastcall TReg2::AtualizaTapNR1(double delta_tap)
{
// Variáveis locais
double passo_double;

// Salva tap anterior e atualiza tap
tap_prev  = tap;
tap      += delta_tap;

   // Calcula passo (apenas para retorno ao Sinap)
passo_double = (tap - 1.) * double(num_passo) / var_tensao_pu;
passo_int    = int(passo_double + 0.0001);
}

//---------------------------------------------------------------------------
void __fastcall TReg2::CalculaDeltaY(double          Sbase,
                                     complex<double> *DYii,
                                     complex<double> *DYij,
                                     complex<double> *DYji)
{
// Calcula variação da matriz Ynodal causada pela variação
// do tap do regulador

// Variáveis locais
int             linY1, linY2;
complex<double> admit, fat_ij;
complex<double> cum(1., 0.), czero(0., 0.), mais120(-0.5, (0.5*sqrt(3.)));

// Inicialização
*DYii = czero;
*DYij = czero;
*DYji = czero;
if(tap_prev == tap) return;            // Tap não foi alterado na última iteração
linY1 = bar1->linhaY;
linY2 = bar2->linhaY;
if((linY1 < 0) || (linY2 < 0)) return; // Descarta barras(s) desconexa(s)
admit = snom_mva / (z1_pu * Sbase);

// Modelo B
// Calcula variação dos elementos de Ynodal quando o tap passa de
// 'tap_prev' para 'tap'. NB: DYjj = 0.
// IMPORTANTE: tratamento diferente para T2_AUTOTRAFODF e para T2_AUTOTRAFOY
switch(tipo)
   {
   case T2_AUTOTRAFO_DF:
      fat_ij = (mais120 - cum) * (tap - tap_prev);
      *DYii  = ((3. * (tap*tap - tap_prev*tap_prev))
             - (3. * (tap - tap_prev))) * admit;
      *DYij  = fat_ij * admit;
      *DYji  = conj(fat_ij) * admit;
      break;

   case T2_AUTOTRAFO_Y:
      *DYii = (tap*tap - tap_prev*tap_prev) * admit;
      *DYij = (tap_prev - tap) * admit;
      *DYji = *DYij;
      break;

   default:  // Nada a fazer
      break;
   }

// Atualiza Ynodal local
ynodalD_pu[0] += (*DYii);
ynodalD_pu[1] += (*DYij);
ynodalD_pu[2] += (*DYji);
}

//---------------------------------------------------------------------------
bool __fastcall TReg2::CalculaTap(int             sentido,
                                  complex<double> *vet_TC)
{
// Variáveis locais
int    delta_int, linha;
double banda_pu, delta, insensibilidade_pu, var_unit, vf_pu, vref, vref2_pu;

// Verifica se o sentido é reverso (21) e se o sub-modelo é 'regAUTO_UNI'
// (Caso 4 da Copel): regulador deve ser bypassado (retorna tap 1)
if((sentido == 21) && (sub_modelo == regAUTO_UNI))
   {
   passo_int = 0;
   tap       = 1.;
   return(true);
   }

// Inicialização
var_unit = var_tensao_pu / double(num_passo);

// Determina nó controlado, sua tensão de referência, sua tensão atual e
// largura da banda morta
if(sentido == 12)  // Sentido 1 -> 2
   {
   linha    = bar_ref_12->linhaY;
   banda_pu = banda_morta_pu;
   vref     = vref_12;
   }
else  // Sentido 2 -> 1
   {
   linha    = bar_ref_21->linhaY;
   banda_pu = banda_morta_inv_pu;
   vref     = vref_21;
   }
if(linha < 0) return(false);
vf_pu              = Abs(vet_TC[linha]);
insensibilidade_pu = 0.5 * banda_pu;

// Verifica se a tensão calculada na barra controlada está dentro da
// "dead band"; nesse caso, retorna sem alterar o tap ('delta_int' = 0)
if(fabs(vf_pu - vref) < insensibilidade_pu) return(true);

// Tensão atual fora da banda morta - calcula tap
if(vf_pu < vref) vref2_pu = vref - insensibilidade_pu; // Solicitação da Copel (HPS, 2018.08.28)
else             vref2_pu = vref + insensibilidade_pu;
// delta = (vref - vf_pu) / var_unit;  // Sentido 1 -> 2
delta = (vref2_pu - vf_pu) / var_unit;  // Sentido 1 -> 2
// Se o sentido é 21 [ tap (1/a):1 ], inverte a relação entre DeltaTap e DeltaV,
// desde que o sub-modelo NÃO SEJA 'regAUTO_FIXA' (Caso 3 da Copel). HPS, 2018.03.21.
if((sentido == 21) && (sub_modelo != regAUTO_FIXA)) delta = -delta;
delta_int  = DeltaPassoInteiro(delta);
passo_int += delta_int;
if     (passo_int >   num_passo)  passo_int =  num_passo;
else if(passo_int < (-num_passo)) passo_int = -num_passo;
tap = 1. + (double(passo_int) * var_unit);

return(true);
}

//---------------------------------------------------------------------------
int __fastcall TReg2::DeltaPassoInteiro(double delta)
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
bool __fastcall TReg2::IsDOIS_SENTIDOS(void)
{
if(modelo == regDOIS_SENTIDOS) return(true);
else                           return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TReg2::IsENERQCT(void)
{
if((modelo == regENERQCT1) ||
   (modelo == regENERQCT2) ||
   (modelo == regENERQCT3)) return(true);
else                        return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TReg2::IsPADRAO(void)
{
if(modelo == regPADRAO) return(true);
else                    return(false);
}

//---------------------------------------------------------------------------
int __fastcall TReg2::PM_GetDeltaPasso(void)
{
return(passo_int - passo_int_prev);
}

//---------------------------------------------------------------------------
bool __fastcall TReg2::PM_GetTapBloqueado(void)
{
return(tap_bloqueado);
}

//---------------------------------------------------------------------------
void __fastcall TReg2::PM_SetTapBloqueado(bool flag_bloqueio)
{
tap_bloqueado = flag_bloqueio;
}

//---------------------------------------------------------------------------
int __fastcall TReg2::SentidoPotencia(int             tipo_potencia,
                                      complex<double> *vet_TC)
{
// Variáveis locais
int    linha1, linha2;
double x12;

// Atualiza tensão nas barras terminais, para poder calcular correntes e
// potências no regulador
linha1 = bar1->linhaY;
linha2 = bar2->linhaY;
if((linha1 < 0) || (linha2 < 0)) return(-1);  // Erro
bar1->tensao_pu = vet_TC[linha1];
bar2->tensao_pu = vet_TC[linha2];

// Calcula regulador (correntes e potências)
Calcula(-1, NULL);  // Cálculo sem acumular perdas

// Obtém potência
if     (tipo_potencia == tpPOTENCIA_ATIVA)   x12 = vet_s_kva[0].real();
else if(tipo_potencia == tpPOTENCIA_REATIVA) x12 = vet_s_kva[0].imag();
else                                         x12 = 0.;  // Erro

// Proteção para evitar que valores muito pequenos fiquem trocando de
// sinal entre iterações consecutivas (regulador da Copel em vazio).
// HPS, 2018.09.05.
if(fabs(x12) < 0.1) x12 = 0.;  // Tolerância de 100 W / 100 VAr

// Retorna sentido de fluxo
if(x12 >= 0.) return(12);  // Sentido 1 -> 2
else          return(21);  // Sentido 2 -> 1
}

//---------------------------------------------------------------------------
bool __fastcall TReg2::TapDiferente(double tap,
                                    double tap_prev)
{
if(fabs(tap - tap_prev) > toler_tap_pu) return(true);  // Houve alteração de tap
else                                    return(false); // Não houve alteração de tap
}

//---------------------------------------------------------------------------
bool __fastcall TReg2::TipoImplementadoNR1(void)
{
switch(tipo)
   {
   case T2_AUTOTRAFO_DF:
   case T2_AUTOTRAFO_Y:
   case T2_DD:
   case T2_DY:
   case T2_YD:
   case T2_YY:
      return(true);

   default:
      return(false);
   }
}

//---------------------------------------------------------------------------


