//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TCanal.h"
#include "VTEqpto.h"
#include "..\Constante\Const.h"
#include "..\Constante\Fases.h"
#include "..\Curvas\VTCurva.h"
#include "..\Patamar\VTPatamar.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCanal* __fastcall NewObjCanal(void)
   {
   return(new TCanal());
   }

//---------------------------------------------------------------------------
__fastcall TCanal::TCanal(void)
   {
   //inicia dados
   Fases            = faseINV;
   Tolerancia_perc  = TOLERANCIA_DEFAULT;
	Enabled          = true;
   Eqpto            = NULL;
	Curva            = NULL;
	}

//---------------------------------------------------------------------------
__fastcall TCanal::~TCanal(void)
   {
   if (PD.curva) {delete PD.curva; PD.curva = NULL;}
   }

//---------------------------------------------------------------------------
VTCanal* __fastcall TCanal::Clone(void)
   {
   //variáveis locais
   TCanal *canal;

   //cria um novo Canal
   if ((canal = (TCanal*)NewObjCanal()) != NULL)
      {//copia dados deste Canal p/ o novo Canal
		canal->CopiaAtributosDe(*this);
      }
   return(canal);
   }

//---------------------------------------------------------------------------
void __fastcall TCanal::CopiaAtributosDe(VTEqpto &ref)
	{
	//variáveis locais
	TCanal &canal = (TCanal&)ref;

	//proteção
	if (ref.Tipo() != eqptoCANAL) return;
	//copia atributos de VTEqpto
	VTEqpto::CopiaAtributosDe(canal);
	//destrói Curva se já existir
	if (Curva) delete Curva;
	//copia atributos do Canal de referência
	PD      = canal.PD;
	Curva   = NULL;
	//copia Curva do Canal de referência
	if (canal.Curva != NULL) Curva = canal.Curva->Clone();
	}

//---------------------------------------------------------------------------
VTCurva* __fastcall TCanal::ExisteCurva(void)
   {
   return(PD.curva);
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TCanal::PM_GetCurva(void)
   {
   return(PD.curva);
   }

//---------------------------------------------------------------------------
bool __fastcall TCanal::PM_GetDescartavel(void)
	{
	return(PD.descartavel);
	}

//---------------------------------------------------------------------------
bool __fastcall TCanal::PM_GetEnabled(void)
	{
	return(PD.enabled);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TCanal::PM_GetEqpto(void)
   {
   return(PD.eqpto);
   }

//---------------------------------------------------------------------------
double __fastcall TCanal::PM_GetErro_pu(VTPatamar *patamar)
   {
   //proteção
   if (IsDoubleZero(Tolerancia_perc)) Tolerancia_perc = TOLERANCIA_DEFAULT;
   //calcula ero em número de valores de sigma
   return((ValorMedido[patamar] - ValorEstimado[patamar]) / Sigma_pu);
   }

//---------------------------------------------------------------------------
double __fastcall TCanal::PM_GetErroMaximo_pu(void)
   {
   //variáveis locais
   int    num_pto;
   double erro_pu;
   double erro_max = 0;
   double vet_valor[nvCURVA_CANAL];
   strHM  hm_ini, hm_fim;

   //proteção
   if (PD.curva == NULL) return(0.);
   //determina número de pontos da Curva
   num_pto = PD.curva->NumeroPontos();
   for (int index = 0; index < num_pto; index++)
      {//determina valores do ponto
      if (! PD.curva->GetPonto(index, hm_ini, hm_fim, vet_valor, nvCURVA_CANAL)) continue;
      erro_pu = (vet_valor[IND_VAL_MEDIDO] - vet_valor[IND_VAL_ESTIMADO]) / Sigma_pu;
      if (index == 0) erro_max = erro_pu;
      else if (fabs(erro_pu) > fabs(erro_max)) erro_max = erro_pu;
      }
   return(erro_max);
   }

//---------------------------------------------------------------------------
int __fastcall TCanal::PM_GetFases(void)
   {
	return(PD.fases);
	}

//---------------------------------------------------------------------------
double __fastcall TCanal::PM_GetFatorEstimadoMedido(VTPatamar *patamar)
	{
   //variáveis locais
   strHM  hm;
   double val_medido, val_estimado;
	double vet_valor[nvCURVA_CANAL];

	hm.hora   = patamar->HoraIni;
	hm.minuto = patamar->MinutoIni;
	//determina valor da Curva no patamar indicado
	if (! PD.curva->GetValor(hm, vet_valor, nvCURVA_CANAL))
      {return(0.);}
	val_medido   = vet_valor[IND_VAL_MEDIDO];
	val_estimado = vet_valor[IND_VAL_ESTIMADO];
   //proteção
   if (IsDoubleZero(val_medido, 1e-6))
      {return(0.);}
   else
      {return(val_estimado / val_medido);}
	}

//---------------------------------------------------------------------------
double __fastcall TCanal::PM_GetFatorNominalMedido(VTPatamar *patamar)
	{
   //variáveis locais
   strHM  hm;
   double val_medido, val_nominal;
	double vet_valor[nvCURVA_CANAL];

	hm.hora   = patamar->HoraIni;
	hm.minuto = patamar->MinutoIni;
	//determina valor da Curva no patamar indicado
	if (! PD.curva->GetValor(hm, vet_valor, nvCURVA_CANAL))
      {return(0.);}
	val_medido  = vet_valor[IND_VAL_MEDIDO];
	val_nominal = vet_valor[IND_VAL_NOMINAL];
   //proteção
   if (IsDoubleZero(val_medido, 1e-6))
      {return(0.);}
   else
      {return(val_nominal / val_medido);}
	}

//---------------------------------------------------------------------------
VTMedidor* __fastcall TCanal::PM_GetMedidor(void)
   {
   return(PD.medidor);
   }

//---------------------------------------------------------------------------
int __fastcall TCanal::PM_GetNumero(void)
   {
   return(PD.num_seq);
	}

//---------------------------------------------------------------------------
double __fastcall TCanal::PM_GetSigma_pu(void)
   {
   return((PD.tolerancia_perc * 0.01) / 1.645);
	}

//---------------------------------------------------------------------------
int __fastcall TCanal::PM_GetTipoCanal(void)
   {
   return(PD.tipo_canal);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCanal::PM_GetTipoCanalAsString(void)
	{
   //variáveis locais
   AnsiString txt;

   //verifica o o tipo do canal
	switch(TipoCanal)
		{
		case canalV:  txt = "V";  break;
		case canalI:  txt = "I";  break;
		case canalP:  txt = "P";  break;
		case canalQ:  txt = "Q";  break;
		case canalS:  txt = "S";  break;
		default:      return("Indefinido");
		}
/*
   //inclui informação da fase
   switch(Fases)
      {
      case faseA:   txt += "a";    break;
      case faseB:   txt += "b";    break;
      case faseC:   txt += "c";    break;
      case faseAB:  txt += "ab";   break;
      case faseBC:  txt += "bc";   break;
      case faseCA:  txt += "ca";   break;
      case faseABC: txt += "abc";  break;
      default:                     break;
      }
*/
   return(txt);
	}

//---------------------------------------------------------------------------
double __fastcall TCanal::PM_GetTolerancia_perc(void)
   {
   return(PD.tolerancia_perc);
   }

//---------------------------------------------------------------------------
double __fastcall TCanal::PM_GetValorEstimado(VTPatamar *patamar)
	{
   //variáveis locais
   strHM  hm;
	double vet_valor[nvCURVA_CANAL];

	hm.hora   = patamar->HoraIni;
	hm.minuto = patamar->MinutoIni;
	//determina valor da Curva no patamar indicado
	if (! PD.curva->GetValor(hm, vet_valor, nvCURVA_CANAL)) return(0.);
	return(vet_valor[IND_VAL_ESTIMADO]);
	}

//---------------------------------------------------------------------------
double __fastcall TCanal::PM_GetValorNominal(VTPatamar *patamar)
	{
   //variáveis locais
   strHM  hm;
	double vet_valor[nvCURVA_CANAL];

	hm.hora   = patamar->HoraIni;
	hm.minuto = patamar->MinutoIni;
	//determina valor da Curva no patamar indicado
	if (! PD.curva->GetValor(hm, vet_valor, nvCURVA_CANAL)) return(0.);
	return(vet_valor[IND_VAL_NOMINAL]);
	}

//---------------------------------------------------------------------------
double __fastcall TCanal::PM_GetValorMedido(VTPatamar *patamar)
	{
   //variáveis locais
   strHM  hm;
	double vet_valor[nvCURVA_CANAL];

	hm.hora   = patamar->HoraIni;
	hm.minuto = patamar->MinutoIni;
	//determina valor da Curva no patamar indicado
	if (! PD.curva->GetValor(hm, vet_valor, nvCURVA_CANAL)) return(0.);
	return(vet_valor[IND_VAL_MEDIDO]);
	}
/*
//---------------------------------------------------------------------------
void __fastcall TCanal::PM_SetCurva(VTCurva *curva)
	{
	//proteções
	if (curva == NULL)     return;
   if (curva == PD.curva) return;
   //destrói eventual Curva atual
   if (PD.curva != NULL) delete PD.curva;
	//salva ponteiro p/ Curva
	PD.curva = curva;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TCanal::PM_SetCurva(VTCurva *curva)
	{
	//proteção
	if (curva == NULL) return;
	//salva ponteiro p/ Curva
	PD.curva = curva;
   }

//---------------------------------------------------------------------------
void __fastcall TCanal::PM_SetDescartavel(bool descartavel)
	{
	PD.descartavel = descartavel;
	}

//---------------------------------------------------------------------------
void __fastcall TCanal::PM_SetEnabled(bool enabled)
	{
	PD.enabled = enabled;
	}

//---------------------------------------------------------------------------
void __fastcall TCanal::PM_SetEqpto(VTEqpto *eqpto)
   {
   PD.eqpto = eqpto;
   }

//---------------------------------------------------------------------------
void __fastcall TCanal::PM_SetFases(int fases)
	{
	PD.fases = fases;
	}

//---------------------------------------------------------------------------
void __fastcall TCanal::PM_SetMedidor(VTMedidor *medidor)
   {
   PD.medidor = medidor;
   }

//---------------------------------------------------------------------------
void __fastcall TCanal::PM_SetNumero(int numero)
   {
	PD.num_seq = numero;
	}

//---------------------------------------------------------------------------
void __fastcall TCanal::PM_SetTipoCanal(int tipo_canal)
   {
   PD.tipo_canal = tipo_canal;
   }

//---------------------------------------------------------------------------
void __fastcall TCanal::PM_SetTolerancia_perc(double tolerancia_perc)
   {
   PD.tolerancia_perc = tolerancia_perc;
   }

//---------------------------------------------------------------------------
void __fastcall TCanal::PM_SetValorEstimado(VTPatamar *patamar, double valor)
	{
   //variáveis locais
   strHM  hm;
	double vet_valor[nvCURVA_CANAL];

   hm.hora   = patamar->HoraIni;
	hm.minuto = patamar->MinutoIni;
	//determina valores da Curva no patamar indicado
	if (PD.curva->GetValor(hm, vet_valor, nvCURVA_CANAL))
		{//atualiza valor
		vet_valor[IND_VAL_ESTIMADO] = valor;
		//atualiza ponto da Curva
		PD.curva->SetPonto(hm, vet_valor, nvCURVA_CANAL);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TCanal::PM_SetValorNominal(VTPatamar *patamar, double valor)
	{
   //variáveis locais
   strHM  hm;
	double vet_valor[nvCURVA_CANAL];

   hm.hora   = patamar->HoraIni;
	hm.minuto = patamar->MinutoIni;
	//determina valores da Curva no patamar indicado
	if (PD.curva->GetValor(hm, vet_valor, nvCURVA_CANAL))
		{//atualiza valor
		vet_valor[IND_VAL_NOMINAL] = valor;
		//atualiza ponto da Curva
		PD.curva->SetPonto(hm, vet_valor, nvCURVA_CANAL);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TCanal::PM_SetValorMedido(VTPatamar *patamar, double valor)
	{
   //variáveis locais
   strHM  hm;
	double vet_valor[nvCURVA_CANAL];

   hm.hora   = patamar->HoraIni;
	hm.minuto = patamar->MinutoIni;
	//determina valores da Curva no patamar indicado
	if (PD.curva->GetValor(hm, vet_valor, nvCURVA_CANAL))
		{//atualiza valor
		vet_valor[IND_VAL_MEDIDO] = valor;
		//atualiza ponto da Curva
		PD.curva->SetPonto(hm, vet_valor, nvCURVA_CANAL);
		}
	}

//---------------------------------------------------------------------------
int __fastcall TCanal::Tipo(void)
   {
   return(eqptoCANAL);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCanal::TipoAsString(void)
   {
   return("Canal");
   }

//---------------------------------------------------------------------------
//eof
