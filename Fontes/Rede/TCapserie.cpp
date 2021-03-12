//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCapserie.h"
#include "VTResFlowLig.h"
#include "..\Constante\Fases.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCapserie* __fastcall NewObjCapserie(int num_pat)
   {
   return(new TCapserie(num_pat));
   }
   
//---------------------------------------------------------------------------
__fastcall TCapserie::TCapserie(int num_pat)
   {
   //variávei locais
   int num_bar = 2;

   //inicia atributos p/ visualização gráfica
   Show[eqp_FIXO]   = true;
   Show[eqp_CODIGO] = true;
   //cria objeto VTResFlowLig p/ armazenar resultados de fluxo
   AlocaResFlowLig(num_bar, num_pat);
   }

//---------------------------------------------------------------------------
__fastcall TCapserie::~TCapserie(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTCapserie* __fastcall TCapserie::Clone(void)
   {
   //variáveis locais
	VTCapserie *capserie;

   //cria umum novo Capserie
	if ((capserie = new TCapserie(resflow->Max_pat)) != NULL)
      {//copia dados deste Capserie p/ o novo Capserie
		capserie->CopiaAtributosDe(*this);
      }
	return(capserie);
   }

//---------------------------------------------------------------------------
int __fastcall TCapserie::Fases(VTBarra *barra)
	{
	return(faseABC);
	}

//---------------------------------------------------------------------------
void __fastcall TCapserie::CopiaAtributosDe(VTEqpto &ref)
   {
   //variáveis locais
	VTCapserie &capserie = (VTCapserie&)ref;

   //proteção
	if (ref.Tipo() != eqptoCAPSERIE) return;
   //copia atributos de TLigacao
	TLigacao::CopiaAtributosDe(capserie);
   //copia atributos de VTCapserie
   vnom = capserie.vnom;
   snom = capserie.snom;
   z    = capserie.z;
	}

//---------------------------------------------------------------------------
int __fastcall TCapserie::Tipo(void)
   {
   return(eqptoCAPSERIE);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCapserie::TipoAsString(void)
   {
   return("Capacitor Série");
   }

//---------------------------------------------------------------------------
void __fastcall TCapserie::Z0_pu(double Sbase, double Vbase, double &r0, double &x0)
   {
   //variáveis locais
   double Zbase;

   //proteção
   if ((Sbase == 0.)||(Vbase == 0)) return;
   //calcula Zbase em ohm
   Zbase = (Vbase * Vbase)/Sbase;
   r0 =   fabs(z.r) / Zbase;
   x0 = - fabs(z.x) / Zbase;
   }

//---------------------------------------------------------------------------
void __fastcall TCapserie::Z1_pu(double Sbase, double Vbase, double &r1, double &x1)
   {
   //variáveis locais
   double Zbase;

   //proteção
   if ((Sbase == 0.)||(Vbase == 0)) return;
   //calcula Zbase em ohm
   Zbase = (Vbase * Vbase)/Sbase;
   r1 = z.r / Zbase;
   x1 = z.x / Zbase;
   }

//---------------------------------------------------------------------------
//eof
