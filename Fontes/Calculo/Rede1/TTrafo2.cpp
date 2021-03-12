//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TTrafo2.h"
#include ".\TBarra.h"
#include "..\Interface.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\SisLin\VTSisLin.h"
#include "..\..\..\DLL_Inc\SisLin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTrafo2::TTrafo2(TBarra *bar1,
                            TBarra *bar2,
                            double Sbase_mva)
   {
   // Salva dados
   this->bar1      = bar1;
   this->bar2      = bar2;
   this->Sbase_mva = Sbase_mva;

   // Inicialização
   flag_regulador = false;
   pai_radial     = NULL;
   id_bar1_orig   = bar1->id;  // Para atribuir corretamente sentido do fluxo
   eqpto          = NULL;
   codigo         = "";
   tap            = 1.;
   tipo_ater1     = atIsolado;
   tipo_ater2     = atIsolado;
   y1par_pu       = complex<double>(0., 0.);
   zater1_pu      = complex<double>(0., 0.);
   zater2_pu      = complex<double>(0., 0.);
   ZeraPerdaEnergia();

   }

//---------------------------------------------------------------------------
__fastcall TTrafo2::~TTrafo2(void)
   {
   }

//---------------------------------------------------------------------------
int __fastcall TTrafo2::Barras(TBarra **bar1,
                               TBarra **bar2,
                               TBarra **bar3)
{
*bar1 = this->bar1;
*bar2 = this->bar2;
*bar3 = NULL;

return(NumBarras());  // Número de barras da ligação
}

//---------------------------------------------------------------------------
void __fastcall TTrafo2::Calcula(int      np,
                                 VTGeralC *geralC)
{
// Variáveis locais
double          duracao_h, fmult, ibase1, ibase2, ibase_const, pfe_kw, Sbase;
double          raiz3_inverso = 1. / sqrt(3.);
complex<double> corr1, corr2, perda1, perda2, v1, v2;
complex<double> czero(0., 0.);

// Inicialização
vet_corr_a[0] = czero;
vet_corr_a[1] = czero;
vet_s_kva[0]  = czero;
vet_s_kva[1]  = czero;
perda_kva     = czero;
if(geralC != NULL) Sbase = geralC->Sbase;
else               Sbase = 100.; // Cálculo só para descobrir o sentido da potência ativa
ibase_const = 1000. * Sbase * raiz3_inverso;

// Descarta trafo com barra(s) desconexa(s)
if((! bar1->flag_conexa) || (! bar2->flag_conexa)) return;

// Obtém tensão nas barras terminais
v1 = bar1->tensao_pu;
v2 = bar2->tensao_pu;

// Calcula corrente e potência injetada em cada nó e perda total
corr1         = ynodalD_pu[0] * v1 + ynodalD_pu[1] * v2;
corr2         = ynodalD_pu[2] * v1 + ynodalD_pu[3] * v2;
ibase1        = ibase_const / bar1->vnom_kv;         // A
ibase2        = ibase_const / bar2->vnom_kv;         // A
vet_corr_a[0] = corr1 * ibase1;
vet_corr_a[1] = corr2 * ibase2;
perda1        = 1000. * v1 * conj(corr1) * Sbase;    // kVA
perda2        = 1000. * v2 * conj(corr2) * Sbase;    // kVA
vet_s_kva[0]  = perda1;                              // Pot. que entra na barra inicial
vet_s_kva[1]  = perda2;                              // Pot. que entra na barra final
perda_kva     = perda1 + perda2;                     // Perda no trafo (kVA)
pfe_kw        = 1000. * y1par_pu.real() * snom_mva;  // Nominal (kW)

// Retorna se os argumentos forem inválidos (cálculo temporário)
if((np < 0) || (geralC == NULL)) return;

// Calcula perda no ferro
if(geralC->flag_ypar_trafos)
   {
   // fmult   = Abs(v2) / tap; // Coerente com a montagem da [Y]
   fmult   = Abs(v2); // Coerente com a montagem de [Y] e os modelos desequilibrados (TTrafo32)
   pfe_kw *= (fmult * fmult);
   }

// Acumula perdas
duracao_h                  = geralC->DuracaoPatamar(np);
perda_energia_diaria_kwh  += perda_kva.real() * duracao_h;
perda_ferro_diaria_kwh    += pfe_kw       * duracao_h;
geralC->perda_total_2     += perda_kva;
geralC->perda_ypar_trafos += complex<double>(pfe_kw, 0.);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrafo2::CalculaCorrenteInjetada(TBarra          *bar_ref,
                                                            complex<double> *vet_tensoes)
{
// Variáveis locais
int             colY, colY_global, ind_local, linY, linY_global;
complex<double> cor1, y11, y12;

ind_local = IndiceLocal(bar_ref);
if(ind_local == 0)  // bar1
   {
   linY = 0;
   colY = 1;
   y11  = ynodalD_pu[0];
   y12  = ynodalD_pu[1];
   }
else if(ind_local == 1)  // bar2
   {
   linY = 1;
   colY = 0;
   y11  = ynodalD_pu[3];
   y12  = ynodalD_pu[2];
   }
else
   {
   return(complex<double>(0., 0.));  // Erro
   }
linY_global = IndiceGlobal(linY);
colY_global = IndiceGlobal(colY);
cor1        = (y11 * vet_tensoes[linY_global]) + (y12 * vet_tensoes[colY_global]);

return(cor1);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrafo2::CalculaPotenciaInjetada(TBarra          *bar_ref,
                                                            complex<double> *vet_tensoes)
{
// Variáveis locais
int             linY, linY_global;
complex<double> cor1, s1;

// Calcula potência injetada
linY        = IndiceLocal(bar_ref);
linY_global = IndiceGlobal(linY);
cor1        = CalculaCorrenteInjetada(bar_ref, vet_tensoes);
s1          = vet_tensoes[linY_global] * conj(cor1);

return(s1);
}

//---------------------------------------------------------------------------
void __fastcall TTrafo2::DefineAterramento(double     Sbase,
                                           TBarra     *bar1,
                                           TBarra     *bar2,
                                           smcTRAFO2E *trafo)
{
// Variáveis locais
double Ybase;

if(trafo->pri.ligacao == ligacaoESTRELA_ATERRADO)
   {
   if(Abs(trafo->pri.zater_ohm) > 0.)
      {
      tipo_ater1 = atZ;
      Ybase      = Sbase / (bar1->vnom_kv * bar1->vnom_kv);
      zater1_pu  = trafo->pri.zater_ohm * Ybase;
      }
   else
      {
      tipo_ater1 = atSolido;
      }
   }

if(trafo->sec.ligacao == ligacaoESTRELA_ATERRADO)
   {
   if(Abs(trafo->sec.zater_ohm) > 0.)
      {
      tipo_ater2 = atZ;
      Ybase      = Sbase / (bar2->vnom_kv * bar2->vnom_kv);
      zater2_pu  = trafo->sec.zater_ohm * Ybase;
      }
   else
      {
      tipo_ater2 = atSolido;
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TTrafo2::ImprimeYnodal(VTGeralC   *geralC,
                                       int        seq,
                                       AnsiString nome_arq)
{
// Variáveis locais
complex<double> *vetor;
AnsiString      string1;
FILE            *arq;

// Imprime matriz
arq = fopen(nome_arq.c_str(), "w");
if(arq == NULL) return;
if(seq == 0)  // Sequência zero
   {
   string1 = "YnodalZ";
   vetor   = ynodalZ_pu;
   }
else  // Sequência direta
   {
   string1 = "YnodalD";
   vetor   = ynodalD_pu;
   }
fprintf(arq, "\n\n *** Matriz %s ***"
             "\n\n     Mat[0]: (%13.6e %13.6e)"
               "\n     Mat[1]: (%13.6e %13.6e)"
               "\n     Mat[2]: (%13.6e %13.6e)"
               "\n     Mat[3]: (%13.6e %13.6e)",
        string1.c_str(), vetor[0].real(), vetor[0].imag(),
        vetor[1].real(), vetor[1].imag(), vetor[2].real(), vetor[2].imag(),
        vetor[3].real(), vetor[3].imag());
fclose(arq);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo2::IndiceGlobal(int indice_local)
{
if     (indice_local == 0) return(bar1->linhaY);
else if(indice_local == 1) return(bar2->linhaY);
else                       return(-1);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo2::IndiceLocal(TBarra *bar_ref)
{
if     (bar_ref == bar1) return(0);
else if(bar_ref == bar2) return(1);
else                     return(-1);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo2::InsereYnodal(int      seq,
                                      VTGeralC *geralC,
                                      VTSisLin *Ynod,
                                      bool     flag_zpequena)
{
// Local variables
int             linha_p, linha_s;
complex<double> *ynodal_pu;

// Define linhas/colunas na matriz Ynodal global
linha_p = bar1->linhaY;
linha_s = bar2->linhaY;
if((linha_p < 0) || (linha_s < 0)) return(true); // Descarta barras(s) desconexa(s)

// Monta matriz Ynodal (armazenada localmente em 'ynodalD[]' ou 'ynodalZ[]')
if(seq == 0)
   {
   if(! MontaYnodalZ(flag_zpequena, geralC)) return(false);
   ynodal_pu = ynodalZ_pu;
   }
else
   {
   if(! MontaYnodalD(flag_zpequena, geralC)) return(false);
   ynodal_pu = ynodalD_pu;
   }

// Transfere Ynodal local para matriz global
Ynod->InsereDiagonal(linha_p, ynodal_pu[0]);
Ynod->Insere(linha_p, linha_s, ynodal_pu[1]);
Ynod->Insere(linha_s, linha_p, ynodal_pu[2]);
Ynod->InsereDiagonal(linha_s, ynodal_pu[3]);

// Verifica condição de erro
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo2::MontaYnodalD(bool     flag_zpequena,
                                      VTGeralC *geralC)
{

// NB: Implementação do Modelo B (HPS, 05.05.2008)

// Variáveis locais
double          alfa, k, km1, fconv, fmult;
complex<double> admit, aux, mais120, y11, y12, y21, y22;

// Inicialização
fconv = snom_mva / Sbase_mva;  // Mudança de base para Sbase
alfa  = tap;
if(flag_zpequena) admit = complex<double>(0., (-1. / REATANCIA_PEQUENA));
else              admit = fconv / z1_pu;

// IMPORTANTE: como o modelo monofásico equivalente ao modelo trifásico do
//             autotrafo em Delta Fechado usado em Gauss3 / NR3 possui
//             representações diferentes na sequência direta e na sequência
//             inversa, no caso de autotrafo em Delta Fechado no programa
//             CURTO_CS trata-se o autotrafo em Delta Fechado como se fosse
//             um trafo normal (senão, CURTO_CS deveria montar também Ynodal
//             de sequência inversa) (HPS, 2015.05.22)
if((geralC->Programa1 == progCURTO_CS) || (tipo != T2_AUTOTRAFO_DF)) // Caso geral
   {
   y11 = alfa * alfa * admit;
   y12 = -alfa * admit;
   y21 = y12;
   y22 = admit;
   }
else  // Programa DIFERENTE de CURTO_CS *E* T2_AUTOTRAFODF:
   {  // modelo equivalente ao modelo trifásico usado em Gauss3/NR3
   k       = alfa;
   km1     = alfa - 1.;
   mais120 = complex<double>(-0.5, (0.5 * sqrt(3.)));  // Operador 'alfa'
   aux     = (k * (mais120 - complex<double>(1., 0.))) - mais120;
   y11     = ((3. * k * km1) + 1.) * admit;
   y12     = aux * admit;
   y21     = conj(aux) * admit;
   y22     = admit;
   }

// Inclui ramo em paralelo do trafo
if(geralC->flag_ypar_trafos && (! flag_zpequena))
   {
   // fmult  = fconv / (tap * tap);
   fmult  = fconv;  // Coerente com os modelos desequilibrados (TTrafo32)
   y22   += (y1par_pu * fmult);
   }

// Salva valores na matriz local
ynodalD_pu[0] = y11;
ynodalD_pu[1] = y12;
ynodalD_pu[2] = y21;
ynodalD_pu[3] = y22;

// Imprime matriz [Ynodal]
// AnsiString str1 = geralC->DirTmp() + "\\YTrafo2.txt";
// ImprimeYnodal(geralC, 1, str1);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo2::MontaYnodalZ(bool     flag_zpequena,
                                      VTGeralC *geralC)
{

// NB: Implementação do Modelo B (HPS, 05.05.2008)

// Variáveis locais
double          alfa, alfaq, fconv;
complex<double> y11, y12, y21, y22, zt;
complex<double> cum, ctres, czero, czero_mais;

// Define valor de constantes
cum        = complex<double>(1., 0.);
ctres      = complex<double>(3., 0.);
czero      = complex<double>(0., 0.);
czero_mais = complex<double>(0., ZEROV);

// Monta matriz
alfa  = tap;
alfaq = alfa * alfa;
fconv = snom_mva / Sbase_mva;  // Mudança de base
if(flag_zpequena) zt = complex<double>(0., REATANCIA_PEQUENA);
else              zt = z0_pu / fconv;  // zt na base Sbase

// Inicialização para os seguintes casos:
//    - T2_DD
//    - T2_DY e T2_YD com o enrolamento em estrela isolada
//    - T2_YY com pelo menos um dos enrolamentos em estrela isolada
y11 = y22 = czero_mais;
y12 = y21 = czero;

// Analisa possibilidades de ligação dos 2 enrolamentos
switch(tipo)
   {
   case T2_AUTOTRAFO_DF:
      y11 = cum / zt;
      y12 = -y11;
      y21 = -y11;
      y22 =  y11;
      break;

   case T2_AUTOTRAFO_Y:
      // Este caso é tratado como se os aterramentos fossem ambos
      // do tipo atSolido. Logo, é idêntico ao caso T2_YY sem verificar
      // o tipo dos aterramentos e com zater1_pu = zater2_pu = 0.
      y22 =  cum / zt;
      y11 =  alfaq * y22;
      y12 = -alfa  * y22;
      y21 =  y12;
      break;

   case T2_DD:  // Nada a fazer (valores já inicializados)
      break;

   case T2_DY:
      if(tipo_ater2 != atIsolado)  // Delta/Yaterr
         {
         y22 = cum / ((ctres * zater2_pu) + zt);
         // Os demais valores são zero (já inicializados)
         }
      break;

   case T2_YD:
      if(tipo_ater1 != atIsolado)  // Yaterr/Delta
         {
         y11 = cum / ((ctres * zater1_pu) + (alfaq * zt));
         // Os demais valores são zero (já inicializados)
         }
      break;

   case T2_YY:
      if((tipo_ater1 != atIsolado) && (tipo_ater2 != atIsolado))
         {  // Yaterr/Yaterr
         y22 =  cum / ((ctres * alfaq * zater1_pu) +
                       (ctres * zater2_pu) + zt);
         y11 =  alfaq * y22;
         y12 = -alfa  * y22;
         y21 =  y12;
         }
      break;

   default:
      return(false);  // Erro
   }

if(geralC->flag_ypar_trafos && (! flag_zpequena)) // Inclui ramo em paralelo do trafo (enrolamento em Yaterr)
   {
   if((tipo == T2_AUTOTRAFO_Y) || (tipo == T2_DY) || (tipo == T2_YY))
      {
      if(tipo_ater2 != atIsolado) y22 += y1par_pu * fconv;
      }
   else if(tipo == T2_YD)
      {
      if(tipo_ater1 != atIsolado) y11 += y1par_pu * fconv;
      }
   }

// Salva valores na matriz local
ynodalZ_pu[0] = y11;
ynodalZ_pu[1] = y12;
ynodalZ_pu[2] = y21;
ynodalZ_pu[3] = y22;

return(true);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo2::NumBarras(void)
{
return(2);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo2::TipoLigacao(void)
   {
   return(Trafo2);
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo2::TrocaBarras(void)
{
int             int1;
double          double1;
complex<double> cvalor;
TBarra          *bar3;

bar3 = bar1;
bar1 = bar2;
bar2 = bar3;

if     (tipo == T2_YD) tipo = T2_DY;
else if(tipo == T2_DY) tipo = T2_YD;

int1       = tipo_ater1;
tipo_ater1 = tipo_ater2;
tipo_ater2 = int1;

double1    = vnom1;
vnom1      = vnom2;
vnom2      = double1;

cvalor     = zater1_pu;
zater1_pu  = zater2_pu;
zater2_pu  = cvalor;

tap        = 1. / tap;
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrafo2::Y1par_pu(VTGeralC *geralC,
                                             TBarra   *bar1)
   {
   complex<double> admit, y1cap_pu;

   admit = snom_mva / (z1_pu * Sbase_mva);
   if(bar1 == this->bar1)  // Barra inicial
      {
      y1cap_pu = tap * (tap - 1.) * admit;
      }
   else  // Barra final
      {
      y1cap_pu = (1. - tap) * admit;
      }

   return(y1cap_pu);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TTrafo2::Z1ser2_pu(void)
   {
   return(z1_pu * Sbase_mva / (snom_mva * tap));
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo2::ZeraPerdaEnergia(void)
   {
   perda_energia_diaria_kwh = 0.;
   perda_ferro_diaria_kwh   = 0.;
   }

//---------------------------------------------------------------------------

