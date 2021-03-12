//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TTrafoZZ.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTTrafoZZ* __fastcall NewObjTrafoZZ(void)
   {
   return(new TTrafoZZ());
   }
   
//---------------------------------------------------------------------------
__fastcall TTrafoZZ::TTrafoZZ(void)
   {
   //inicia atributos p/ visualização gráfica
   Show[eqp_FIXO]   = true;
   Show[eqp_CODIGO] = true;
   }

//---------------------------------------------------------------------------
__fastcall TTrafoZZ::~TTrafoZZ(void)
   {
   //falta implementar
   }

//---------------------------------------------------------------------------
VTTrafoZZ* __fastcall TTrafoZZ::Clone(void)
   {
   //variáveis locais
   VTTrafoZZ *trafoZZ;

   //cria umum novo TrafoZZ
   if ((trafoZZ = NewObjTrafoZZ()) != NULL)
      {//copia dados deste TrafoZZ p/ o novo TrafoZZ
      trafoZZ->CopiaAtributosDe(*this);
      }
   return(trafoZZ);
   }

//---------------------------------------------------------------------------
void __fastcall TTrafoZZ::CopiaAtributosDe(VTEqpto &ref)
   {
   //variáveis locais
   VTTrafoZZ &trafoZZ = (VTTrafoZZ&)ref;

   //proteção
   if (ref.Tipo() != eqptoTRAFOZZ) return;
   //copia atributos de TEqbar
   TEqbar::CopiaAtributosDe(trafoZZ);
   //copia atributos de VTTrafoZZ
   vnom  = trafoZZ.vnom;
   snom  = trafoZZ.snom;
   z0    = trafoZZ.z0;
   }

//---------------------------------------------------------------------------
int __fastcall TTrafoZZ::Tipo(void)
   {
   return(eqptoTRAFOZZ);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTrafoZZ::TipoAsString(void)
   {
   return("TrafoZZ");
   }

//---------------------------------------------------------------------------
bool __fastcall TTrafoZZ::Z0_pu(double Sbase, strIMP &z0_pu)
	{
	//variáveis locais
	double fat_mult;

	try{//converte z0 do filtro (pu de Snom ==> pu de Sbase)
		fat_mult = (Sbase/snom);
		z0_pu.r = z0.r * fat_mult;
		z0_pu.x = z0.x * fat_mult;
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
//eof
