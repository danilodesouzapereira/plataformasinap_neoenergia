//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Math.hpp>
#include <complex>
#include "TDemanda.h"
#include "VTCalculaDem.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTDemanda* __fastcall NewObjDemanda(VTApl *apl)
   {
   try{
      return(new TDemanda(apl));
      }catch(Exception &e)
         {//nada a fazer
         }
   //erro na criação do objeto
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TDemanda::TDemanda(VTApl *apl)
   {
   try{//salva ponteiro p/ objetos
      this->apl = apl;
      fases     = (VTFases*)apl->GetObject(__classid(VTFases));
      patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
      //cria objetos
		calcula = NewObjCalculaDem(apl);
      //inicia data de referência
      DataReferencia = Now();
      } catch (Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TDemanda::~TDemanda(void)
   {
   //destrói objetos
   if (calcula) {delete calcula; calcula = NULL;}
	}

//alteração OpenDSS
//---------------------------------------------------------------------------
bool __fastcall TDemanda::GetSmax_mva(VTEqpto *eqpto, double &s_mva, double &p_mw, double &q_mvar)
   {
   //variáveis locais
   complex<double> spat_mva, smax_mva;
   int             num_pat  = patamares->NumPatamar();

   //totaliza demanda do Eqpto em todos patamares
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {
      spat_mva = S_mva[eqpto][ind_pat];
      if (ind_pat == 0) smax_mva = spat_mva;
      if (Abs(spat_mva) > Abs(smax_mva)) smax_mva = spat_mva;
      }
   //define valores máximos
   s_mva  = Abs(smax_mva);
   p_mw   = smax_mva.real();
   q_mvar = smax_mva.imag();

   return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TDemanda::GetSmedia_mva(VTEqpto *eqpto, double &s_mva, double &p_mw, double &q_mvar)
   {
   //variáveis locais
   complex<double> soma_mva = CZero();
   int             num_pat  = patamares->NumPatamar();

   //totaliza demanda do Eqpto em todos patamares
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {
      soma_mva += S_mva[eqpto][ind_pat];
      }
   //calcula valores médios
   s_mva  = Abs(soma_mva)   / num_pat;
   p_mw   = soma_mva.real() / num_pat;
   q_mvar = soma_mva.imag() / num_pat;

   return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TDemanda::GetSmin_mva(VTEqpto *eqpto, double &s_mva, double &p_mw, double &q_mvar)
   {
   //variáveis locais
   complex<double> spat_mva, smin_mva;
   int             num_pat  = patamares->NumPatamar();

   //totaliza demanda do Eqpto em todos patamares
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {
      spat_mva = S_mva[eqpto][ind_pat];
      if (ind_pat == 0) smin_mva = spat_mva;
      if (Abs(spat_mva) < Abs(smin_mva)) smin_mva = spat_mva;
      }
   //define valores máximos
   s_mva  = Abs(smin_mva);
   p_mw   = smin_mva.real();
   q_mvar = smin_mva.imag();

   return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TDemanda::Get_S3f_mva(VTEqpto *eqpto, int np, complex<double> s_mva[MAX_FASE])
   {
   //variáveis locais
   VTCarga *carga;

   //zera o vetor com valores a serem retornados
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++) {s_mva[ind_fase] = CZero();}
   //verifica o tipo de Eqpto
   switch(eqpto->Tipo())
      {
      case eqptoCAPACITOR:   return(GetValorPorFaseCapacitor((VTCapacitor*)eqpto, np, s_mva));
      case eqptoREATOR:      return(GetValorPorFaseReator((VTReator*)eqpto, np, s_mva));
      case eqptoCARGA:
         carga = (VTCarga*)eqpto;
         if (carga->DefinidaPorFase) return(GetValorPorFaseCargaReducao(carga, np, s_mva));
         else                        return(GetValorPorFaseCarga(carga, np, s_mva));
      default: break;
      }
	return(false);
	}

//---------------------------------------------------------------------------
bool __fastcall TDemanda::GetValorPorFaseCapacitor(VTCapacitor *capacitor, int np, complex<double> s_mva[MAX_FASE])
   {
   //variáveis locais
   complex<double> s_fase;
   strDEM          str_dem;

   //zera o vetor com valores a serem retornados
   //for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++) {s_mva[ind_fase] = CZero();}
   //determina demanda por fase
	if(! calcula->GetValorCapacitor(capacitor, np, &str_dem)) return(false);
   // Distribui potência total entre as fases
   s_fase = complex<double>(str_dem.p, str_dem.q) / 3.;
   for(int ind_fase=indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {
      s_mva[ind_fase] = s_fase;
      }
   s_mva[indNEUTRO] = CZero();
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TDemanda::GetValorPorFaseCarga(VTCarga *carga, int np, complex<double> s_mva[MAX_FASE])
   {
   //variáveis locais
   double  num_fases;
   strDEM  str_dem;
   VTFases *fases = (VTFases *) apl->GetObject(__classid(VTFases));

   //zera o vetor com valores a serem retornados
   //for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++) {s_mva[ind_fase] = CZero();}
   // Verifica se a carga é bifásica sem neutro (AB, BC, CA): não é possível distribuir a demanda nas fases
   if((fases->NumeroDeFases(carga->Fases) == 2) && ((carga->Fases & faseN) != faseN)) return(false);
   //determina demanda total da Carga
   if(! calcula->GetValorCarga(carga, np, &str_dem)) return(false);
   //distribui a demanda pelas fases da Carga
   num_fases = fases->NumeroDeFases(carga->Fases);
   if (carga->Fases & faseA) s_mva[indFASE_A] = complex<double>(str_dem.p, str_dem.q) / num_fases;
   if (carga->Fases & faseB) s_mva[indFASE_B] = complex<double>(str_dem.p, str_dem.q) / num_fases;
   if (carga->Fases & faseC) s_mva[indFASE_C] = complex<double>(str_dem.p, str_dem.q) / num_fases;
   //zera demanda no neutro
   s_mva[indNEUTRO] = CZero();

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TDemanda::GetValorPorFaseCargaReducao(VTCarga *carga, int np, complex<double> s_mva[MAX_FASE])
   {
   //variáveis locais
   strDEM str_dem[MAX_FASE];

   //zera o vetor com valores a serem retornados
   //for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++) {s_mva[ind_fase] = CZero();}
   //determina demanda
   if (! calcula->GetValorCargaReducao(carga, np, str_dem)) return(false);
   //Copia demanda por fase
   for (int nf = indFASE_A; nf <= indNEUTRO; nf++)
      {
      s_mva[nf] = complex<double>(str_dem[nf].p, str_dem[nf].q);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TDemanda::GetValorPorFaseReator(VTReator *reator, int np, complex<double> s_mva[MAX_FASE])
   {
   //variáveis locais
   complex<double> s_fase;
   strDEM          str_dem;

   //zera o vetor com valores a serem retornados
   //for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++) {s_mva[ind_fase] = CZero();}
   //determina demanda
   if (! calcula->GetValorReator(reator, np, &str_dem)) return(false);
   // Distribui potência total entre as fases
   s_fase = complex<double>(str_dem.p, str_dem.q) / 3.;
   for(int ind_fase=indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {
      s_mva[ind_fase] = s_fase;
      }
   s_mva[indNEUTRO] = CZero();
   return(true);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TDemanda::GetValorTotalBateria(VTBateria *bateria, int np)
   {
   //variáveis locais
   strDEM str_dem;

   //determina demanda trifásica
	if (! calcula->GetValorBateria(bateria, np, &str_dem)) return(CZero());
   return(complex<double>(str_dem.p, str_dem.q));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TDemanda::GetValorTotalCapacitor(VTCapacitor *capacitor, int np)
   {
   //variáveis locais
   strDEM str_dem;

   //determina demanda trifásica
	if (! calcula->GetValorCapacitor(capacitor, np, &str_dem)) return(CZero());
   return(complex<double>(str_dem.p, str_dem.q));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TDemanda::GetValorTotalCarga(VTCarga *carga, int np)
   {
   //variáveis locais
   strDEM str_dem;

   //determina demanda
   if (! calcula->GetValorCarga(carga, np, &str_dem)) return(CZero());
   return(complex<double>(str_dem.p, str_dem.q));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TDemanda::GetValorTotalCargaReducao(VTCarga *carga, int np)
   {
   //variáveis locais
   strDEM          str_dem[MAX_FASE];
   complex<double> s_mva = CZero();

   //determina demanda
   if (! calcula->GetValorCargaReducao(carga, np, str_dem)) return(CZero());
   //soma as demandas das 4 fases
   for (int nf = indFASE_A; nf <= indNEUTRO; nf++)
      {
      s_mva += complex<double>(str_dem[nf].p, str_dem[nf].q);
      }
   return(s_mva);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TDemanda::GetValorTotalReator(VTReator *reator, int np)
   {
   //variáveis locais
   strDEM str_dem;

   //determina demanda
   if (! calcula->GetValorReator(reator, np, &str_dem)) return(CZero());
   return(complex<double>(str_dem.p, str_dem.q));
   }

//---------------------------------------------------------------------------
TDateTime __fastcall TDemanda::PM_GetDataReferencia(void)
   {
   return(PD.data_referencia);
   }

//---------------------------------------------------------------------------
int __fastcall TDemanda::PM_GetDiasMes(void)
	{
	return(DaysInMonth(DataReferencia));
	}

//---------------------------------------------------------------------------
double __fastcall TDemanda::PM_GetEdia_mwh(VTEqpto *eqpto)
   {
   //variáveis locais
   double energia = 0.;
   int    num_pat = patamares->NumPatamar();

   //loop p/ todos patamares
   for (int np = 0; np < num_pat; np++)
      {
      energia += E_mwh[eqpto][np];
      }
   return(energia);
   }

//---------------------------------------------------------------------------
double __fastcall TDemanda::PM_GetEmes_mwh(VTEqpto *eqpto)
   {
	return(PD.dias_mes * Edia_mwh[eqpto]);
   }

//---------------------------------------------------------------------------
double __fastcall TDemanda::PM_GetE_mwh(VTEqpto *eqpto, int np)
   {
   return(P_mw[eqpto][np] * patamares->Patamar[np]->Duracao);
   }

//---------------------------------------------------------------------------
double __fastcall TDemanda::PM_GetP_mw(VTEqpto *eqpto, int np)
   {
   return(S_mva[eqpto][np].real());
   }

//---------------------------------------------------------------------------
double __fastcall TDemanda::PM_GetQ_mvar(VTEqpto *eqpto, int np)
   {
   return(S_mva[eqpto][np].imag());
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TDemanda::PM_GetS_mva(VTEqpto *eqpto, int np)
   {
   //variáveis locais
   VTCarga *carga;

   switch(eqpto->Tipo())
      {
	  case eqptoBATERIA:	 return(GetValorTotalBateria((VTBateria*)eqpto, np));
	  case eqptoCAPACITOR:   return(GetValorTotalCapacitor((VTCapacitor*)eqpto,  np));
      case eqptoREATOR:      return(GetValorTotalReator((VTReator*)eqpto,  np));
      case eqptoCARGA:
         carga = (VTCarga*)eqpto;
         if (carga->DefinidaPorFase) return(GetValorTotalCargaReducao(carga, np));
		 else                        return(GetValorTotalCarga(carga, np));
	  default: break;
      }
	return(CZero());
	}

//---------------------------------------------------------------------------
void __fastcall TDemanda::PM_SetDataReferencia(TDateTime data)
   {
   //salva data de referência
   PD.data_referencia = data;
   //determina número de dias do mês
   PD.dias_mes = DaysInMonth(data);
   //atualiza VTCalculaDem
   calcula->DataReferencia = data;
   }

//---------------------------------------------------------------------------
void __fastcall TDemanda::PM_SetP_mw(VTEqpto   *eqpto, int np, double p)
   {      //hk080918
   //variáveis locais
   VTCarga *carga;

   switch(eqpto->Tipo())
      {
      case eqptoCAPACITOR:
         SetUmValorCapacitor((VTCapacitor*)eqpto, np, indP, p);
         break;
      case eqptoREATOR:
         SetUmValorReator((VTReator*)eqpto, np, indP, p);
         break;
      case eqptoCARGA:
         carga = (VTCarga*)eqpto;
         if (carga->DefinidaPorFase) SetUmValorCargaReducao(carga, np, indP, p);
         else                        SetUmValorCarga(carga, np, indP, p);
         break;
      case eqptoNO:
         //não se aplica
         break;
      default: break;
      }

   }
//---------------------------------------------------------------------------
void __fastcall TDemanda::PM_SetQ_mvar(VTEqpto *eqpto, int np, double q)
   {
   //variáveis locais
   VTCapacitor *capacitor;
   VTCarga     *carga;
   VTReator    *reator;

   switch(eqpto->Tipo())
      {
      case eqptoCAPACITOR:
         SetUmValorCapacitor((VTCapacitor*)eqpto, np, indQ, q);
         break;
      case eqptoREATOR:
         SetUmValorReator((VTReator*)eqpto, np, indQ, q);
         break;
      case eqptoCARGA:
         carga = (VTCarga*)eqpto;
         if (carga->DefinidaPorFase) SetUmValorCargaReducao(carga, np, indQ, q);
         else                        SetUmValorCarga(carga, np, indQ, q);
         break;
      case eqptoNO:
         //não se aplica
         break;
      default: break;
      }

   }

//---------------------------------------------------------------------------
bool __fastcall TDemanda::PM_SetS_mva(VTEqpto *eqpto, int np, complex<double> s_mva)
   {
   //variáveis locais
   VTCarga *carga;

   switch(eqpto->Tipo())
      {
      case eqptoCAPACITOR:   return(SetValorTotalCapacitor((VTCapacitor*)eqpto,  np, s_mva));
      case eqptoREATOR:      return(SetValorTotalReator((VTReator*)eqpto,  np, s_mva));
      case eqptoCARGA:
         carga = (VTCarga*)eqpto;
         if (carga->DefinidaPorFase) return(SetValorTotalCargaReducao(carga, np, s_mva));
         else                        return(SetValorTotalCarga(carga, np, s_mva));
      default:
         return(false);
      }
	}

//---------------------------------------------------------------------------
void __fastcall TDemanda::ReiniciaCurva(VTEqpto *eqpto)
   {
   //variáveis locais
   VTCarga *carga;

   //verifica tipo de Eqpto
   switch(eqpto->Tipo())
      {
      case eqptoCAPACITOR:
         ReiniciaCurvaCapacitor((VTCapacitor*)eqpto);
         break;
      case eqptoREATOR:
         ReiniciaCurvaReator((VTReator*)eqpto);
         break;
      case eqptoCARGA:
         carga = (VTCarga*)eqpto;
         if (carga->DefinidaPorFase) ReiniciaCurvaCargaReducao(carga);
         else                        ReiniciaCurvaCarga(carga);
         break;
      default: break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDemanda::ReiniciaCurvaCapacitor(VTCapacitor *capacitor)
   {
   //variáveis locais
   strDEM str_dem = {0., 0.};
   int    max_pat = patamares->NumPatamar();

   //atualiza todos patamares da Curva
   for (int np = 0; np < max_pat; np++)
      {
      calcula->SetValorCapacitor(capacitor, np, &str_dem);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDemanda::ReiniciaCurvaCarga(VTCarga *carga)
   {
   //variáveis locais
   strDEM str_dem = {0., 0.};
   int    max_pat = patamares->NumPatamar();

   //atualiza todos patamares da Curva
   for (int np = 0; np < max_pat; np++)
      {
      calcula->SetValorCarga(carga, np, &str_dem);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDemanda::ReiniciaCurvaCargaReducao(VTCarga *carga)
   {
   //variáveis locais
   strDEM str_dem[MAX_FASE];
   int    max_pat = patamares->NumPatamar();

   //zera demanda em todas as fases
   for (int nf = 0; nf < MAX_FASE; nf++)
      {
      str_dem[nf].p = str_dem[nf].q = 0.;
      }
   //atualiza todos patamares da Curva
   for (int np = 0; np < max_pat; np++)
      {
      calcula->SetValorCargaReducao(carga, np, str_dem);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDemanda::ReiniciaCurvaReator(VTReator *reator)
   {
   //variáveis locais
   strDEM str_dem = {0., 0.};
   int    max_pat = patamares->NumPatamar();

   //atualiza todos patamares da Curva
   for (int np = 0; np < max_pat; np++)
      {
      calcula->SetValorReator(reator, np, &str_dem);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TDemanda::Set_S3f_mva(VTEqpto *eqpto, int np, complex<double> s_mva[MAX_FASE])
   {
   //variáveis locais
   VTCarga *carga;

   switch(eqpto->Tipo())
      {
      case eqptoCAPACITOR:   return(SetValorPorFaseCapacitor((VTCapacitor *) eqpto, np, s_mva));
      case eqptoREATOR:      return(SetValorPorFaseReator((VTReator *) eqpto, np, s_mva));
      case eqptoCARGA:
         carga = (VTCarga *) eqpto;
         if (carga->DefinidaPorFase) return(SetValorPorFaseCargaReducao(carga, np, s_mva));
         else                        return(SetValorPorFaseCarga(carga, np, s_mva));
      default: break;
      }
	return(false);
	}

//---------------------------------------------------------------------------
bool __fastcall TDemanda::SetUmValorCapacitor(VTCapacitor *capacitor, int np, int ind_valor, double &valor)
   {
   //variáveis locais
   strDEM str_dem = {0., 0.};

	//obtém valores atuais de PQ da Carga
	if (! calcula->GetValorCapacitor(capacitor, np, &str_dem)) return(false);
	//atualiza somente o valor indicado
	switch(ind_valor)
		{
		case indP: str_dem.p = valor; break;
		case indQ: str_dem.q = valor; break;
		//case indV: str_sup.v = valor; break;
		//case indT: str_sup.t = valor; break;
		default:   return(false);
		}
	//atualiza valores do Suprimento
   if (! calcula->SetValorCapacitor(capacitor, np, &str_dem)) return(false);
   return(true);
   }

 //---------------------------------------------------------------------------
bool __fastcall TDemanda::SetUmValorCarga(VTCarga *carga, int np, int ind_valor, double &valor)
   {
   //variáveis locais
   strDEM str_dem = {0., 0.};

	//obtém valores atuais de PQ da Carga
	if (! calcula->GetValorCarga(carga, np, &str_dem)) return(false);
	//atualiza somente o valor indicado
	switch(ind_valor)
		{
		case indP: str_dem.p = valor; break;
		case indQ: str_dem.q = valor; break;
		//case indV: str_sup.v = valor; break;
		//case indT: str_sup.t = valor; break;
		default:   return(false);
		}
	//atualiza valores do Suprimento
   if (! calcula->SetValorCarga(carga, np, &str_dem)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TDemanda::SetUmValorCargaReducao(VTCarga *carga, int np, int ind_valor, double &valor)
	{
	//variáveis locais
   int    vet_fase[MAX_FASE] = {faseA, faseB, faseC, faseN};
   int    num_fase;
	double valor_fase;
	strDEM str_dem[MAX_FASE];

   //determina número de fases da Carga
   if ((num_fase = fases->NumeroDeFases(carga->Fases)) == 0) return(false);
	//determina demanda por fase da Carga de redução
	if (! calcula->GetValorCargaReducao(carga, np, str_dem)) return(false);
	//distribui o valor igualmente nas fases existentes e zera valor do neutro
	for (int nf = 0; nf < MAX_FASE; nf++)
		{//zera o neutro
      if      (nf == indNEUTRO)             valor_fase = 0.;
      else if (carga->Fases & vet_fase[nf]) valor_fase = valor / num_fase;
      else                                  valor_fase = 0.;
      //atualiza o valor indicado
		switch(ind_valor)
			{
			case indP: str_dem[nf].p   = valor_fase; break;
			case indQ: str_dem[nf].q   = valor_fase; break;
			//case indV: str_sup[nf].v = valor; break;
			//case indT: str_sup[nf].t = valor; break;
			default:   return(false);
			}
		}
	//atualiza demanda da Carga
	return(calcula->SetValorCargaReducao(carga, np, str_dem));
	}

//---------------------------------------------------------------------------
bool __fastcall TDemanda::SetUmValorReator(VTReator *reator, int np, int ind_valor, double &valor)
   {
   //variáveis locais
   strDEM str_dem;

	//obtém valores atuais de PQ da Carga
	if (! calcula->GetValorReator(reator, np, &str_dem)) return(false);
	//atualiza somente o valor indicado
	switch(ind_valor)
		{
		case indP: str_dem.p = valor; break;
		case indQ: str_dem.q = valor; break;
		//case indV: str_sup.v = valor; break;
		//case indT: str_sup.t = valor; break;
		default:   return(false);
		}
	//atualiza valores do Suprimento
   if (! calcula->SetValorReator(reator, np, &str_dem)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TDemanda::SetValorPorFaseCapacitor(VTCapacitor *capacitor, int np, complex<double> s_mva[MAX_FASE])
   {
   //variáveis locais
   complex<double> csoma;
   strDEM          str_dem;

   // Proteção: verifica se a curva é típica
   if (capacitor->curva->Tipica) return(false);

   // Define str_dem
   csoma = CZero();
   for(int ind_fase=indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {
      csoma += s_mva[ind_fase];
      }
   str_dem.p = csoma.real();
   str_dem.q = csoma.imag();

   // Atualiza demanda do Capacitor
   return(calcula->SetValorCapacitor(capacitor, np, &str_dem));
   }

//---------------------------------------------------------------------------
bool __fastcall TDemanda::SetValorPorFaseCarga(VTCarga *carga, int np, complex<double> s_mva[MAX_FASE])
   {
   //variáveis locais
   complex<double> csoma;
   strDEM          str_dem;

   //proteção: verifica se Curva é típica
   if (carga->curva->Tipica) return(false);

   // Define str_dem
   csoma = CZero();
   for(int ind_fase=indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {
      csoma += s_mva[ind_fase];
      }
   str_dem.p = csoma.real();
   str_dem.q = csoma.imag();

   //atualiza demanda da carga
   return(calcula->SetValorCarga(carga, np, &str_dem));
   }

//---------------------------------------------------------------------------
bool __fastcall TDemanda::SetValorPorFaseCargaReducao(VTCarga *carga, int np, complex<double> s_mva[MAX_FASE])
	{
	//variáveis locais
	strDEM str_dem[MAX_FASE];

   // Define str_dem
	for (int nf = 0; nf < MAX_FASE-1; nf++)
		{
		str_dem[nf].p = s_mva[nf].real();
		str_dem[nf].q = s_mva[nf].imag();
		}
   str_dem[indNEUTRO].p = 0.;
   str_dem[indNEUTRO].q = 0.;
	//atualiza demanda da Carga
	return(calcula->SetValorCargaReducao(carga, np, str_dem));
	}

//---------------------------------------------------------------------------
bool __fastcall TDemanda::SetValorPorFaseReator(VTReator *reator, int np, complex<double> s_mva[MAX_FASE])
   {
   //variáveis locais
   complex<double> csoma;
   strDEM          str_dem;

   // Proteção: verifica se a curva é típica
   if (reator->curva->Tipica) return(false);

   // Define str_dem
   csoma = CZero();
   for(int ind_fase=indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {
      csoma += s_mva[ind_fase];
      }
   str_dem.p = csoma.real();
   str_dem.q = csoma.imag();

   // Atualiza demanda do Reator
   return(calcula->SetValorReator(reator, np, &str_dem));
   }

//---------------------------------------------------------------------------
bool __fastcall TDemanda::SetValorTotalCapacitor(VTCapacitor *capacitor, int np, complex<double> s_mva)
   {
   //variáveis locais
   strDEM str_dem;

   //proteção: verifica se Curva é típica
   if (capacitor->curva->Tipica) return(false);
   //inicia strDEM
   str_dem.p = s_mva.real();
   str_dem.q = s_mva.imag();
   //atualiza demanda do Reator
   return(calcula->SetValorCapacitor(capacitor, np, &str_dem));
   }

//---------------------------------------------------------------------------
bool __fastcall TDemanda::SetValorTotalCarga(VTCarga *carga, int np, complex<double> s_mva)
   {
   //variáveis locais
   strDEM str_dem;

   //proteção: verifica se Curva é típica
   if (carga->curva->Tipica) return(false);
   //inicia strDEM
   str_dem.p = s_mva.real();
   str_dem.q = s_mva.imag();
   //atualiza demanda do Reator
   return(calcula->SetValorCarga(carga, np, &str_dem));
   }

//---------------------------------------------------------------------------
bool __fastcall TDemanda::SetValorTotalCargaReducao(VTCarga *carga, int np, complex<double> s_mva)
	{
	//variáveis locais
	double p_mw, q_mvar;
	strDEM str_dem[MAX_FASE];

	//divide demanda entre as 3 fases e zera demanda do neutro
   p_mw   = s_mva.real() / 3.;
   q_mvar = s_mva.imag() / 3.;
	for (int nf = 0; nf < MAX_FASE-1; nf++)
		{
		str_dem[nf].p = p_mw;
		str_dem[nf].q = q_mvar;
		}
   str_dem[indNEUTRO].p = 0;
   str_dem[indNEUTRO].q = 0;
	//atualiza demanda da Carga
	return(calcula->SetValorCargaReducao(carga, np, str_dem));
	}

//---------------------------------------------------------------------------
bool __fastcall TDemanda::SetValorTotalReator(VTReator *reator, int np, complex<double> s_mva)
   {
   //variáveis locais
   strDEM str_dem;

   //proteção: verifica se Curva é típica
   if (reator->curva->Tipica) return(false);
   //inicia strDEM
   str_dem.p = s_mva.real();
   str_dem.q = s_mva.imag();
   //atualiza demanda do Reator
   return(calcula->SetValorReator(reator, np, &str_dem));
   }

//---------------------------------------------------------------------------
//eof
