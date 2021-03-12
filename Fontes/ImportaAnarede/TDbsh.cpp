//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDbsh.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
__fastcall TDbsh::TDbsh(void)
   {
   //cria lista
   lisINDIV = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TDbsh::~TDbsh(void)
   {
   //destroi lista e seus objetos
   if(lisINDIV) {LimpaTList(lisINDIV); delete lisINDIV; lisINDIV = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TDbsh::ApagaCapDbar(void)
   {
   if(apaga_dbar.AnsiCompareIC("N") == 0) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TDbsh::Capacitor(void)
   {
   //variáveis locais
   strINDIV *indiv;
   double   q_mvar;

   //soma todos os dados individualizados: dependendo do sinal, é ou não capacitor
   q_mvar = Valor_mvar();
   if(q_mvar >= 0) return(true);
   return(false);
   }

//---------------------------------------------------------------------------
double __fastcall TDbsh::Valor_mvar(void)
   {
   //variáveis locais
   strINDIV *indiv;
   double   q_mvar = 0.;

   for(int n = 0; n < lisINDIV->Count; n++)
      {//soma todos os dados individualizados
      indiv = (strINDIV*)lisINDIV->Items[n];
      //desconsidera se estiver desligado
      if(indiv->estado.AnsiCompareIC("D") == 0) continue;
      q_mvar += (indiv->uni_op * indiv->q_mvar);
      }
   return(q_mvar);
   }

//---------------------------------------------------------------------------
__fastcall strINDIV::strINDIV(void)
   {
   //nada
   }

//---------------------------------------------------------------------------
__fastcall strINDIV::~strINDIV(void)
   {
   //nada
   }

//-----------------------------------------------------------------------------
