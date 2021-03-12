//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TCapSerie.h"
#include ".\TBarra.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\SisLin\VTSisLin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TCapSerie::TCapSerie(TBarra *bar1,
                                TBarra *bar2,
                                double Sbase_mva)
{
this->bar1      = bar1;
this->bar2      = bar2;
this->Sbase_mva = Sbase_mva;
id_bar1_orig    = bar1->id;  // Para atribuir corretamente sentido do fluxo
pai_radial      = NULL;
ZeraPerdaEnergia();
}

//---------------------------------------------------------------------------
__fastcall TCapSerie::~TCapSerie(void)
{
}

//---------------------------------------------------------------------------
int __fastcall TCapSerie::Barras(TBarra **bar1,
                                 TBarra **bar2,
                                 TBarra **bar3)
{
*bar1 = this->bar1;
*bar2 = this->bar2;
*bar3 = NULL;

return(NumBarras());  // Número de barras da ligação
}

//---------------------------------------------------------------------------
void __fastcall TCapSerie::Calcula(int      np,
                                   VTGeralC *geralC)
{
// Variáveis locais
double          ibase, zbase;
double          raiz3 = sqrt(3.);
complex<double> admitancia, corrente1, corrente2, perda1, perda2, v1, v2;
complex<double> czero(0., 0.);

// Inicialização
vet_corr_a[0] = czero;
vet_corr_a[1] = czero;
vet_s_kva[0]  = czero;
vet_s_kva[1]  = czero;
perda         = czero;

// Descarta capacitor com barra(s) desconexa(s)
if((! bar1->flag_conexa) || (! bar2->flag_conexa)) return;

// Calcula correnets, potências e perda
zbase         = bar1->vnom_kv * bar1->vnom_kv / geralC->Sbase;
admitancia    = zbase / complex<double>(r_ohm, x_ohm);
v1            = bar1->tensao_pu;
v2            = bar2->tensao_pu;
corrente1     = admitancia * (v1 - v2);
corrente2     = -corrente1;
ibase         = 1000. * geralC->Sbase / (raiz3 * bar1->vnom_kv); // A
vet_corr_a[0] = corrente1 * ibase;
vet_corr_a[1] = corrente2 * ibase;
perda1        = 1000. * v1 * conj(corrente1) * geralC->Sbase; // kVA
perda2        = 1000. * v2 * conj(corrente2) * geralC->Sbase; // kVA
vet_s_kva[0]  = perda1;                                       // Pot. que entra na barra inicial
vet_s_kva[1]  = perda2;                                       // Pot. que entra na barra final
perda         = perda1 + perda2;                              // Perda no cap. série (kVA)

// Acumula perdas
geralC->perda_total_2    += perda;
perda_energia_diaria_kwh += perda.real() * geralC->DuracaoPatamar(np);
}

//---------------------------------------------------------------------------
int __fastcall TCapSerie::IndiceGlobal(int indice_local)
{
return(-1);
}

//---------------------------------------------------------------------------
bool __fastcall TCapSerie::InsereYnodal(VTGeralC *geralC,
                                        VTSisLin *Ynod)
{
// Variáveis locais
int             colY, linY;
double          zbase;
complex<double> y1, z1;

// Insere capacitor série
linY = bar1->linhaY;
colY = bar2->linhaY;
if((linY < 0) || (colY < 0)) return(true);  // Descarta barras(s) desconexa(s)
z1 = complex<double>(r_ohm, x_ohm);
if(Abs(z1) > 0.)
   {
   zbase = bar1->vnom_kv * bar1->vnom_kv / geralC->Sbase;
   y1    = zbase / z1;
   Ynod->InsereQuatro(linY, colY, y1);  // +ii, -ij, -ji, +jj
   }

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
int __fastcall TCapSerie::NumBarras(void)
{
return(2);
}

//---------------------------------------------------------------------------
int __fastcall TCapSerie::TipoLigacao(void)
{
return(CapSerie1);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TCapSerie::Y1par_pu(VTGeralC *geralC,
                                               TBarra   *bar1)
   {
   return(complex<double>(0., 0.));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TCapSerie::Z1ser2_pu(void)
   {
   double          ybase;
   complex<double> z1_pu;

   ybase = Sbase_mva / (bar1->vnom_kv * bar1->vnom_kv);
   z1_pu = complex<double>((r_ohm * ybase), (x_ohm * ybase));

   return(z1_pu);
   }

//---------------------------------------------------------------------------
void __fastcall TCapSerie::ZeraPerdaEnergia(void)
   {
   perda_energia_diaria_kwh = 0.;
   }

//---------------------------------------------------------------------------


