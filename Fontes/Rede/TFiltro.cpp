//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFiltro.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFiltro* __fastcall NewObjFiltro(void)
   {
   return(new TFiltro());
   }
   
//---------------------------------------------------------------------------
__fastcall TFiltro::TFiltro(void)
   {
   //inicia atributos p/ visualização gráfica
   Show[eqp_FIXO]   = true;
   Show[eqp_CODIGO] = true;
   }

//---------------------------------------------------------------------------
__fastcall TFiltro::~TFiltro(void)
   {
   //falta implementar
   }

//---------------------------------------------------------------------------
VTFiltro* __fastcall TFiltro::Clone(void)
   {
   //variáveis locais
   VTFiltro *filtro;

   //cria umum novo Filtro
   if ((filtro = NewObjFiltro()) != NULL)
      {//copia dados deste Filtro p/ o novo Filtro
      filtro->CopiaAtributosDe(*this);
      }
   return(filtro);
   }

//---------------------------------------------------------------------------
void __fastcall TFiltro::CopiaAtributosDe(VTEqpto &ref)
   {
   //variáveis locais
   VTFiltro &filtro = (VTFiltro&)ref;

   //proteção
   if (ref.Tipo() != eqptoFILTRO) return;
   //copia atributos de TEqbar
   TEqbar::CopiaAtributosDe(filtro);
   //copia atributos de VTFiltro
   vnom  = filtro.vnom;
   snom  = filtro.snom;
   z0    = filtro.z0;
   }

//---------------------------------------------------------------------------
int __fastcall TFiltro::Tipo(void)
   {
   return(eqptoFILTRO);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFiltro::TipoAsString(void)
   {
   return("Filtro");
   }

//---------------------------------------------------------------------------
bool __fastcall TFiltro::Z0_pu(double Sbase, strIMP &z0_pu)
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
