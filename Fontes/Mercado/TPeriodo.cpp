//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPeriodo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TPeriodo::TPeriodo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TPeriodo::~TPeriodo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TPeriodo::ContemAno(int ano)
   {
   //verifica se o perido contém o ano indicado
   return ((ano >= AnoInicial)&&(ano <= AnoFinal));
   }

//---------------------------------------------------------------------------
double __fastcall TPeriodo::PM_GetValor(void)
   {
   return(PD.valor);
   }

//---------------------------------------------------------------------------
void __fastcall TPeriodo::PM_SetValor(double valor)
   {
   PD.valor = valor;
   }
   
//---------------------------------------------------------------------------
//eof
