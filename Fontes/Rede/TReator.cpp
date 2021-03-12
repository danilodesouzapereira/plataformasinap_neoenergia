//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TReator.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTReator* __fastcall NewObjReator(int num_pat)
   {
   return(new TReator(num_pat));
   }
   
//---------------------------------------------------------------------------
__fastcall TReator::TReator(int num_pat)
   {
   //inicia atributos p/ visualiza��o gr�fica
   Show[eqp_FIXO]   = true;
   Show[eqp_CODIGO] = true;
   }

//---------------------------------------------------------------------------
__fastcall TReator::~TReator(void)
   {
   //falta implementar
   }

//---------------------------------------------------------------------------
VTReator* __fastcall TReator::Clone(void)
   {
   //vari�veis locais
   VTReator *reator;

   //cria umum novo Reator
   if ((reator = NewObjReator(curva->Escala)) != NULL)
      {//copia dados deste Reator p/ o novo Reator
      reator->CopiaAtributosDe(*this);
      }
   return(reator);
   }

//---------------------------------------------------------------------------
void __fastcall TReator::CopiaAtributosDe(VTEqpto &ref)
   {
   //vari�veis locais
   VTReator &reator = (VTReator&)ref;

   //prote��o
   if (ref.Tipo() != eqptoREATOR) return;
   //copia atributos de TEqbar
   TEqbar::CopiaAtributosDe(reator);
   //copia atributos de VTReator
   ligacao = reator.ligacao;
   vnom    = reator.vnom;
   q       = reator.q;
   //destr�i Curva pr�pria
   if ((curva)&&(! curva->Tipica)) delete curva;
   //verifica se a Curva do Reator de refer�ncia � t�pica
   if (reator.curva->Tipica)
      {//assume a mesma Curva t�pica
      curva = reator.curva;
      }
   else
      {//cria c�pia de Curva pr�pria
      curva = reator.curva->Clone();
      curva->Eqpto = this;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReator::DefineCurva(VTCurva *curva)
   {
   //salva ponteiro p/ objeto
   this->curva = curva;
   }


//---------------------------------------------------------------------------
VTCurva* __fastcall TReator::ExisteCurva(void)
   {
   return(curva);
   }

//---------------------------------------------------------------------------
AnsiString  __fastcall TReator::PM_GetPadrao(void)
   {
   return(padrao);
   }

//---------------------------------------------------------------------------
double __fastcall TReator::PM_GetQ_mvar(void)
   {
   return(q);
   }

//---------------------------------------------------------------------------
void __fastcall TReator::PM_SetPadrao(AnsiString padrao)
   {
   this->padrao = padrao;
   }

//---------------------------------------------------------------------------
int __fastcall TReator::Tipo(void)
   {
   return(eqptoREATOR);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TReator::TipoAsString(void)
   {
   return("Reator");
   }

//---------------------------------------------------------------------------
void __fastcall TReator::Z0_pu(double Sbase, double Vbase, strIMP &z0_pu)
   {
   Z1_pu(Sbase, Vbase, z0_pu);
   }

//---------------------------------------------------------------------------
void __fastcall TReator::Z1_pu(double Sbase, double Vbase, strIMP &z1_pu)
   {
   //vari�veis locais
   double Zbase, x1_ohm;

   //prote��o
   if ((Sbase == 0.)||(Vbase == 0.)) return;
   Zbase   = (Vbase * Vbase)/Sbase;
   //calcula reat�ncia em ohm
   x1_ohm = vnom * vnom / q;
   //calcula imped�ncia em pu na nova base
   z1_pu.r = 0.;
   z1_pu.x = x1_ohm/Zbase;
   }

//---------------------------------------------------------------------------
//eof
