//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TTrecho.h"
#include ".\TBarra.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\SisLin\VTSisLin.h"
#include "..\..\..\DLL_Inc\SisLin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTrecho::TTrecho(TBarra *bar1,
                            TBarra *bar2)
   {
   // Variáveis locais
   complex<double> czero(0., 0.);

   this->bar1         = bar1;
   this->bar2         = bar2;
   pai_radial         = NULL;
   id_bar1_orig       = bar1->id; // Para atribuir corretamente sentido do fluxo
   id                 = ID_INVALIDO;
   ynodalD_pu         = NULL;
   eqpto              = NULL;
   grupo_mutua        = -1;
   codigo             = "";
   flag_chave_fechada = true;     // Chave fechada (usado em Estabilidade e Estimador1)
   Z0ser_pu           = czero;
   Z1ser_pu           = czero;
   Y0ser_pu           = czero;
   Y1ser_pu           = czero;
   Y0cap_pu           = czero;
   Y1cap_pu           = czero;
   Ynodal0_ji_pu      = czero;
   Ynodal1_ji_pu      = czero;
   ZeraPerdaEnergia();
   }

//---------------------------------------------------------------------------
__fastcall TTrecho::~TTrecho(void)
   {
   delete ynodalD_pu;
   }

//---------------------------------------------------------------------------
int __fastcall TTrecho::Barras(TBarra **bar1,
                               TBarra **bar2,
                               TBarra **bar3)
{
*bar1 = this->bar1;
*bar2 = this->bar2;
*bar3 = NULL;

return(NumBarras());  // Número de barras da ligação
}

//---------------------------------------------------------------------------
void __fastcall TTrecho::Calcula(int      np,
                                 VTGeralC *geralC)
{
double          ibase;
double          raiz3 = sqrt(3.);
complex<double> corrente, corrente1, corrente2, perda1, perda2, v1, v2, ypar;
complex<double> cmeio(0.5, 0.), czero(0., 0.);

// Descarta trecho que participa em mútua [calculado em TGrupoMutua::Calcula()]
if(grupo_mutua >= 0) return;

// Inicialização
vet_corr_a[0] = czero;
vet_corr_a[1] = czero;
vet_s_kva[0]  = czero;
vet_s_kva[1]  = czero;
perda_kva     = czero;
if(! flag_chave_fechada) return;  // Descarta trecho com chave aberta

// Calcula correntes, potências e perda
v1        = bar1->tensao_pu;
v2        = bar2->tensao_pu;
corrente  = Y1ser_pu * (v1 - v2);
corrente1 =  corrente;
corrente2 = -corrente;
if(geralC->flag_capac_LT)  // Considera capacitância das linhas
   {
   ypar       = cmeio * Y1cap_pu;
   corrente1 += ypar * v1;
   corrente2 += ypar * v2;
   }
ibase         = 1000. * geralC->Sbase / (raiz3 * bar1->vnom_kv); // A
vet_corr_a[0] = corrente1 * ibase;
vet_corr_a[1] = corrente2 * ibase;
perda1        = 1000. * v1 * conj(corrente1) * geralC->Sbase; // kVA
perda2        = 1000. * v2 * conj(corrente2) * geralC->Sbase; // kVA
vet_s_kva[0]  = perda1;                                       // Pot. que entra na barra inicial
vet_s_kva[1]  = perda2;                                       // Pot. que entra na barra final
perda_kva     = perda1 + perda2;                              // Perda no trecho (kVA)

// Acumula perdas
geralC->perda_total_2    += perda_kva;
perda_energia_diaria_kwh += perda_kva.real() * geralC->DuracaoPatamar(np);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::CalculaCorrenteInjetada(TBarra          *bar_ref,
                                                            complex<double> *vet_tensoes)
{
// Variáveis locais
int             colY, colY_global, ind_local, linY, linY_global;
complex<double> cor1, y11, y12;
complex<double> czero(0., 0.);

// Proteção
if(ynodalD_pu == NULL) return(czero);

// Calcula corrente injetada em 'bar_ref'
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
   return(czero);  // Erro
   }
linY_global = IndiceGlobal(linY);
colY_global = IndiceGlobal(colY);
cor1        = (y11 * vet_tensoes[linY_global]) + (y12 * vet_tensoes[colY_global]);

return(cor1);
}

//---------------------------------------------------------------------------
void __fastcall TTrecho::CalculaPiEquivalente(void)
{
// Variáveis locais
complex<double> gamal, zser, ypar;
complex<double> cmeio(0.5, 0.), cum(1., 0.);

// IMPORTANTE: o operador "*=" NÃO funciona com properties!!!

// Sequência zero
ypar = Y0cap_pu;
if(Abs(ypar) > 0.)
   {
   zser = Z0ser_pu;
   if(Abs(zser) > 0.)
      {
      gamal     = sqrt(zser * ypar);
      Z0ser_pu  = zser * (sinh(gamal) / gamal);
      Y0ser_pu  = cum / Z0ser_pu;
      gamal    *= cmeio;
      Y0cap_pu  = ypar * (tanh(gamal) / gamal);
      }
   }

// Sequência direta
ypar = Y1cap_pu;
if(Abs(ypar) > 0.)
   {
   zser = Z1ser_pu;
   if(Abs(zser) > 0.)
      {
      gamal     = sqrt(zser * ypar);
      Z1ser_pu  = zser * (sinh(gamal) / gamal);
      Y1ser_pu  = cum / Z1ser_pu;
      gamal    *= cmeio;
      Y1cap_pu  = ypar * (tanh(gamal) / gamal);
      }
   }
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::CalculaPotenciaInjetada(TBarra          *bar_ref,
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
int __fastcall TTrecho::IndiceGlobal(int indice_local)
{
if     (indice_local == 0) return(bar1->linhaY);
else if(indice_local == 1) return(bar2->linhaY);
else                       return(-1);
}

//---------------------------------------------------------------------------
int __fastcall TTrecho::IndiceLocal(TBarra *bar_ref)
{
if     (bar_ref == bar1) return(0);
else if(bar_ref == bar2) return(1);
else                     return(-1);
}

//---------------------------------------------------------------------------
bool __fastcall TTrecho::InsereYnodal0(VTGeralC *geralC,
                                       VTSisLin *Ynod)
{
int             colY, linY;
complex<double> y1;
complex<double> cmeio(0.5, 0.);

// Proteção
if(grupo_mutua >= 0)     return(true);     // Descarta trecho com mútua
if(! flag_chave_fechada) return(true);     // Descarta trecho com chave aberta
linY = bar1->linhaY;
colY = bar2->linhaY;
if((linY < 0) || (colY < 0)) return(true); // Descarta barras(s) desconexa(s)

// Insere yserie
Ynod->InsereQuatro(linY, colY, Y0ser_pu);  // +ii, -ij, -ji, +jj

// Insere yparalelo, se o correspondente flag permitir
if(geralC->flag_capac_LT)
   {
   y1 = cmeio * Y0cap_pu;
   Ynod->InsereDiagonal(linY, y1);
   Ynod->InsereDiagonal(colY, y1);
   }

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrecho::InsereYnodal1(VTGeralC *geralC,
                                       VTSisLin *Ynod)
{
int             colY, linY;
complex<double> y1;
complex<double> cmeio(0.5, 0.);

// Proteção
if(grupo_mutua >= 0)     return(true);     // Descarta trecho com mútua
if(! flag_chave_fechada) return(true);     // Descarta trecho com chave aberta
linY = bar1->linhaY;
colY = bar2->linhaY;
if((linY < 0) || (colY < 0)) return(true); // Descarta barras(s) desconexa(s)

// Insere yserie
Ynod->InsereQuatro(linY, colY, Y1ser_pu);  // +ii, -ij, -ji, +jj

// Insere yparalelo, se o correspondente flag permitir
if(geralC->flag_capac_LT)
   {
   y1 = cmeio * Y1cap_pu;
   Ynod->InsereDiagonal(linY, y1);
   Ynod->InsereDiagonal(colY, y1);
   }

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TTrecho::MontaYnodal1_pu(VTGeralC *geralC)
{
// Esta função é utilizada pelo Estimador1

// Variáveis locais
complex<double> y1;
complex<double> cmeio(0.5, 0.);

// Retorna se a matriz já foi montada
if(ynodalD_pu != NULL) return(true);

// Inicialização
ynodalD_pu = new complex<double> [4];

// Insere yserie
y1            =  Y1ser_pu;
ynodalD_pu[0] =  y1;
ynodalD_pu[1] = -y1;
ynodalD_pu[2] = -y1;
ynodalD_pu[3] =  y1;

// Insere yparalelo, se o correspondente flag permitir
if(geralC->flag_capac_LT)
   {
   y1             = cmeio * Y1cap_pu;
   ynodalD_pu[0] += y1;
   ynodalD_pu[3] += y1;
   }

return(true);
}

//---------------------------------------------------------------------------
int __fastcall TTrecho::NumBarras(void)
{
return(2);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::PM_GetY0cap_pu(void)
{
return(y0cap_pu);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::PM_GetY0ser_pu(void)
{
return(y0ser_pu);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::PM_GetY1cap_pu(void)
{
return(y1cap_pu);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::PM_GetY1ser_pu(void)
{
return(y1ser_pu);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::PM_GetZ0ser_pu(void)
{
return(z0ser_pu);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::PM_GetZ1ser_pu(void)
{
return(z1ser_pu);
}

//---------------------------------------------------------------------------
void __fastcall TTrecho::PM_SetY0cap_pu(complex<double> valor)
{
y0cap_pu = valor;
}

//---------------------------------------------------------------------------
void __fastcall TTrecho::PM_SetY0ser_pu(complex<double> valor)
{
y0ser_pu = valor;
}

//---------------------------------------------------------------------------
void __fastcall TTrecho::PM_SetY1cap_pu(complex<double> valor)
{
y1cap_pu = valor;
}

//---------------------------------------------------------------------------
void __fastcall TTrecho::PM_SetY1ser_pu(complex<double> valor)
{
y1ser_pu = valor;
}

//---------------------------------------------------------------------------
void __fastcall TTrecho::PM_SetZ0ser_pu(complex<double> valor)
{
z0ser_pu = valor;
}

//---------------------------------------------------------------------------
void __fastcall TTrecho::PM_SetZ1ser_pu(complex<double> valor)
{
z1ser_pu = valor;
}

//---------------------------------------------------------------------------
int __fastcall TTrecho::TipoLigacao(void)
   {
   return(Trecho1);
   }

//---------------------------------------------------------------------------
void __fastcall TTrecho::TrocaBarras(void)
{
TBarra *bar3;

bar3 = bar1;
bar1 = bar2;
bar2 = bar3;
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::Y1par_pu(VTGeralC *geralC,
                                             TBarra   *bar1)
{
// Retorna zero se as capacitâncias não foram incluídas
if(geralC->flag_capac_LT) return(complex<double>(0.5, 0.) * Y1cap_pu);
else                      return(complex<double>(0., 0.));
}

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::Z1ser2_pu(void)
{
return(z1ser_pu);
}

//---------------------------------------------------------------------------
void __fastcall TTrecho::ZeraPerdaEnergia(void)
   {
   perda_energia_diaria_kwh = 0.;
   }

//---------------------------------------------------------------------------

