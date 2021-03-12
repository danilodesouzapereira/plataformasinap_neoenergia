//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TBanco.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRegulador.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTBanco* __fastcall NewObjBanco(VTApl *apl)
   {
   return(new TBanco(apl));
   }

//---------------------------------------------------------------------------
__fastcall TBanco::TBanco(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   }

//---------------------------------------------------------------------------
__fastcall TBanco::~TBanco(void)
   {
   //destrói eqptos criados
   if (PD.regulador)  {delete PD.regulador;  PD.regulador  = NULL;}
   if (PD.barra_nova) {delete PD.barra_nova; PD.barra_nova = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TBanco::DefineCoordenadasBarraNova(void)
   {
   //variáveis locais
   int x1, y1, x2, y2, dx, dy;

   //obtém coordenadas da Ligacao pai
   PD.ligacao_pai->CoordenadasEsquematico(x1, y1, x2, y2);
   dx = (x2 - x1) * 0.1;
   dy = (y2 - y1) * 0.1;
   if (PD.barra_rede == PD.ligacao_pai->pbarra1)
      {
      PD.barra_nova->esq.x1 = PD.barra_nova->esq.x2 = x1 + dx;
      PD.barra_nova->esq.y1 = PD.barra_nova->esq.y2 = y1 + dy;
      }
   else
      {
      PD.barra_nova->esq.x1 = PD.barra_nova->esq.x2 = x2 - dx;
      PD.barra_nova->esq.y1 = PD.barra_nova->esq.y2 = y2 - dy;
      }
   }

//---------------------------------------------------------------------------
int __fastcall TBanco::NumUnidades(void)
   {
   //veirifica o tipo da Ligacao
   switch(Regulador->ligacao)
      {
      case lgEST_AT:        return(1);
      case lgTRIANG:        return(1);
      case lgEST_ISO:       return(1);
      case lgMONOFASICO:    return(1);
      case lgDELTA_ABERTO:  return(2);
      case lgDELTA_FECHADO: return(3);
      default:              return(1);
      }
   }

//---------------------------------------------------------------------------
double __fastcall TBanco::Snom_mvar(void)
   {
   return(NumUnidades() * Regulador->snom);
   }

//---------------------------------------------------------------------------
//eof
