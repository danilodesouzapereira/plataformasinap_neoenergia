// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TMedicao.h"
#include "VTMedicoes.h"
#include "..\Constante\Fases.h"
#include "..\Curvas\VTCurva.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTMedicao* __fastcall NewObjMedicao(void)
{
	try
	{
		return (new TMedicao());
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TMedicao::TMedicao(void)
{
	// inicia dados
	diagnostico = diagMED_OK;
	enabled = false;
	ind_pat_dem_max = 0;
	ind_bar_ref = 0;
	erro = 0.;
	// cria objetos
	lisCAPACITOR = new TList();
	lisCAR_AJUS = new TList();
	lisCAR_FIXA = new TList();
	lisGER_SUP = new TList();
	chave_med = NULL;
	curva_pq = NULL;
}

// ---------------------------------------------------------------------------
__fastcall TMedicao::~TMedicao(void)
{
	// destrói objetos
	if (curva_pq)
	{
		delete curva_pq;
		curva_pq = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisCAPACITOR)
	{
		delete lisCAPACITOR;
		lisCAPACITOR = NULL;
	}
	if (lisCAR_AJUS)
	{
		delete lisCAR_AJUS;
		lisCAR_AJUS = NULL;
	}
	if (lisCAR_FIXA)
	{
		delete lisCAR_FIXA;
		lisCAR_FIXA = NULL;
	}
	if (lisGER_SUP)
	{
		delete lisGER_SUP;
		lisGER_SUP = NULL;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TMedicao::CurvaNula(int tipo_medicao)
{ /*
	 //variáveis locais
	 double valor[MAX_FASE+MAX_FASE];
	 strHM  hm_ini, hm_fim;
	 int    num_pto   = curva_orig->NumeroPontos();
	 int    num_valor = curva_orig->NumeroValores;

	 //proteção
	 if (num_valor > (sizeof(valor)/sizeof(double))) return(true);
	 //loop p/ todos os pontos da Curva
	 for (int index = 0; index < num_pto; index++)
	 {//obtém ponto da curva
	 if (! curva_orig->GetPonto(index, hm_ini, hm_fim, valor, num_valor)) return(true);
	 //verifica o tipo de medicao
	 switch(tipo_medicao)
	 {
	 case medicaoIMAXxFASE:
	 break;
	 case medicaoCURVA_Ix1FASE: //{Ia ou Ib ou Ic}
	 if ((IsDoubleZero(valor[0])) && (IsDoubleZero(valor[1])) && (IsDoubleZero(valor[2]))) return(true);
	 break;
	 case medicaoCURVA_Ix3FASE: //{Ia, Ib, Ic}
	 if (IsDoubleZero(valor[0])) return(true);
	 if (IsDoubleZero(valor[1])) return(true);
	 if (IsDoubleZero(valor[2])) return(true);
	 break;
	 case medicaoCURVA_P:    //{P}
	 if (IsDoubleZero(valor[0])) return(true);
	 break;
	 case medicaoCURVA_PQxFASE: //{Pa,Qa, Pb,Qc, Pc,Qc}
	 if (IsDoubleZero(valor[0])) return(true);
	 if (IsDoubleZero(valor[2])) return(true);
	 if (IsDoubleZero(valor[4])) return(true);
	 break;
	 case medicaoCURVA_PQ:    //{P, Q}
	 if (IsDoubleZero(valor[0])) return(true);
	 break;
	 case medicaoENERGIA_MWH:   //{E}
	 if (IsDoubleZero(valor[0])) return(true);
	 break;
	 default:
	 return(true);
	 }
	 } */
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TMedicao::CurvaNulaNegativa(int tipo_medicao)
{
	// variáveis locais
	double valor[MAX_FASE + MAX_FASE];
	strHM hm_ini, hm_fim;
	int num_pto = curva_pq->NumeroPontos();
	int num_valor = curva_pq->NumeroValores;
	int nb = 0;

	// proteção
	if (num_valor > (sizeof(valor) / sizeof(double)))
		return (true);
	// loop p/ todos os pontos da Curva
	for (int index = 0; index < num_pto; index++)
	{ // obtém ponto da curva  {Pa,Qa, Pb,Qc, Pc,Qc}
		if (!curva_pq->GetPonto(index, hm_ini, hm_fim, valor, num_valor))
			return (true);
		// verifica se os valores de P são negativos, considerando as fases da rede
		if (!IsDoubleZero(chave_med->resflow->Pa_mw[nb][index]) && (valor[0] <= 0))
			return (true);
		if (!IsDoubleZero(chave_med->resflow->Pb_mw[nb][index]) && (valor[2] <= 0))
			return (true);
		if (!IsDoubleZero(chave_med->resflow->Pc_mw[nb][index]) && (valor[4] <= 0))
			return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TMedicao::DiagnosticoAsStr(void)
{
	switch (diagnostico)
	{
	case diagMED_OK:
		return "OK";
	case diagMED_ERRO_INICIAL:
		return "Diferença inicial maior do que o limite definido";
	case diagMED_ERRO_FINAL:
		return "O erro final é maior que o erro máximo permitido";
	case diagMED_ERRO_FLUXO:
		return "Fluxo não calculado para a rede";
	case diagTEMPO:
		return "Tempo excedido para o cálculo";
	default:
		return "";
	}
}

// ---------------------------------------------------------------------------
int __fastcall TMedicao::FasesMed(int ind_pat)
{
	// variáveis locais
	double valor[MAX_FASE + MAX_FASE];
	strHM hm_ini, hm_fim;
	int num_pto, num_valor;
	int fase_tag = faseINV;
	int nb = 0;

	// proteção
	if (!curva_pq)
		return (faseINV);
	num_pto = curva_pq->NumeroPontos();
	num_valor = curva_pq->NumeroValores;
	if (num_valor > (sizeof(valor) / sizeof(double)))
		return (faseINV);
	// obtem ponto da curva  {Pa,Qa, Pb,Qc, Pc,Qc}
	if (!curva_pq->GetPonto(ind_pat, hm_ini, hm_fim, valor, num_valor))
		return (faseINV);
	// verifica se os valores de P são válidos, p/determinar as fases da medição
	if (!IsDoubleZero(valor[0]))
		fase_tag |= faseA;
	if (!IsDoubleZero(valor[2]))
		fase_tag |= faseB;
	if (!IsDoubleZero(valor[4]))
		fase_tag |= faseC;
	return (fase_tag);
}

// ---------------------------------------------------------------------------
int __fastcall TMedicao::FasesRede(int ind_pat)
{
	// variáveis locais
	int fase_tag = faseINV;
	int nb = 0;

	// proteção
	if (!chave_med)
		return (faseINV);
	if (!chave_med->resflow)
		return (faseINV);
	// verifica se os valores de P são válidos, p/determinar as fases da rede
	if (!IsDoubleZero(chave_med->resflow->Pa_mw[nb][ind_pat]))
		fase_tag |= faseA;
	if (!IsDoubleZero(chave_med->resflow->Pb_mw[nb][ind_pat]))
		fase_tag |= faseB;
	if (!IsDoubleZero(chave_med->resflow->Pc_mw[nb][ind_pat]))
		fase_tag |= faseC;
	return (fase_tag);
}

// ---------------------------------------------------------------------------
bool __fastcall TMedicao::FasesValidas(void)
{
	// variáveis locais
	int num_pto, fase_rede, fase_med;

	// proteção
	if (!curva_pq)
		return (faseINV);
	num_pto = curva_pq->NumeroPontos();
	// loop p/ todos os pontos da Curva
	for (int index = 0; index < num_pto; index++)
	{ // verifica as fases da medição e as fases do ponto onde está a medição
		fase_rede = FasesRede(index);
		fase_med = FasesMed(index);
		if (fase_rede == faseINV)
			return (false);
		if (fase_med == faseINV)
			return (false);
		if ((fase_med & fase_rede) != fase_rede)
			return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
double __fastcall TMedicao::MaiorErro(void)
{
	// variaveis locais
	double maior_erro = 0.;
	double valor[MAX_FASE + MAX_FASE], diferenca[MAX_FASE];
	strHM hm_ini, hm_fim;
	int num_pto = curva_pq->NumeroPontos();
	int num_valor = curva_pq->NumeroValores;
	int nb = 0;

	// loop p/ todos os pontos da Curva
	for (int index = 0; index < num_pto; index++)
	{ // obtém ponto da curva  {Pa,Qa, Pb,Qc, Pc,Qc}
		if (!curva_pq->GetPonto(index, hm_ini, hm_fim, valor, num_valor))
			continue;
		// calcula diferença em módulo entre fluxo e medição (somente P)
		diferenca[0] = fabs(chave_med->resflow->Pa_mw[nb][index] - valor[0]);
		diferenca[1] = fabs(chave_med->resflow->Pb_mw[nb][index] - valor[2]);
		diferenca[2] = fabs(chave_med->resflow->Pc_mw[nb][index] - valor[4]);
		// determina o maior
		maior_erro = Max(maior_erro, diferenca[0]);
		maior_erro = Max(maior_erro, diferenca[1]);
		maior_erro = Max(maior_erro, diferenca[2]);
	}
	return (maior_erro);
}

// ---------------------------------------------------------------------------
void __fastcall TMedicao::Reinicia(void)
{
	// reinicia chave e curva pq
	chave_med = NULL;
	if (curva_pq)
	{
		delete curva_pq;
		curva_pq = NULL;
	}
}

// ---------------------------------------------------------------------------
// eof
