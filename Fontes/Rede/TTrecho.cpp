//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TTrecho.h"
#include "VTMatC.h"
#include "VTMatZ.h"
#include "VTRede.h"
#include "VTResFlowLig.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe TTrecho
//---------------------------------------------------------------------------
VTTrecho* __fastcall NewObjTrecho(int num_pat)
   {
   return(new TTrecho(num_pat));
   }

//---------------------------------------------------------------------------
__fastcall TTrecho::TTrecho(int num_pat)
   {
   //variávei locais
   int num_bar = 2;

   //cria objeto VTResFlowLig p/ armazenar resultados de fluxo
   AlocaResFlowLig(num_bar, num_pat);
   //inicia atributos p/ visualização gráfica
   Show[lig_P] = true;
   Show[lig_Q] = true;
   }

//---------------------------------------------------------------------------
__fastcall TTrecho::~TTrecho(void)
   {
   //destrói Arranjo (Trecho de rede reduzida)
   if ((arranjo)&&(arranjo->Owner == this))
      {delete arranjo; arranjo = NULL;}
   //
   //OBS: a classe base TLigacao destrói o objeto VTResFlowLig
   }

//---------------------------------------------------------------------------
VTTrecho* __fastcall TTrecho::Clone(void)
   {
   //variáveis locais
   VTTrecho *trecho;

   //cria um novo Trecho
   if ((trecho = NewObjTrecho(resflow->Max_pat)) != NULL)
      {//copia dados deste Trecho p/ o novo Trecho
      trecho->CopiaAtributosDe(*this);
      }
   return(trecho);
   }

//---------------------------------------------------------------------------
void __fastcall TTrecho::CopiaAtributosDe(VTEqpto &ref)
   {
   //variáveis locais
   TList    *lisPTO_TRE;
   TTrecho  &trecho = (TTrecho&)ref;

   //proteção
   if (ref.Tipo() != eqptoTRECHO) return;
   //copia atributos de TLigacao
   TLigacao::CopiaAtributosDe(trecho);
   //copia atributos de VTTrecho
   comp = trecho.comp;
   //destrói eventual Arranjo próprio
   if ((arranjo)&&(arranjo->Owner == this))
      {delete arranjo; arranjo = NULL;}
   //verifica o Trecho possui um arranjo
   if (trecho.arranjo != NULL)
      {//verifica se o Arranjo pertence ao Trecho
      if (trecho.arranjo->Owner == &trecho)
         {//Arranjo pertence ao Trecho: cria uma cópia
         arranjo = trecho.arranjo->Clone();
         arranjo->Owner = this;
         }
      else
         {//Arranjo não pertence ao Trecho
         arranjo = trecho.arranjo;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTrecho::DefineObjArranjo(VTArranjo *arranjo)
   {
   //destrói eventual Arranjo próprio existente
   if ((this->arranjo)&&(this->arranjo->Owner == this))
      {
      delete this->arranjo;
      }
   //salva ponteiro p/ objeto
   this->arranjo = arranjo;
   //se Arranjo não está associado a outro objeto, associa c/ este Trecho
   if (arranjo->Owner == NULL)
      {
      arranjo->Owner  = this;
      arranjo->Codigo = "próprio";
      }
   }

//---------------------------------------------------------------------------
int __fastcall TTrecho::Fases(VTBarra *barra)
	{
	return((arranjo == NULL) ? faseINV : arranjo->Fases);
	}
	
//---------------------------------------------------------------------------
void __fastcall TTrecho::LisMutua(TList *lisMUTUA)
   {
   //retorna lista de Mútuas da Rede que conectam este Trecho
   rede->LisMutua(lisMUTUA, this);
   }

//---------------------------------------------------------------------------
bool __fastcall TTrecho::PM_GetArranjoTipico(void)
   {
   //proteção
   if (arranjo == NULL) return(false);
   return(arranjo->Owner != this);
   }

//---------------------------------------------------------------------------
double __fastcall TTrecho::PM_GetCapacidade_amp(void)
   {
   return(arranjo->Iadm_amp);
   }

//---------------------------------------------------------------------------
double __fastcall TTrecho::PM_GetComprimento_m(void)
   {
   return(Comprimento_km * 1000.);
   }

//---------------------------------------------------------------------------
double __fastcall TTrecho::PM_GetComprimento_km(void)
   {
   //variáveis locais
   int x1, y1, x2, y2;

   //proteção
   if (IsDoubleZero(comp, 1e-5))
      {//calcula comprimento em função das coordenadas utm
      if (CoordenadasUtm_cm(x1, y1, x2, y2))
         {//calcula comprimento em função das coordenadas utm
         comp = double(Distancia_cm(x1, y1, x2, y2)) * 1e-5;
         }
      //garante comprimento mínimo de 1 metro (1e-3 km)
      if (IsDoubleZero(comp, 1e-5)) comp = 1e-3;
      }
   return(comp);
   }

//---------------------------------------------------------------------------
bool __fastcall TTrecho::PM_GetEquilibrado(void)
   {
   //verifica o tipo de Arranjo
   switch(arranjo->TipoCfg)
      {
      case arranjoCABO_Z0Z1: return(true);
      case arranjoZ0Z1:      return(true);
      case arranjoCABOxFASE:
         //verifica se utiliza as 3 fases (com ou sem neutro)
         if (arranjo->Fases == faseABC)  return(true);
         if (arranjo->Fases == faseABCN) return(true);
         return(false);
      case arranjoMAT_Z:     return(false);
         //verifica simetria de MatZ
         //return(arranjo->matZ->Simetrica);
      case arranjoMUTUA:     return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
double __fastcall TTrecho::PM_GetIadm_amp(int nf)
   {
   return(arranjo->Ifase_amp[nf]);
   }

//---------------------------------------------------------------------------
VTMatC* __fastcall TTrecho::PM_GetMatC(void)
   {
   return(arranjo->MatC);
   }

//---------------------------------------------------------------------------
double __fastcall TTrecho::PM_GetMatC_siemens(int i, int j)
   {
   return(arranjo->Ycap_siemens_km[i][j] * comp);
   }

//---------------------------------------------------------------------------
VTMatZ* __fastcall TTrecho::PM_GetMatZ(void)
   {
   return(arranjo->MatZ);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::PM_GetMatZ_ohm(int i, int j)
   {
   return(arranjo->Z_ohm_km[i][j] * double(comp));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TTrecho::PM_GetMatZ_ohm_km(int i, int j)
   {
   return(arranjo->Z_ohm_km[i][j]);
   }

//---------------------------------------------------------------------------
bool __fastcall TTrecho::PM_GetRamalLigacao(void)
	{
	return(arranjo->ramal);
	}

//---------------------------------------------------------------------------
bool __fastcall TTrecho::PM_GetRural(void)
	{
	return(false);
	}

//---------------------------------------------------------------------------
bool __fastcall TTrecho::PM_GetUtilizaNeutro(void)
   {
   switch(arranjo->Fases)
      {
      case faseAN:   return(true);
      case faseBN:   return(true);
      case faseCN:   return(true);
      case faseABN:  return(true);
      case faseBCN:  return(true);
      case faseCAN:  return(true);
      case faseABCN: return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TTrecho::PM_SetComprimento_km(double comp_km)
   {
   comp = comp_km;
   }

//---------------------------------------------------------------------------
bool __fastcall TTrecho::Z0_ohm(double &r0, double &x0, double &yc0)
   {
   //variáveis locais
   strIMP z0;

   //obtém Z0 do Arranjo
   if (! arranjo->Z0_ohm(comp, z0, yc0)) return(false);
   //atualiza parâmetros de retorno
   r0  = z0.r;
   x0  = z0.x;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TTrecho::Z0_pu(double Sbase, double Vbase, double &r0, double &x0, double &yc0)
   {
   //variáveis locais
   double yc0_pu;
   strIMP z0_pu;

   //obtém Z0 do Arranjo
   if (! arranjo->Z0_pu(Sbase, Vbase, comp, z0_pu, yc0_pu)) return(false);
   //atualiza parâmetros de retorno
   r0  = z0_pu.r;
   x0  = z0_pu.x;
   yc0 = yc0_pu;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TTrecho::Z1_ohm(double &r1, double &x1, double &yc1)
   {
   //variáveis locais
   strIMP z1;

   //obtém Z1 do Arranjo
   if (! arranjo->Z1_ohm(comp, z1, yc1)) return(false);
   //atualiza parâmetros de retorno
   r1  = z1.r;
   x1  = z1.x;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TTrecho::Z1_pu(double Sbase, double Vbase, double &r1, double &x1, double &yc1)
   {
   //variáveis locais
   double yc1_pu;
   strIMP z1_pu;

   //obtém Z1 do Arranjo
   if (! arranjo->Z1_pu(Sbase, Vbase, comp, z1_pu, yc1_pu)) return(false);
   //atualiza parâmetros de retorno
   r1  = z1_pu.r;
   x1  = z1_pu.x;
   yc1 = yc1_pu;
   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TTrecho::Tipo(void)
   {
   return(eqptoTRECHO);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTrecho::TipoAsString(void)
   {
   return("Trecho");
   }
/*
//---------------------------------------------------------------------------
void __fastcall TTrecho::Z0_ohm(strIMP &z, double &y)
   {
   z.r = z0.r * comp;
   z.x = z0.x * comp;
   y   = 0.;
   }

//---------------------------------------------------------------------------
void __fastcall TTrecho::Z0_pu(strIMP &z, double &y, double Sbase, double Vbase)
   {
   //variáveis locais
   double Zbase = (Vbase * Vbase) / Sbase;

   z.r = (z0.r * comp) / Zbase;
   z.x = (z0.x * comp) / Zbase;
   y   = 0.;
   }

//---------------------------------------------------------------------------
void __fastcall TTrecho::Z1_ohm(strIMP &z, double &y)
   {
   z.r = z1.r * comp;
   z.x = z1.x * comp;
   y   = 0.;
   }

//---------------------------------------------------------------------------
void __fastcall TTrecho::Z1_pu(strIMP &z, double &y, double Sbase, double Vbase)
   {
   //variáveis locais
   double Zbase = (Vbase * Vbase) / Sbase;

   z.r = (z1.r * comp) / Zbase;
   z.x = (z1.x * comp) / Zbase;
   y   = 0.;
   }
*/
//---------------------------------------------------------------------------
//eof
