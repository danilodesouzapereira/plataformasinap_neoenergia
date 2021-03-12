//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCabo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCabo
//---------------------------------------------------------------------------
VTCabo* __fastcall NewObjCabo(void)
   {
   return(new TCabo());
   }
   
//---------------------------------------------------------------------------
__fastcall TCabo::TCabo(void)
   {
   Id          = -1;
   Codigo      = "";
   TipoCfg     = caboR_GMR;
   Iadm_A      = 0.;
   Vmax_KV     = -1.;
   Vmin_KV     = -1;
   r           = 0.;
   gmr         = 0.;
   c0          = 0.;
   c1          = 0.;
   //cor         = clBlue;
   //altera Status do Cabo
   Status[sttNOVO] = true;
   }

//---------------------------------------------------------------------------
__fastcall TCabo::~TCabo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTCabo* __fastcall TCabo::Clone(void)
   {
   //variáveis locais
   VTCabo *cabo;

   //cria um novo Cabo
   if ((cabo = NewObjCabo()) != NULL)
      {//copia dados deste Cabo p/ o novo Cabo
		cabo->CopiaAtributosDe(*this);
      }
   return(cabo);
   }

//---------------------------------------------------------------------------
void __fastcall TCabo::CopiaAtributosDe(VTEqpto &ref)
	{
   //variáveis locais
	TCabo &cabo = (TCabo&)ref;

	//proteção
	if (ref.Tipo() != eqptoCABO) return;
	//copia atributos do Eqpto
	VTEqpto::CopiaAtributosDe(cabo);
	//copia atributos do Cabo
	PD       = cabo.PD;
	r        = cabo.r;
	gmr      = cabo.gmr;
	z0       = cabo.z0;
	z1       = cabo.z1;
	c0       = cabo.c0;
	c1       = cabo.c1;
	}

//---------------------------------------------------------------------------
double __fastcall TCabo::PM_GetIadm_A(void)
   {
   return(PD.iadm);
   }

//---------------------------------------------------------------------------
int __fastcall TCabo::PM_GetTipoCfg(void)
   {
   return(PD.tipo_cfg);
   }

//---------------------------------------------------------------------------
double __fastcall TCabo::PM_GetVmax_KV(void)
   {
   return(PD.vmax);
   }

//---------------------------------------------------------------------------
double __fastcall TCabo::PM_GetVmin_KV(void)
   {
   return(PD.vmin);
   }

//---------------------------------------------------------------------------
void __fastcall TCabo::PM_SetCodigo(AnsiString codigo)
   {
   //proteção contra dimensão do string
   if (codigo.Length() > 128) codigo = codigo.SubString(1, 128);
   VTEqpto::PD.codigo = codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TCabo::PM_SetIAdm_A(double iadm)
   {
   PD.iadm = iadm;
   }

//---------------------------------------------------------------------------
void __fastcall TCabo::PM_SetTipoCfg(int tipo_cfg)
   {
   PD.tipo_cfg = tipo_cfg;
   }

//---------------------------------------------------------------------------
void __fastcall TCabo::PM_SetVmax_KV(double vmax)
   {
   PD.vmax = vmax;
   }

//---------------------------------------------------------------------------
void __fastcall TCabo::PM_SetVmin_KV(double vmin)
   {
   PD.vmin = vmin;
   }

//---------------------------------------------------------------------------
int __fastcall TCabo::Tipo(void)
	{
	return(eqptoCABO);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCabo::TipoAsString(void)
   {
   return("Cabo");
   }

//---------------------------------------------------------------------------
//eof

 