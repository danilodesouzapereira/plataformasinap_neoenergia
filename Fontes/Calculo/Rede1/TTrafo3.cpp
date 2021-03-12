//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TTrafo3.h"
#include ".\TBarra.h"
#include "..\Interface.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\SisLin\VTSisLin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTrafo3::TTrafo3(TBarra *bar1,
                            TBarra *bar2,
                            TBarra *bar3)
   {
   // Salva dados
   this->bar1 = bar1;
   this->bar2 = bar2;
   this->bar3 = bar3;

   // Inicializa constantes
   raiz3_inverso = 1. / sqrt(3.);
   czero         = complex<double>(0., 0.);
   czero_mais    = complex<double>(0., ZEROV);
   cum           = complex<double>(1., 0.);
   cdois         = complex<double>(2., 0.);

   // Inicialização
   pai_radial = NULL;
   ptr_radial = NULL;
   tipo_ater1 = atIsolado;
   tipo_ater2 = atIsolado;
   tipo_ater3 = atIsolado;
   zater1_pu  = czero;
   zater2_pu  = czero;
   zater3_pu  = czero;
   ZeraPerdaEnergia();
   }

//---------------------------------------------------------------------------
__fastcall TTrafo3::~TTrafo3(void)
   {
   if(ptr_radial != NULL)
      {
      TObject *pobj = (TObject *) ptr_radial;
      delete pobj;
      }
   }

//---------------------------------------------------------------------------
int __fastcall TTrafo3::Barras(TBarra **bar1,
                               TBarra **bar2,
                               TBarra **bar3)
{
*bar1 = this->bar1;
*bar2 = this->bar2;
*bar3 = this->bar3;

return(NumBarras());  // Número de barras da ligação
}

//---------------------------------------------------------------------------
void __fastcall TTrafo3::Calcula(int      np,
                                 VTGeralC *geralC)
{
// Variáveis locais
double          duracao_h, fmult, ibase1, ibase2, ibase3, ibase_const,
                pfe_kw;
double          raiz3_inverso = 1. / sqrt(3.);
complex<double> corr1, corr2, corr3, perda1, perda2, perda3, v1, v2, v3;

// Inicialização
vet_corr_a[0] = czero;
vet_corr_a[1] = czero;
vet_corr_a[2] = czero;
vet_s_kva[0]  = czero;
vet_s_kva[1]  = czero;
vet_s_kva[2]  = czero;
perda_kva     = czero;

// Descarta trafo com barra(s) desconexa(s)
if((! bar1->flag_conexa) || (! bar2->flag_conexa) ||
   (! bar3->flag_conexa)) return;

// Obtém tensão nas barras terminais
v1          = bar1->tensao_pu;
v2          = bar2->tensao_pu;
v3          = bar3->tensao_pu;
ibase_const = 1000. * geralC->Sbase * raiz3_inverso;

// Calcula corrente e potência injetada em cada nó e perda total
corr1         = (ynodalD_pu[0] * v1) + (ynodalD_pu[1] * v2) + (ynodalD_pu[2] * v3);
corr2         = (ynodalD_pu[1] * v1) + (ynodalD_pu[3] * v2) + (ynodalD_pu[4] * v3);
corr3         = (ynodalD_pu[2] * v1) + (ynodalD_pu[4] * v2) + (ynodalD_pu[5] * v3);
ibase1        = ibase_const / bar1->vnom_kv;  // A
ibase2        = ibase_const / bar2->vnom_kv;  // A
ibase3        = ibase_const / bar3->vnom_kv;  // A
vet_corr_a[0] = corr1 * ibase1;
vet_corr_a[1] = corr2 * ibase2;
vet_corr_a[2] = corr3 * ibase3;
perda1        = 1000. * v1 * conj(corr1) * geralC->Sbase; // kVA
perda2        = 1000. * v2 * conj(corr2) * geralC->Sbase; // kVA
perda3        = 1000. * v3 * conj(corr3) * geralC->Sbase; // kVA
vet_s_kva[0]  = perda1;                                   // Pot. que entra no primário
vet_s_kva[1]  = perda2;                                   // Pot. que entra no secundário
vet_s_kva[2]  = perda3;                                   // Pot. que entra no terciário
perda_kva     = perda1 + perda2 + perda3;                 // Perda no trafo (kVA)
pfe_kw        = 1000. * ypar_pu.real() * geralC->Sbase;   // Nominal (kW)
if(geralC->flag_ypar_trafos)
   {
   if     (enrol_Ypar == 0) fmult = Abs(v1) * (vnom1 / vtap1); // Coerente com a montagem da [Y]
   else if(enrol_Ypar == 1) fmult = Abs(v2) * (vnom2 / vtap2); // Coerente com a montagem da [Y]
   else                     fmult = Abs(v3) * (vnom3 / vtap3); // Coerente com a montagem da [Y]
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
void __fastcall TTrafo3::DefineAterramento(double     Sbase,
                                           TBarra     *bar1,
                                           TBarra     *bar2,
                                           TBarra     *bar3,
                                           smcTRAFO3E *trafo)
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

if(trafo->ter.ligacao == ligacaoESTRELA_ATERRADO)
   {
   if(Abs(trafo->ter.zater_ohm) > 0.)
      {
      tipo_ater3 = atZ;
      Ybase      = Sbase / (bar3->vnom_kv * bar3->vnom_kv);
      zater3_pu  = trafo->ter.zater_ohm * Ybase;
      }
   else
      {
      tipo_ater3 = atSolido;
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TTrafo3::DeterminaEnrolamentosRef(VTGeralC *geralC)
{
// Determina o enrolamento de referência:
//    - Para potência de base ('enrol_Sref'): enrolamento com maior Snom
//    - Para conexão do ramo paralelo do trafo (Pfe, 'enrol_Ypar'):
//      enrolamento ligado em Y com maior Snom - ANTIGO
//    - Para conexão do ramo paralelo do trafo (Pfe, 'enrol_Ypar'):
//      primeiro enrolamento ligado em Y, mantendo coerência com
//      modelo trifásico (TTrafo33). HPS, 2019.07.24.

// Determina 'enrol_Sref'
enrol_Sref = 0;           // Inicializa pelo primário
Sref_mva   = snom1_mva;
if(snom2_mva > Sref_mva)  // Verifica secundário
   {
   enrol_Sref = 1;
   Sref_mva   = snom2_mva;
   }
if(snom3_mva > Sref_mva)  // Verifica terciário
   {
   enrol_Sref = 2;
   Sref_mva   = snom3_mva;
   }

// Determina 'enrol_Ypar'
if     (IsYYY()) enrol_Ypar = 0;  // Primário
else if(IsDYY()) enrol_Ypar = 1;  // Secundário
else             enrol_Ypar = 2;  // DDY - Terciário
}

//---------------------------------------------------------------------------
int __fastcall TTrafo3::IndiceGlobal(int indice_local)
{
return(-1);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo3::InsereYnodal(int      seq,
                                      VTGeralC *geralC,
                                      VTSisLin *Ynod,
                                      bool     flag_zpequena)
{
// Local variables
int             linha_p, linha_t, linha_s;
complex<double> *vet_ynodal;

// Define linhas/colunas na matriz global
linha_p = bar1->linhaY;
linha_s = bar2->linhaY;
linha_t = bar3->linhaY;
if((linha_p < 0) || (linha_s < 0) || (linha_t < 0)) return(true); // Descarta barras(s) desconexa(s)

// Monta Ynodal do trafo
if(seq == 0)
   {
   vet_ynodal = ynodalZ_pu;
   if(! MontaYnodalZ(flag_zpequena, geralC)) return(false);
   }
else  // Sequência direta
   {
   vet_ynodal = ynodalD_pu;
   if(! MontaYnodalD(flag_zpequena, geralC)) return(false);
   }

// Transfere Ynodal local para matriz global
Ynod->InsereDiagonal(linha_p, vet_ynodal[0]);            // Ypp
Ynod->InsereSimetrico(linha_p, linha_s, vet_ynodal[1]);  // Yps e Ysp
Ynod->InsereSimetrico(linha_p, linha_t, vet_ynodal[2]);  // Ypt e Ytp

Ynod->InsereDiagonal(linha_s, vet_ynodal[3]);            // Yss
Ynod->InsereSimetrico(linha_s, linha_t, vet_ynodal[4]);  // Yst e Yts

Ynod->InsereDiagonal(linha_t, vet_ynodal[5]);            // Ytt

// Verifica condição de erro
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo3::IsDDY(void)
{
if((tipo == TRAFO_DDY1) || (tipo == TRAFO_DDY11)) return(true);
else                                              return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo3::IsDYY(void)
{
if((tipo == TRAFO_DY1Y1)  || (tipo == TRAFO_DY1Y11) ||
   (tipo == TRAFO_DY11Y1) || (tipo == TRAFO_DY11Y11)) return(true);
else                                                  return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo3::IsYYY(void)
{
if(tipo == TRAFO_YYY) return(true);
else                  return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo3::MontaYnodalD(bool     flag_zpequena,
                                      VTGeralC *geralC)
{
// Variáveis locais
double          alfa, beta, gama;
complex<double> const1, const2, ydiag, yf1, yf2;

// Calcula taps do modelo
alfa = vnom1 / vtap1;
beta = vnom2 / vtap2;
gama = vnom3 / vtap3;

// Compatibilidade: na sequência zero, os modelos de trafo de 3 enrolamentos
//                  NÃO possuem representação de tap. Por esta razão, no
//                  cálculo de curto-circuito CS força-se os 3 taps iguais
//                  a 1 na sequência direta.
// ATENÇÃO: comandos abaixo desabilitados em 2014.12.03. Estudo de curto-
//          circuito com o CurtoCS: numa rede da Eletropaulo na qual a chave de
//          socorro entre 2 circuitos primários de SEs distintas estava fechada,
//          ocorreu corrente de circulação mesmo com a rede em vazio (sem
//          pré-falta). Isto porque uma das SEs possuia somente trafos de 3
//          enrolamentos com taps diferentes de 1 mas forçados iguais a 1 pelos
//          comandos abaixo, e a outra SE possuia somente trafos de 2
//          enrolamentos com taps iguais aos taps dos trafos de 3 enrolamentos
//          (diferentes de 1), que foram respeitados pelo CurtoCS. Esta corrente
//          de circulação em vazio provocou I1 != I2 na rede para defeito
//          fase-terra, o que causou Ib != Ic em todos os trechos que fazem
//          parte do caminho elétrico entre as duas SEs. Demorei bastante para
//          identificar esta questão, e acabei decidindo fazer com que a
//          corrente de circulação em vazio seja zero, às custas de uma pequena
//          incompatibilidade entre os modelos de sequência direta e sequência
//          zero dos trafos de 3 enrolamentos (o modelo de sequência zero
//          continua não tendo a representação de taps). HPS, 2014.12.03.
/*
if(geralC->Programa1 == progCURTO_CS)
   {
   alfa = beta = gama = 1.;
   }
*/

// Calcula impedâncias do modelo estrela (as impedâncias já
// estão na base Sbase)
if(flag_zpequena)
   {
   zp1_pu = zs1_pu = zt1_pu = complex<double>(0., REATANCIA_PEQUENA);
   }
else
   {
   zp1_pu = 0.5 * (zps1_pu + zpt1_pu - zst1_pu);
   zs1_pu = 0.5 * (zps1_pu + zst1_pu - zpt1_pu);
   zt1_pu = 0.5 * (zst1_pu + zpt1_pu - zps1_pu);
   }

// Calcula 1a. linha da matriz (primário)
const1        = 1./zs1_pu + 1./zt1_pu;
const2        = 1. + zp1_pu * const1;
ydiag         = alfa * alfa * const1 / const2;      // Ypp
yf1           = - alfa * beta / (zs1_pu * const2);  // Yps = Ysp
yf2           = - alfa * gama / (zt1_pu * const2);  // Ypt = Ytp
ynodalD_pu[0] = ydiag;
ynodalD_pu[1] = yf1;
ynodalD_pu[2] = yf2;

// Calcula 2a. linha da matriz
const1        = 1./zp1_pu + 1./zt1_pu;
const2        = 1. + zs1_pu * const1;
ydiag         = beta * beta * const1 / const2;      // Yss
yf1           = - beta * gama / (zt1_pu * const2);  // Yst = Yts
ynodalD_pu[3] = ydiag;
ynodalD_pu[4] = yf1;

// Calcula 3a. linha da matriz
const1        = 1./zp1_pu + 1./zs1_pu;
const2        = 1. + zt1_pu * const1;
ydiag         = gama * gama * const1 / const2;  // Ytt
ynodalD_pu[5] = ydiag;

// Inclui ramo em paralelo do trafo, se indicado pelo flag
if(geralC->flag_ypar_trafos && (! flag_zpequena))
   {
   if     (enrol_Ypar == 0) ynodalD_pu[0] += alfa * alfa * ypar_pu; // Ref. é o primário
   else if(enrol_Ypar == 1) ynodalD_pu[3] += beta * beta * ypar_pu; // Ref. é o secundário
   else                     ynodalD_pu[5] += gama * gama * ypar_pu; // Ref. é o terciário
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafo3::MontaYnodalZ(bool     flag_zpequena,
                                      VTGeralC *geralC)
{
// Variáveis locais
complex<double> csoma, y0, y1, y2, yp, ys, yt;

// Calcula admitâncias nodais em função da ligação dos enrolamentos
if(flag_zpequena) yp = ys = yt = complex<double>(0., (-1. / REATANCIA_PEQUENA));
switch(tipo)
   {
   case TRAFO_DDY1:
   case TRAFO_DDY11:
      if(! flag_zpequena)
         {
         yp = cdois / (zps0_pu + zpt0_pu - zst0_pu);
         ys = cdois / (zps0_pu + zst0_pu - zpt0_pu);
         yt = cdois / (zst0_pu + zpt0_pu - zps0_pu + (6. * zater3_pu));
         }
      csoma         = cum   / (yp + ys + yt);
      y0            = (yp*yt + ys*yt) * csoma;
      ynodalZ_pu[0] = czero_mais;  // Para não deixar o primário   desconexo
      ynodalZ_pu[1] = czero;
      ynodalZ_pu[2] = czero;
      ynodalZ_pu[3] = czero_mais;  // Para não deixar o secundário desconexo
      ynodalZ_pu[4] = czero;
      ynodalZ_pu[5] = y0;
      break;

   case TRAFO_DY1Y1:
   case TRAFO_DY1Y11:
   case TRAFO_DY11Y1:
   case TRAFO_DY11Y11:
      if(! flag_zpequena)
         {
         yp = cdois / (zps0_pu + zpt0_pu - zst0_pu);
         ys = cdois / (zps0_pu + zst0_pu - zpt0_pu + (6. * zater2_pu));
         yt = cdois / (zst0_pu + zpt0_pu - zps0_pu + (6. * zater3_pu));
         }
      csoma = cum / (yp + ys + yt);
      // p=ic s=kc t=jc   ;   0=st=kc-jc  1=ps=ic-kc  2=tp=jc-ic
      y0            =  ys * yt * csoma;
      y1            =  yp * ys * csoma;
      y2            =  yt * yp * csoma;
      ynodalZ_pu[0] =  czero_mais;  // Para não deixar o primário desconexo
      ynodalZ_pu[1] =  czero;
      ynodalZ_pu[2] =  czero;
      ynodalZ_pu[3] =  y0 + y1;
      ynodalZ_pu[4] = -y0;
      ynodalZ_pu[5] =  y0 + y2;
      break;

   case TRAFO_YYY:
      if(! flag_zpequena)
         {
         yp = cdois / (zps0_pu + zpt0_pu - zst0_pu + (6. * zater1_pu));
         ys = cdois / (zps0_pu + zst0_pu - zpt0_pu + (6. * zater2_pu));
         yt = cdois / (zst0_pu + zpt0_pu - zps0_pu + (6. * zater3_pu));
         }
      csoma = cum / (yp + ys + yt);
      // p=ic s=kc t=jc   ;   0=st=kc-jc  1=ps=ic-kc  2=tp=jc-ic
      y0            =  ys * yt * csoma;
      y1            =  yp * ys * csoma;
      y2            =  yt * yp * csoma;
      ynodalZ_pu[0] =  y1 + y2;
      ynodalZ_pu[1] = -y1;
      ynodalZ_pu[2] = -y2;
      ynodalZ_pu[3] =  y0 + y1;
      ynodalZ_pu[4] = -y0;
      ynodalZ_pu[5] =  y0 + y2;
      break;

   default:  // Erro
      return(false);
   }

// Inclui ramo em paralelo do trafo, se indicado pelo flag
if(geralC->flag_ypar_trafos && (! flag_zpequena))
   {
   if     (enrol_Ypar == 0) ynodalZ_pu[0] += ypar_pu;
   else if(enrol_Ypar == 1) ynodalZ_pu[3] += ypar_pu;
   else                     ynodalZ_pu[5] += ypar_pu;
   }

return(true);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo3::NumBarras(void)
{
return(3);
}

//---------------------------------------------------------------------------
int __fastcall TTrafo3::TipoLigacao(void)
   {
   return(Trafo3);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TTrafo3::Y1par_pu(VTGeralC *geralC,
                                             TBarra   *bar1)
   {
   return(complex<double>(0., 0.)); // Apenas para compatibilidade com VTLigacaoG1
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TTrafo3::Z1ser2_pu(void)
   {
   return(complex<double>(0., 0.)); // Apenas para compatibilidade com VTLigacaoG1
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo3::ZeraPerdaEnergia(void)
   {
   perda_energia_diaria_kwh = 0.;
   perda_ferro_diaria_kwh   = 0.;
   }

//---------------------------------------------------------------------------


