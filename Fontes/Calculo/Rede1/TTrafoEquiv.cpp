//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TTrafoEquiv.h"
#include ".\TBarra.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Eqv\VTEqv.h"
#include "..\..\SisLin\VTSisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTrafoEquiv::TTrafoEquiv(TBarra *bar1,
                                    TBarra *bar2)
   {
   complex<double> czero(0., 0.);

   // Salva e inicializa variáveis
   this->bar1    = bar1;
   this->bar2    = bar2;
   pai_radial    = NULL;
   id_bar1_orig  = bar1->id;  // Para atribuir corretamente sentido do fluxo
   Ynodal0_ij_pu = czero;
   Ynodal0_ji_pu = czero;
   Ynodal1_ij_pu = czero;
   Ynodal1_ji_pu = czero;
   modelo_eqv    = modeloEQV_INDEF;
   ZeraPerdaEnergia();

   // Calcula defasagem do trafo
   defasagem = bar2->ang_ref_rad - bar1->ang_ref_rad;

   // Zera matrizes de admitâncias nodais
   for(int n=0; n < 4; n++)
      {
      vet_ynodal0_pu[n] = czero;
      vet_ynodal1_pu[n] = czero;
      }
   }

//---------------------------------------------------------------------------
__fastcall TTrafoEquiv::~TTrafoEquiv(void)
   {
   }

//---------------------------------------------------------------------------
int __fastcall TTrafoEquiv::Barras(TBarra **bar1,
                                   TBarra **bar2,
                                   TBarra **bar3)
{
*bar1 = this->bar1;
*bar2 = this->bar2;
*bar3 = NULL;

return(NumBarras());  // Número de barras da ligação
}

//---------------------------------------------------------------------------
void __fastcall TTrafoEquiv::Calcula(int      np,
                                     VTGeralC *geralC)
{
double          ibase1, ibase2, ibase_const;
double          raiz3_inverso = 1. / sqrt(3.);
complex<double> corr1, corr2, perda1, perda2, v1, v2;
complex<double> czero(0., 0.);

// Inicialização
vet_corr_a[0] = czero;
vet_corr_a[1] = czero;
vet_s_kva[0]  = czero;
vet_s_kva[1]  = czero;
perda_kva     = czero;

// Descarta trafo com barra(s) desconexa(s)
if((! bar1->flag_conexa) || (! bar2->flag_conexa)) return;

// Obtém tensão nas barras terminais
v1          = bar1->tensao_pu;
v2          = bar2->tensao_pu;
ibase_const = 1000. * geralC->Sbase * raiz3_inverso;

// Calcula corrente e potência injetada em cada nó e perda total
corr1         = (vet_ynodal1_pu[0] * v1) + (vet_ynodal1_pu[1] * v2);
corr2         = (vet_ynodal1_pu[2] * v1) + (vet_ynodal1_pu[3] * v2);
ibase1        = ibase_const / bar1->vnom_kv;              // A
ibase2        = ibase_const / bar2->vnom_kv;              // A
vet_corr_a[0] = corr1 * ibase1;
vet_corr_a[1] = corr2 * ibase2;
perda1        = 1000. * v1 * conj(corr1) * geralC->Sbase; // kVA
perda2        = 1000. * v2 * conj(corr2) * geralC->Sbase; // kVA
vet_s_kva[0]  = perda1;                                   // Pot. que entra na barra inicial
vet_s_kva[1]  = perda2;                                   // Pot. que entra na barra final
perda_kva     = perda1 + perda2;                          // Perda no trafo (kVA)

// Acumula perdas
geralC->perda_total_2    += perda_kva;
perda_energia_diaria_kwh += perda_kva.real() * geralC->DuracaoPatamar(np);
}

//---------------------------------------------------------------------------
int __fastcall TTrafoEquiv::IndiceGlobal(int indice_local)
{
return(-1);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafoEquiv::InsereYnodal(int      seq,
                                          VTSisLin *Ynod)
{
if(seq == 0) return(InsereYnodal0(Ynod));
else         return(InsereYnodal1(Ynod));
}

//---------------------------------------------------------------------------
bool __fastcall TTrafoEquiv::InsereYnodal0(VTSisLin *Ynod)
{
// Local variables
int             linha_p, linha_s;
complex<double> y11, y12, y21, y22;

// Define linhas/colunas da matriz Ynodal
linha_p = bar1->linhaY;
linha_s = bar2->linhaY;
if((linha_p < 0) || (linha_s < 0)) return(true); // Descarta barras(s) desconexa(s)

// Obtém admitâncias nodais do trafo equivalente.
// IMPORTANTE: as definições de 'y11' e 'y22' têm que ser coerentes com os
//             descontos introduzidos na matriz de admitâncias nodais
//             das barras terminais, em 'TEqv1::MontaListaLigacoes()'.
y11 = - Ynodal0_ij_pu;
y12 =   Ynodal0_ij_pu;
y21 =   Ynodal0_ji_pu;
y22 = - Ynodal0_ji_pu;

// Salva admitâncias nodais na matriz local e as insere na matriz global
vet_ynodal0_pu[0] = y11;
vet_ynodal0_pu[1] = y12;
vet_ynodal0_pu[2] = y21;
vet_ynodal0_pu[3] = y22;

Ynod->InsereDiagonal(linha_p, y11);
Ynod->Insere(linha_p, linha_s, y12);
Ynod->Insere(linha_s, linha_p, y21);
Ynod->InsereDiagonal(linha_s, y22);

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrafoEquiv::InsereYnodal1(VTSisLin *Ynod)
{
// Local variables
int             linha_p, linha_s;
complex<double> cfator, y11, y12, y21, y22;

// Define linhas/colunas da matriz Ynodal
linha_p = bar1->linhaY;
linha_s = bar2->linhaY;
if((linha_p < 0) || (linha_s < 0)) return(true); // Descarta barras(s) desconexa(s)

// Obtém admitâncias nodais do trafo equivalente
// IMPORTANTE: as definições de 'y11' e 'y22' têm que ser coerentes com os
//             descontos introduzidos na matriz de admitâncias nodais
//             das barras terminais, em 'TEqv1::MontaListaLigacoes()'
y11 = - Ynodal1_ij_pu;
y12 =   Ynodal1_ij_pu;
y21 =   Ynodal1_ji_pu;
y22 = - Ynodal1_ji_pu;

// Verifica necessidade de corrigir valores para trafo equivalente com
// defasagem não nula gerado por Eqv3
if(modelo_eqv == modeloEQV3)
   {
   if(fabs(defasagem) > 0.001)  // Trafo com defasagem não nula
      {
      cfator  = polar(1., defasagem);
      y12    *= cfator;
      y21    *= conj(cfator);
      }
   }

// Salva admitâncias nodais na matriz local e as insere na matriz global
vet_ynodal1_pu[0] = y11;
vet_ynodal1_pu[1] = y12;
vet_ynodal1_pu[2] = y21;
vet_ynodal1_pu[3] = y22;

Ynod->InsereDiagonal(linha_p, y11);
Ynod->Insere(linha_p, linha_s, y12);
Ynod->Insere(linha_s, linha_p, y21);
Ynod->InsereDiagonal(linha_s, y22);

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
int __fastcall TTrafoEquiv::NumBarras(void)
{
return(2);
}

//---------------------------------------------------------------------------
int __fastcall TTrafoEquiv::TipoLigacao(void)
   {
   return(TrafoEquiv1);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TTrafoEquiv::Y1par_pu(VTGeralC *geralC,
                                                 TBarra   *bar1)
   {
   return(complex<double>(0., 0.));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TTrafoEquiv::Z1ser2_pu(void)
   {
   return(complex<double>(1., 0.) / Ynodal1_ij_pu);
   }

//---------------------------------------------------------------------------
void __fastcall TTrafoEquiv::ZeraPerdaEnergia(void)
   {
   perda_energia_diaria_kwh = 0.;
   }

//---------------------------------------------------------------------------

