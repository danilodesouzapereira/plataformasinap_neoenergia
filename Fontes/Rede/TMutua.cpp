//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMutua.h"
#include "VTBarra.h"
#include "VTTrecho.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTMutua* __fastcall NewObjMutua(void)
   {
   return(new TMutua());
   }

//---------------------------------------------------------------------------
__fastcall TMutua::TMutua(void)
   {
   //inicia atributos p/ visualização gráfica
   Show[eqp_FIXO]   = true;
   Show[eqp_CODIGO] = true;
   //define posição nos Trechos
   postre1 = postre2 = 50;
   }

//---------------------------------------------------------------------------
__fastcall TMutua::~TMutua(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTMutua* __fastcall TMutua::Clone(void)
   {
   //variáveis locais
   VTMutua *mutua;

   //cria uma nova Mutua
   if ((mutua = NewObjMutua()) != NULL)
      {//copia dados desta Mutua p/ a nova Mutua
      mutua->CopiaAtributosDe(*this);
      }
   return(mutua);
   }

//---------------------------------------------------------------------------
void __fastcall TMutua::CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2)
   {
   //variáveis locais
   int xa, ya, xb, yb;

   //determina coordenadas do primeiro Trecho da Mutua
   ptre1->CoordenadasEsquematico(xa, ya, xb, yb);
   x1 = xa + (double(xb - xa) * (postre1 / 100.));
   y1 = ya + (double(yb - ya) * (postre1 / 100.));
   //determina coordenadas do segundo Trecho da Mutua
   ptre2->CoordenadasEsquematico(xa, ya, xb, yb);
   x2 = xa + (double(xb - xa) * (postre2 / 100.));
   y2 = ya + (double(yb - ya) * (postre2 / 100.));
   }

//---------------------------------------------------------------------------
void __fastcall TMutua::CoordenadasEsquematico(int &xm, int &ym, int &x1, int &y1,
                                               int &x2, int &y2, int &x3, int &y3)
   {
   //determina coordenadas x1, y1, x2, y2
   CoordenadasEsquematico(x1, y1, x2, y2);
   //define x3, y3  = (0,0)
   x3 = y3 = 0;
   //define xm, ym
   xm = (x1+x2)/2;
   ym = (y1+y2)/2;
   }

//---------------------------------------------------------------------------
bool __fastcall TMutua::CoordenadasUtm_cm(int &x, int &y)
   {
   //variáveis locais
   int x_dummy, y_dummy;

   //determina coordenadas da Mutua
   return(CoordenadasUtm_cm(x, y, x_dummy, y_dummy));
   }

//---------------------------------------------------------------------------
bool __fastcall TMutua::CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2)
   {
   //variáveis locais
   int xa, ya, xb, yb;

   //determina coordenadas do primeiro Trecho da Mutua
   if (! ptre1->CoordenadasUtm_cm(xa, ya, xb, yb)) return(false);
   x1 = xa + (double(xb - xa) * (postre1 / 100.));
   y1 = ya + (double(yb - ya) * (postre1 / 100.));
   //determina coordenadas do segundo Trecho da Mutua
   if (! ptre2->CoordenadasUtm_cm(xa, ya, xb, yb)) return(false);
   x2 = xa + (double(xb - xa) * (postre2 / 100.));
   y2 = ya + (double(yb - ya) * (postre2 / 100.));
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMutua::CoordenadasUtm_cm(int &xm, int &ym, int &x1, int &y1,
                                          int &x2, int &y2, int &x3, int &y3)
   {
   //determina coordenadas da Mutua
   if (! CoordenadasUtm_cm(x1, y1, x2, y2)) return(false);
   //calcula xm, ym
   xm = (x1+x2)/2;
   ym = (y1+y2)/2;
   //define (x3, y3) = (xm, ym)
   x3 = xm;
   y3 = ym;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMutua::CoordenadasUtm_m(int &x, int &y)
   {
   //determina coordenadas da Mutua
   if (! CoordenadasUtm_cm(x, y)) return(false);
   //converte coodenadas p/ metro
   x /= 100;
   y /= 100;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMutua::CoordenadasUtm_m(int &x1, int &y1, int &x2, int &y2)
   {
   //determina coordenadas da Mutua
   if (! CoordenadasUtm_cm(x1, y1, x2, y2)) return(false);
   //converte coodenadas p/ metro
   x1 /= 100;
   y1 /= 100;
   x2 /= 100;
   y2 /= 100;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMutua::CoordenadasUtm_m(int &xm, int &ym, int &x1, int &y1,
                                             int &x2, int &y2, int &x3, int &y3)
   {
   //determina coordenadas da Mutua
   if (! CoordenadasUtm_cm(xm, ym, x1, y1, x2, y2, x3, y3)) return(false);
   //converte coodenadas p/ metro
   xm /= 100;
   ym /= 100;
   x1 /= 100;
   y1 /= 100;
   x2 /= 100;
   y2 /= 100;
   x3 /= 100;
   y3 /= 100;
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TMutua::CopiaAtributosDe(VTEqpto &ref)
   {
   //variáveis locais
   TMutua &mutua = (TMutua &)ref;

   //proteção
   if (ref.Tipo() != eqptoMUTUA) return;
   //copia atributos de VTEqpto
   VTEqpto::CopiaAtributosDe(ref);
   //copia dados específicos classe TMutua
   //rede      = mutua.rede;
   ptre1     = mutua.ptre1;
   ptre2     = mutua.ptre2;
   pbar_tre1 = mutua.pbar_tre1;
   pbar_tre2 = mutua.pbar_tre2;
   z0        = mutua.z0;
   z1        = mutua.z1;
   postre1   = mutua.postre1;
   postre2   = mutua.postre2;
   }

//---------------------------------------------------------------------------
void __fastcall TMutua::DefineObjBarTrecho(VTTrecho *ptre, VTBarra *pbarra)
   {
   //salva Barra de referência do Trecho
   if      (ptre == ptre1) pbar_tre1 = pbarra;
   else if (ptre == ptre2) pbar_tre2 = pbarra;
   }

//---------------------------------------------------------------------------
void __fastcall TMutua::DefineObjTrecho(VTTrecho *ptre1, VTTrecho *ptre2)
   {
   //salva ponteiros p/ os Trechos
   this->ptre1 = ptre1;
   this->ptre2 = ptre2;
   //assume Barras de referência
   pbar_tre1 = ptre1->pbarra1;
   pbar_tre2 = ptre2->pbarra1;
   }

//---------------------------------------------------------------------------
void __fastcall TMutua::DefinePosTre(VTTrecho *ptre, int x, int y)
   {
   //variáveis locais
   double postre;
   int    x1, y1, x2, y2, dx, dy;

   //obtém coordenadas do Trecho
   ptre->CoordenadasEsquematico(x1, y1, x2, y2);
   //determina dx, dy do Trecho
   dx = abs(x2 - x1);
   dy = abs(y2 - y1);
   //verifica se Trecho vertical ou horizontal
   if (ptre->Vertical())
      {//calcula posição no Trecho (0 a 100%)
      if (dy == 0)
         {postre = 50;}
      else
         {postre = (abs(y - y1)*100.)/dy;}
      }
   else
      {//barra mais horizontal
      if (dx == 0)
         {postre = 50;}
      else
         {postre = (abs(x - x1)*100.)/dx;}
      }
   //proteção
   if(postre > 100.) postre = 100.;
   if(postre < 0.)   postre = 0.;
   //atualiza posição
   if      (ptre == ptre1) postre1 = postre;
   else if (ptre == ptre2) postre2 = postre;
   }

//---------------------------------------------------------------------------
bool __fastcall TMutua::ExisteTrecho(VTTrecho* trecho)
   {
   return((trecho == ptre1)||(trecho == ptre2));
   }

//---------------------------------------------------------------------------
bool __fastcall TMutua::ExisteTrechoComum(VTMutua *mutua)
   {
   //proteção
   if (mutua == NULL) return(false);
   //verifica se um dos Trechos é comum
   if (ExisteTrecho(mutua->ptre1)) return(true);
   if (ExisteTrecho(mutua->ptre2)) return(true);

   return(false);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TMutua::Rede(void)
   {
   return(ptre1->rede);
   }
   
//---------------------------------------------------------------------------
int __fastcall TMutua::Tipo(void)
   {
   return(eqptoMUTUA);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMutua::TipoAsString(void)
   {
   return("Mutua");
   }

//---------------------------------------------------------------------------
bool __fastcall TMutua::Z0_pu(complex<double> &z0_pu, double sbase)
   {
   //variáveis locais
   double   comp, zbase;
   VTTrecho *ptrecho;

   //proteção:
   if ((sbase == 0.)||(ptre1->pbarra1->vnom == 0.)) return(false);
   //determina Trecho de menor comprimento
   ptrecho = (ptre1->Comprimento_km < ptre2->Comprimento_km) ? ptre1 : ptre2;
   //calcula Zbase
   zbase = (ptre1->pbarra1->vnom * ptre1->pbarra1->vnom) / sbase;
   //calcula z0_pu em ohm e converte p/ pu, forçando r0 e r1 nulos
   z0_pu = (complex<double>(0., z0.x) * ptrecho->Comprimento_km) / zbase;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMutua::Z1_pu(complex<double> &z1_pu, double sbase)
   {
   //variáveis locais
   double   comp, zbase;
   VTTrecho *ptrecho;

   //proteção:
   if ((sbase == 0.)||(ptre1->pbarra1->vnom == 0.)) return(false);
   //determina Trecho de menor comprimento
   ptrecho = (ptre1->Comprimento_km < ptre2->Comprimento_km) ? ptre1 : ptre2;
   //calcula Zbase
   zbase = (ptre1->pbarra1->vnom * ptre1->pbarra1->vnom) / sbase;
   //calcula z1_pu em ohm e converte p/ pu, forçando r1 nulo
   z1_pu = (complex<double>(0., z1.x) * ptrecho->Comprimento_km) / zbase;
   return(true);
   }

//---------------------------------------------------------------------------
//eof
