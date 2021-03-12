//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TSup.h"
#include ".\TBarra.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TSup::TSup(TBarra *barra,
                      int    tipo)
   {
   // Salva ponteiro
   this->barra = barra;

   // Inicialização
   eqpto = NULL;

   // Redefine tipo da barra e inicializa tensão, admitâncias equivalentes e
   // potência especificada
   barra->tipo          = tipo;
   barra->tipo_original = tipo;
   barra->sup           = this;
   fp_min               = 0.;
   tensao_pu            = complex<double>(1., 0.);
   y0_pu                = complex<double>(0., 0.);
   y1_pu                = complex<double>(0., 0.);
   ZeraPotencia();
   }

//---------------------------------------------------------------------------
__fastcall TSup::~TSup(void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TSup::ZeraPotencia(void)
   {
   sesp_pu = complex<double>(0., 0.);
   }

//---------------------------------------------------------------------------






