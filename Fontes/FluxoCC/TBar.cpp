//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TBar.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TBar::TBar(void       *eqpto,
                      int        id,
                      AnsiString codigo)
   {
   //salva parâmetros
   this->eqpto  = eqpto;
   this->id     = id;
   this->codigo = codigo;

   //inicia dados
   tipo    = barCARGA;
   index   = 0;
   p_car   = 0.;
   p_ger   = 0.;
   vnom_kv = 13.8;

   //assume valores de resultado
   res.v_pu       = 1.;
   res.angulo_rad = 0.;
   }

//---------------------------------------------------------------------------
__fastcall TBar::~TBar(void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TBar::InsereCarga(double p)
   {
   //acumula potência de carga
   p_car += p;
   }

//---------------------------------------------------------------------------
void __fastcall TBar::InsereGeracao(double p)
   {
   //acumula potência de geração
   p_ger += p;
   }

//---------------------------------------------------------------------------
//eof
