// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Math.hpp>
#include "TCalculaDem.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Curvas\VTCurva.h"
#include "..\Mercado\VTCenario.h"
#include "..\Mercado\VTCrescimento.h"
#include "..\Mercado\VTMercado.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBateria.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTReator.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe
// ---------------------------------------------------------------------------
VTCalculaDem* __fastcall NewObjCalculaDem(VTApl *apl)
{
	try
	{
		return (new TCalculaDem(apl));
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// erro na criação do objeto
	return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TCalculaDem::TCalculaDem(VTApl *apl)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
	patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	// inicia data de referência
	DataReferencia = Now();
}

// ---------------------------------------------------------------------------
__fastcall TCalculaDem::~TCalculaDem(void)
{
	// nada a fazer
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall TCalculaDem::GetValorCapacitor(VTCapacitor *capacitor, int np, strDEM *str_dem)
 {
 //variáveis locais
 double    Sbase;
 strHM     hm;
 VTPatamar *patamar;

 //determina hora/minuto de cálculo do patamar
 patamar   = patamares->Patamar[np];
 hm.hora   = patamar->HoraCal;
 hm.minuto = patamar->MinutoCal;
 //verifica se Curva típica (do tipo On/Off)
 if (capacitor->curva->Tipica)
 {//curva típica do tipo On/Off (1/0)
 double valor[nvCURVA_LD];
 if (! capacitor->curva->GetValor(hm, valor, sizeof(valor)/sizeof(double))) return(false);
 str_dem->p = 0.;
 str_dem->q = (int(valor[0]) == 0) ? 0. : capacitor->q;
 }
 else
 {//curva própria c/ valor de demanda reativa
 double valor[nvCURVA_PQ];
 if (! capacitor->curva->GetValor(hm, valor, sizeof(valor)/sizeof(double))) return(false);
 switch(capacitor->curva->Unidade)
 {
 case unidKVA:
 Sbase = 0.001;
 break;
 case unidMVA:
 default:
 Sbase = 1.;
 }
 str_dem->p = 0.;
 str_dem->q = valor[1] * Sbase;
 }
 return(true);
 }
 */

// ---------------------------------------------------------------------------
bool __fastcall TCalculaDem::GetValorBateria(VTBateria *bateria, int np, strDEM *str_dem)
{
	// variáveis locais
	double valor[nvCURVA_PQ] =
	{0., 0.};
	double p, q, fatpot, Sbase;
	strHM hm;
	VTCurva *curva;
	VTPatamar *patamar;

	// determina Curva associada à Carga
	curva = bateria->curvaCargaDescarga;
	// determina Curva associada à Carga
	if ((curva = bateria->curvaCargaDescarga) == NULL)
		return (false);
	// determina hora/minuto de cálculo do patamar
	patamar = patamares->Patamar[np];
	hm.hora = patamar->HoraCal;
	hm.minuto = patamar->MinutoCal;
	// determina Sbase(MVA) em função da unidade da Curva
	switch (curva->Unidade)
	{
	case unidKVA:
		Sbase = 0.001;
		break;
	case unidMVA:
		Sbase = 1.;
		break;
	default:
		Sbase = 0;
	}
	// determina valores da Curva no patamar indicado
	if (!curva->GetValor(hm, valor, sizeof(valor) / sizeof(double)))
		return (false);
	// calcula demanda em função do tipo da Curva
	switch (curva->Tipo)
	{
	case curvaPQ:
	case curvaPQ_MDP:
		p = valor[0];
		q = valor[1];
		break;
	case curvaPFP:
	case curvaPFP_MDP:
		p = valor[0];
		fatpot = valor[1];
		q = p * Tan(ArcCos(fatpot));
		break;
	default:
		p = q = 0.;
	}

	// preenche strDEM a ser retornada
	str_dem->p = Sbase * p;
	str_dem->q = Sbase * q;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCalculaDem::GetValorCapacitor(VTCapacitor *capacitor, int np, strDEM *str_dem)
{
	// variáveis locais
	double Sbase;
	strHM hm;
	VTPatamar *patamar;

	// determina hora/minuto de cálculo do patamar
	patamar = patamares->Patamar[np];
	hm.hora = patamar->HoraCal;
	hm.minuto = patamar->MinutoCal;
	// verifica se Curva típica do tipo Ligada/Desligada
	// if (capacitor->curva->Tipo == curvaLD)
	//
	// ATENÇÃO: CÓDIO TEMPORÁRIO POR CAUSA DE ERRO NA DEFINIÇÃO DE CURVAS TÍPICAS
	// As curvas típicas estão definida como curvaPQ, sendo P=1 e Q=0
	// verifica se o Capacitor não está ligado
	if (!capacitor->Status[sttENABLED])
	{
		str_dem->p = 0.;
		str_dem->q = 0.;
	}
	else if ((capacitor->curva->Tipo == curvaLD) || (capacitor->curva->Tipica))
	{ // curva típica do tipo On/Off (1/0)
		double valor[nvCURVA_LD];
		if (!capacitor->curva->GetValor(hm, valor, sizeof(valor) / sizeof(double)))
			return (false);
		str_dem->p = 0.;
		str_dem->q = (int(valor[0]) == 0) ? 0. : capacitor->q;
	}
	else if (capacitor->curva->Tipo == curvaPQ)
	{ // curva própria c/ valor de demanda reativa
		double valor[nvCURVA_PQ];
		if (!capacitor->curva->GetValor(hm, valor, sizeof(valor) / sizeof(double)))
			return (false);
		switch (capacitor->curva->Unidade)
		{
		case unidKVA:
			Sbase = 0.001;
			break;
		case unidMVA:
		default:
			Sbase = 1.;
		}
		str_dem->p = 0.;
		str_dem->q = valor[1] * Sbase;
	}
	else
	{
		str_dem->p = 0.;
		str_dem->q = 0.;
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCalculaDem::GetValorCarga(VTCarga *carga, int np, strDEM *str_dem)
{
	// variáveis locais
	double valor[nvCURVA_CAR] =
	{0., 0.};
	double taxa_mercado = 1.;
	double horas_mes;
	double p, q, fatpot, Sbase;
	strHM hm;
	VTCurva *curva;
	VTPatamar *patamar;

	// define número de horas do mês
	horas_mes = PD.dias_mes * 24;
	// verifica se é IP com curva do tipo On/Off
	if (carga->IP)
	{ // detemina valor p/ Carga IP
		return (GetValorCargaIP(carga, np, str_dem));
	}
	/*
	 //determina Curva associada à Carga
	 curva = carga->curva;
	 if (curva->Unidade == unidPU_MAX)
	 {//determina Curva com valores em pu da média, associada à Curva da Carga
	 curva = curva->CurvaSiamesaPUmedia;
	 }
	 */
	// determina Curva associada à Carga
	if ((curva = carga->curva) == NULL)
		return (false);
	// verifica se a Curva possui Curva siamesa
	if (curva->CurvaSiamesaPUmedia)
	{ // determina Curva com valores em pu da média, associada à Curva da Carga
		curva = curva->CurvaSiamesaPUmedia;
	}
	// determina hora/minuto de cálculo do patamar
	patamar = patamares->Patamar[np];
	hm.hora = patamar->HoraCal;
	hm.minuto = patamar->MinutoCal;
	// determina Sbase(MVA) em função da unidade da Curva
	switch (curva->Unidade)
	{
	case unidKVA:
		Sbase = 0.001;
		break;
	case unidMVA:
		Sbase = 1.;
		break;
	case unidPU_MED: // Sbase é a demanda média da curva
		Sbase = (carga->Energia_mwhmes * curva->FatorCorrecaoEnergia) / horas_mes;
		break;
	case unidPU_MAX: // Sbase é a demanda máxima da curva
		// esse caso não pode ocorrer pois será usada a Curva siamesa em pu da média
		Sbase = 0;
		break;
	default:
		Sbase = 0;
	}
	// determina valores da Curva no patamar indicado
	if (!curva->GetValor(hm, valor, sizeof(valor) / sizeof(double)))
		return (false);
	// calcula demanda em função do tipo da Curva
	switch (curva->Tipo)
	{
	case curvaPQ:
	case curvaPQ_MDP:
		p = valor[0];
		q = valor[1];
		break;
	case curvaPFP:
	case curvaPFP_MDP:
		p = valor[0];
		fatpot = valor[1];
		q = p * Tan(ArcCos(fatpot));
		break;
	default:
		p = q = 0.;
	}
	/*
	 //determina taxa de crescimento da Carga p/ o ano definido em Mercado
	 if (carga->crescimento)
	 {
	 taxa_mercado = 1. + carga->crescimento->TaxaAcumulada_pu[mercado->IndiceAnoAtual];
	 }
	 */
	// determina taxa de crescimento da Carga p/ o ano definido em Mercado e pelo Cenario selecionado
	if (carga->crescimento)
	{
		taxa_mercado = carga->crescimento->TaxaAcumulada_pu[mercado->IndiceAnoAtual];
		// verifica se há um Cenario definido
		if (mercado->Cenario)
		{
			taxa_mercado = taxa_mercado * mercado->Cenario->Taxa;
		}
		// define taxa_mercado p/ multiplicar o valor demanda
		taxa_mercado += 1.;
	}
	// preenche strDEM a ser retornada
	str_dem->p = Sbase * p * taxa_mercado;
	str_dem->q = Sbase * q * taxa_mercado;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCalculaDem::GetValorCargaIP(VTCarga *carga, int np, strDEM *str_dem)
{
	// variáveis locais
	double p_mw, q_mvar;
	double valor[32];
	strHM hm;
	VTPatamar *patamar;

	// proteção: zera valores de retorno
	str_dem->p = str_dem->q = 0.;
	// determina pot.ativa e reativa da lâmpada (independente do patamar)
	p_mw = carga->IP_NumLamp * carga->IP_Pot_kw * 0.001; // kW => MW
	q_mvar = 0.;
	if (carga->IP_Fatpot != 0.)
	{
		q_mvar = p_mw * Tan(ArcCos(carga->IP_Fatpot));
	}
	// determina hora/minuto de cálculo do patamar
	patamar = patamares->Patamar[np];
	hm.hora = patamar->HoraCal;
	hm.minuto = patamar->MinutoCal;
	// determina valor da Curva no patamar indicado
	if (!carga->curva->GetValor(hm, valor, carga->curva->NumeroValores))
		return (false);
	// verificao tipo de curva
	if (carga->curva->Tipo == curvaLD)
	{ // verifcia se curva indicada carga desligada no patamar
		if (IsDoubleZero(valor[0]))
		{
			p_mw = q_mvar = 0.;
		}
	}
	else if (carga->curva->Tipo == curvaPFP_MDP)
	{ // corrige  pot.ativa
		p_mw *= valor[0];
		// recalcula pot.reativa, considerando fator de potência da curva e não o da Carga IP
		q_mvar = 0.;
		if (valor[1] != 0.)
		{
			q_mvar = p_mw * Tan(ArcCos(valor[1]));
		}
	}
	else if (carga->curva->Tipo == curvaPQ_MDP)
	{ // corrige  pot.ativa  e reativa
		p_mw *= valor[0];
		q_mvar *= valor[1];
	}
	// atuliza valores a serem retornados
	str_dem->p = p_mw;
	str_dem->q = q_mvar;

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCalculaDem::GetValorCargaReducao(VTCarga *carga, int np, strDEM *str_dem)
{
	// variáveis locais
	double valor[nvCURVA_RRCAR];
	double taxa_mercado = 1.;
	strHM hm;
	VTPatamar *patamar;

	// determina hora/minuto de cálculo do patamar
	patamar = patamares->Patamar[np];
	hm.hora = patamar->HoraCal;
	hm.minuto = patamar->MinutoCal;
	// determina valores da Curva no patamar indicado
	if (!carga->curva->GetValor(hm, valor, sizeof(valor) / sizeof(double)))
		return (false);
	// determina taxa de crescimento da Carga p/ o ano definido em Mercado
	if (carga->crescimento)
	{
		taxa_mercado = 1. + carga->crescimento->TaxaAcumulada_pu[mercado->IndiceAnoAtual];
	}
	for (int nf = indFASE_A; nf <= indNEUTRO; nf++)
	{
		str_dem[nf].p = valor[2 * nf + 0] * taxa_mercado;
		str_dem[nf].q = valor[2 * nf + 1] * taxa_mercado;
	}
	return (true);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TCalculaDem::GetValorReator(VTReator *reator, int np, strDEM *str_dem)
 {
 //variáveis locais
 double    Sbase;
 strHM     hm;
 VTPatamar *patamar;

 //determina hora/minuto de cálculo do patamar
 patamar   = patamares->Patamar[np];
 hm.hora   = patamar->HoraCal;
 hm.minuto = patamar->MinutoCal;
 //verifica se Curva típica (do tipo On/Off)
 if (reator->curva->Tipica)
 {//curva típica do tipo On/Off (1/0)
 double valor[nvCURVA_LD];
 if (! reator->curva->GetValor(hm, valor, sizeof(valor)/sizeof(double))) return(false);
 str_dem->p = 0.;
 str_dem->q = (int(valor[0]) == 0) ? 0. : reator->q;
 }
 else
 {//curva própria c/ valor de demanda reativa
 double valor[nvCURVA_PQ];
 if (! reator->curva->GetValor(hm, valor, sizeof(valor)/sizeof(double))) return(false);
 switch(reator->curva->Unidade)
 {
 case unidKVA:
 Sbase = 0.001;
 break;
 case unidMVA:
 default:
 Sbase = 1.;
 }
 str_dem->p = 0.;
 str_dem->q = valor[1] * Sbase;
 }
 return(true);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TCalculaDem::GetValorReator(VTReator *reator, int np, strDEM *str_dem)
{
	// variáveis locais
	double Sbase;
	strHM hm;
	VTPatamar *patamar;

	// determina hora/minuto de cálculo do patamar
	patamar = patamares->Patamar[np];
	hm.hora = patamar->HoraCal;
	hm.minuto = patamar->MinutoCal;
	// verifica se Curva típica do tipo Ligada/Desligada
	// if (reator->curva->Tipo == curvaLD)
	//
	// ATENÇÃO: CÓDIGO TEMPORÁRIO POR CAUSA DE ERRO NA DEFINIÇÃO DE CURVAS TÍPICAS
	// As curvas típicas estão definida como curvaPQ, sendo P=1 e Q=0
	if ((reator->curva->Tipo == curvaLD) || (reator->curva->Tipica))
	{ // curva típica do tipo On/Off (1/0)
		double valor[nvCURVA_LD];
		if (!reator->curva->GetValor(hm, valor, sizeof(valor) / sizeof(double)))
			return (false);
		str_dem->p = 0.;
		str_dem->q = (int(valor[0]) == 0) ? 0. : reator->q;
	}
	else if (reator->curva->Tipo == curvaPQ)
	{ // curva própria c/ valor de demanda reativa
		double valor[nvCURVA_PQ];
		if (!reator->curva->GetValor(hm, valor, sizeof(valor) / sizeof(double)))
			return (false);
		switch (reator->curva->Unidade)
		{
		case unidKVA:
			Sbase = 0.001;
			break;
		case unidMVA:
		default:
			Sbase = 1.;
		}
		str_dem->p = 0.;
		str_dem->q = valor[1] * Sbase;
	}
	else
	{
		str_dem->p = 0.;
		str_dem->q = 0.;
	}
	return (true);
}

// ---------------------------------------------------------------------------
TDateTime __fastcall TCalculaDem::PM_GetDataReferencia(void)
{
	return (PD.data_referencia);
}

// ---------------------------------------------------------------------------
void __fastcall TCalculaDem::PM_SetDataReferencia(TDateTime data)
{
	// salva data de referência
	PD.data_referencia = data;
	// determina número de dias do mês
	PD.dias_mes = DaysInMonth(data);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/index.cfm#/tasks/7056570
bool __fastcall TCalculaDem::SetValorCapacitor(VTCapacitor *capacitor, int np, strDEM *str_dem)
{
	// variáveis locais
	double Sbase;
	double valor[nvCURVA_PQ];
	strHM hm, hm_fim;
	VTPatamar *patamar, *patamar2;

	// proteção: verifica se é uma curva típica
	if (capacitor->curva->Tipica)
	{
		// Erro("TCalculaDem::SetValorCapacitor() - Não é permitido alterar curva típica de capacitor");
		return (false);
	}
	switch (capacitor->curva->Unidade)
	{
	case unidKVA:
		Sbase = 1000.;
		break;
	case unidMVA:
		Sbase = 1.;
		break;
	case unidPU_MED: // Sbase é a demanda média da curva
		Sbase = 1;
		break;
	case unidPU_MAX: // Sbase é a demanda máxima da curva
		Sbase = 1; ;
		break;
	default:
		Sbase = 0;
	}
	// determina hora/minuto de cálculo do patamar
	patamar = patamares->Patamar[np];
	hm.hora = patamar->HoraCal;
	hm.minuto = patamar->MinutoCal;
	// define valores da Curva no patamar indicado
	valor[0] = str_dem->p * Sbase;
	valor[1] = str_dem->q * Sbase;
	// tratamento especial para curva MMTN e LMP
	if ((patamares->Escala == patMMTN) || (patamares->Escala == patLMP))
	{ // verifica se é ultimo patamar
		if (patamares->NumPatamar() == (np + 1))
		{
			if ((patamar2 = patamares->Patamar[0]) == NULL)
				return (false);
		}
		else
		{
			if ((patamar2 = patamares->Patamar[np + 1]) == NULL)
				return (false);
		}
		// determina proximo patamar
		hm_fim.hora = patamar2->HoraCal;
		hm_fim.minuto = patamar2->MinutoCal;
		// atualiza valores do Ponto da Curva
		return (capacitor->curva->SetPonto(hm, hm_fim, valor, (sizeof(valor) / sizeof(double))));
	}
	return (capacitor->curva->SetPonto(hm, valor, sizeof(valor) / sizeof(double)));
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/index.cfm#/tasks/7056570
bool __fastcall TCalculaDem::SetValorCarga(VTCarga *carga, int np, strDEM *str_dem)
{
	// variáveis locais
	double Sbase, taxa_mercado = 1.;
	double valor[nvCURVA_CAR];
	strHM hm, hm_fim;
	VTPatamar *patamar, *patamar2;

	// proteção
	if (carga->curva->Tipica)
		return (false);
	// determina valor de base dos dados
	switch (carga->curva->Unidade)
	{
	case unidKVA:
		Sbase = 1000.;
		break;
	case unidMVA:
		Sbase = 1.;
		break;
	case unidPU_MED: // Sbase é a demanda média da curva
		Sbase = 1;
		break;
	case unidPU_MAX: // Sbase é a demanda máxima da curva
		Sbase = 1; ;
		break;
	default:
		Sbase = 0;
	}
	// determina taxa de crescimento da Carga p/ o ano definido em Mercado e pelo Cenario selecionado
	if (carga->crescimento)
	{
		taxa_mercado = carga->crescimento->TaxaAcumulada_pu[mercado->IndiceAnoAtual];
		// verifica se há um Cenario definido
		if (mercado->Cenario)
		{
			taxa_mercado = taxa_mercado * mercado->Cenario->Taxa;
		}
		// define taxa_mercado p/ dividir o valor da demanda
		taxa_mercado += 1.;
	}
	// determina hora/minuto de cálculo do patamar
	patamar = patamares->Patamar[np];
	hm.hora = patamar->HoraCal;
	hm.minuto = patamar->MinutoCal;
	// define valores da Curva no patamar indicado
	valor[0] = (str_dem->p * Sbase) / taxa_mercado;
	valor[1] = (str_dem->q * Sbase) / taxa_mercado;
	// tratamento especial para curva MMTN e LMP
	if ((patamares->Escala == patMMTN) || (patamares->Escala == patLMP))
	{ // verifica se é ultimo patamar
		if (patamares->NumPatamar() == (np + 1))
		{
			if ((patamar2 = patamares->Patamar[0]) == NULL)
				return (false);
		}
		else
		{
			if ((patamar2 = patamares->Patamar[np + 1]) == NULL)
				return (false);
		}
		// determina proximo patamar
		hm_fim.hora = patamar2->HoraCal;
		hm_fim.minuto = patamar2->MinutoCal;
		// atualiza valores do Ponto da Curva
		return (carga->curva->SetPonto(hm, hm_fim, valor, (sizeof(valor) / sizeof(double))));
	}
	return (carga->curva->SetPonto(hm, valor, sizeof(valor) / sizeof(double)));
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/index.cfm#/tasks/7056570
bool __fastcall TCalculaDem::SetValorCargaReducao(VTCarga *carga, int np, strDEM *str_dem)
{
	// variáveis locais
	double taxa_mercado = 1.;
	double valor[nvCURVA_RRCAR];
	strHM hm, hm_fim;
	VTPatamar *patamar, *patamar2;

	// determina taxa de crescimento da Carga p/ o ano definido em Mercado e pelo Cenario selecionado
	if (carga->crescimento)
	{
		taxa_mercado = carga->crescimento->TaxaAcumulada_pu[mercado->IndiceAnoAtual];
		// verifica se há um Cenario definido
		if (mercado->Cenario)
		{
			taxa_mercado = taxa_mercado * mercado->Cenario->Taxa;
		}
		// define taxa_mercado p/ dividir o valor da demanda
		taxa_mercado += 1.;
	}
	// determina hora/minuto de cálculo do patamar
	patamar = patamares->Patamar[np];
	hm.hora = patamar->HoraCal;
	hm.minuto = patamar->MinutoCal;
	// define valores da Curva no patamar indicado
	for (int nf = 0; nf < MAX_FASE; nf++)
	{
		valor[2 * nf + 0] = str_dem[nf].p / taxa_mercado;
		valor[2 * nf + 1] = str_dem[nf].q / taxa_mercado;
	}
	// tratamento especial para curva MMTN e LMP
	if ((patamares->Escala == patMMTN) || (patamares->Escala == patLMP))
	{ // verifica se é ultimo patamar
		if (patamares->NumPatamar() == (np + 1))
		{
			if ((patamar2 = patamares->Patamar[0]) == NULL)
				return (false);
		}
		else
		{
			if ((patamar2 = patamares->Patamar[np + 1]) == NULL)
				return (false);
		}
		// determina proximo patamar
		hm_fim.hora = patamar2->HoraCal;
		hm_fim.minuto = patamar2->MinutoCal;
		// atualiza valores do Ponto da Curva
		return (carga->curva->SetPonto(hm, hm_fim, valor, (sizeof(valor) / sizeof(double))));
	}
	return (carga->curva->SetPonto(hm, valor, sizeof(valor) / sizeof(double)));
}

// ---------------------------------------------------------------------------
bool __fastcall TCalculaDem::SetValorReator(VTReator *reator, int np, strDEM *str_dem)
{
	Erro("TCalculaDem::SetValorReator() - Não é permitido alterar curva típica de reator");
	return (false);
}

// ---------------------------------------------------------------------------
// eof
