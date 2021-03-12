// ---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TResumoTec.h"
#include "..\Constante\Const.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTResumoTec* __fastcall NewObjResumoTec(int ano_ini, int ano_fim)
{
	try
	{
		return (new TResumoTec(ano_ini, ano_fim));
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TResumoTec::TResumoTec(int ano_ini, int ano_fim)
{
	// variáveis locais
	int num_ano = ano_fim - ano_ini + 1;

	// cria lista de resumos por alimentador
	lisRESUMO_REDE = new TList();
	// salva valores de ano inicial e final
	PD.ano_ini = ano_ini;
	PD.ano_fim = ano_fim;
	// acerta dimensão DynamicArray
	PD.energia.Length = num_ano;
	// zera todos os dados
	for (int ind_ano = 0; ind_ano < num_ano; ind_ano++)
	{
		PD.energia[ind_ano].calculo_ok = false;
		PD.energia[ind_ano].compensacao = 0;
		PD.energia[ind_ano].diag_bar_ruim = 0;
		PD.energia[ind_ano].diag_bar_reg = 0;
		PD.energia[ind_ano].diag_bar_bom = 0;
		PD.energia[ind_ano].diag_car_ruim = 0;
		PD.energia[ind_ano].diag_car_reg = 0;
		PD.energia[ind_ano].diag_car_bom = 0;
		PD.energia[ind_ano].diag_tra_ruim = 0;
		PD.energia[ind_ano].diag_tra_reg = 0;
		PD.energia[ind_ano].diag_tra_bom = 0;
		PD.energia[ind_ano].diag_tre_ruim_km = 0;
		PD.energia[ind_ano].diag_tre_reg_km = 0;
		PD.energia[ind_ano].diag_tre_bom_km = 0;
		PD.energia[ind_ano].dec = 0;
		PD.energia[ind_ano].end = 0;
		PD.energia[ind_ano].fec = 0;
		PD.energia[ind_ano].entrada_mwhm = 0;
		PD.energia[ind_ano].entregue_mwhm = 0;
		PD.energia[ind_ano].gerada_mwhm = 0;
		PD.energia[ind_ano].perda_mwhm = 0;
		PD.energia[ind_ano].perc_bar_ruim = 0;
		PD.energia[ind_ano].perc_bar_reg = 0;
		PD.energia[ind_ano].perc_bar_bom = 0;
		PD.energia[ind_ano].perc_car_ruim = 0;
		PD.energia[ind_ano].perc_car_reg = 0;
		PD.energia[ind_ano].perc_car_bom = 0;
		PD.energia[ind_ano].perc_tra_ruim = 0;
		PD.energia[ind_ano].perc_tra_reg = 0;
		PD.energia[ind_ano].perc_tra_bom = 0;
	}
}

// ---------------------------------------------------------------------------
__fastcall TResumoTec::~TResumoTec(void)
{
	// destroi lista e seus objetos
	if (lisRESUMO_REDE)
	{
		LimpaTList(lisRESUMO_REDE);
		delete lisRESUMO_REDE;
		lisRESUMO_REDE = NULL;
	}
	// libera memória do DynamicArray usado p/ taxa acumulada
	PD.energia.Length = 0;
}

// ---------------------------------------------------------------------------
VTResumoTec* __fastcall TResumoTec::ExisteResumoRede(AnsiString alimentador, AnsiString subestacao)
{
	// variáveis locais
	VTResumoTec* resumo;

	for (int n = 0; n < lisRESUMO_REDE->Count; n++)
	{
		resumo = (VTResumoTec*)lisRESUMO_REDE->Items[n];
		if ((resumo->Alimentador.AnsiCompareIC(alimentador) == 0) && (resumo->Subestacao.AnsiCompareIC(subestacao) == 0))
			return resumo;
	}
	return NULL;
}

// ---------------------------------------------------------------------------
int __fastcall TResumoTec::IndiceAno(int ano)
{
	// variáveis locais
	int ind_ano;

	// proteção
	if ((ano >= PD.ano_ini) && (ano <= PD.ano_fim))
	{
		ind_ano = ano - PD.ano_ini;
	}
	else
	{
		ind_ano = 0;
	}
	// retorna índice do ano
	return (ind_ano);
}

// ---------------------------------------------------------------------------
TList* __fastcall TResumoTec::LisResumoTecRede(void)
{
	return lisRESUMO_REDE;
}

// ---------------------------------------------------------------------------
int __fastcall TResumoTec::PM_GetAnoInicial(void)
{
	return (PD.ano_ini);
}

// ---------------------------------------------------------------------------
int __fastcall TResumoTec::PM_GetAnoFinal(void)
{
	return (PD.ano_fim);
}

// ---------------------------------------------------------------------------
bool __fastcall TResumoTec::PM_GetCalculo_OK(int ano)
{
	return ((PD.energia[IndiceAno(ano)]).calculo_ok);
}

// ---------------------------------------------------------------------------
double __fastcall TResumoTec::PM_GetCompensacao_RS(int ano)
{
	return ((PD.energia[IndiceAno(ano)]).compensacao);
}

// ---------------------------------------------------------------------------
int __fastcall TResumoTec::PM_GetDiagBarra(int ano, int idiag)
{
	int neq = 0;
	if (idiag == diagRUIM)
		neq = (PD.energia[IndiceAno(ano)]).diag_bar_ruim;
	else if (idiag == diagREGULAR)
		neq = (PD.energia[IndiceAno(ano)]).diag_bar_reg;
	else if (idiag == diagBOM)
		neq = (PD.energia[IndiceAno(ano)]).diag_bar_bom;

	return (neq);
}

// ---------------------------------------------------------------------------
int __fastcall TResumoTec::PM_GetDiagCarga(int ano, int idiag)
{
	int neq = 0;
	if (idiag == diagRUIM)
		neq = (PD.energia[IndiceAno(ano)]).diag_car_ruim;
	else if (idiag == diagREGULAR)
		neq = (PD.energia[IndiceAno(ano)]).diag_car_reg;
	else if (idiag == diagBOM)
		neq = (PD.energia[IndiceAno(ano)]).diag_car_bom;

	return (neq);
}

// ---------------------------------------------------------------------------
int __fastcall TResumoTec::PM_GetDiagTrafo(int ano, int idiag)
{
	int neq = 0;
	if (idiag == diagRUIM)
		neq = (PD.energia[IndiceAno(ano)]).diag_tra_ruim;
	else if (idiag == diagREGULAR)
		neq = (PD.energia[IndiceAno(ano)]).diag_tra_reg;
	else if (idiag == diagBOM)
		neq = (PD.energia[IndiceAno(ano)]).diag_tra_bom;

	return (neq);
}

// ---------------------------------------------------------------------------
double __fastcall TResumoTec::PM_GetDiagTrechoKm(int ano, int idiag)
{
	double neq = 0;
	if (idiag == diagRUIM)
		neq = (PD.energia[IndiceAno(ano)]).diag_tre_ruim_km;
	else if (idiag == diagREGULAR)
		neq = (PD.energia[IndiceAno(ano)]).diag_tre_reg_km;
	else if (idiag == diagBOM)
		neq = (PD.energia[IndiceAno(ano)]).diag_tre_bom_km;

	return (neq);
}

// ---------------------------------------------------------------------------
double __fastcall TResumoTec::PM_GetDEC(int ano)
{ // DVK 2015.08.12
	return ((PD.energia[IndiceAno(ano)]).dec);
}

// ---------------------------------------------------------------------------
double __fastcall TResumoTec::PM_GetEND(int ano)
{
	return ((PD.energia[IndiceAno(ano)]).end);
}

// ---------------------------------------------------------------------------
double __fastcall TResumoTec::PM_GetFEC(int ano)
{ // DVK 2015.08.12
	return ((PD.energia[IndiceAno(ano)]).fec);
}

// ---------------------------------------------------------------------------
double __fastcall TResumoTec::PM_GetEntrada(int ano)
{
	return ((PD.energia[IndiceAno(ano)]).entrada_mwhm);
}

// ---------------------------------------------------------------------------
double __fastcall TResumoTec::PM_GetEntregue(int ano)
{
	return ((PD.energia[IndiceAno(ano)]).entregue_mwhm);
}

// ---------------------------------------------------------------------------
double __fastcall TResumoTec::PM_GetGerada(int ano)
{
	return ((PD.energia[IndiceAno(ano)]).gerada_mwhm);
}

// ---------------------------------------------------------------------------
double __fastcall TResumoTec::PM_GetNota(void)
{
	return (0.);
}

// ---------------------------------------------------------------------------
double __fastcall TResumoTec::PM_GetPerda_mwhm(int ano)
{
	return ((PD.energia[IndiceAno(ano)]).perda_mwhm);
}

// ---------------------------------------------------------------------------
double __fastcall TResumoTec::PM_GetPerda_perc(int ano)
{
	return ((PD.energia[IndiceAno(ano)]).perda_perc);
}

// ---------------------------------------------------------------------------
double __fastcall TResumoTec::PM_GetPercDiagBarra(int ano, int idiag)
{ // DVK 2013.12.20
	double neq = 0;

	if (idiag == diagRUIM)
		neq = (PD.energia[IndiceAno(ano)]).perc_bar_ruim;
	else if (idiag == diagREGULAR)
		neq = (PD.energia[IndiceAno(ano)]).perc_bar_reg;
	else if (idiag == diagBOM)
		neq = (PD.energia[IndiceAno(ano)]).perc_bar_bom;

	return (neq);
}

// ---------------------------------------------------------------------------
double __fastcall TResumoTec::PM_GetPercDiagCarga(int ano, int idiag)
{
	double neq = 0;

	if (idiag == diagRUIM)
		neq = (PD.energia[IndiceAno(ano)]).perc_car_ruim;
	else if (idiag == diagREGULAR)
		neq = (PD.energia[IndiceAno(ano)]).perc_car_reg;
	else if (idiag == diagBOM)
		neq = (PD.energia[IndiceAno(ano)]).perc_car_bom;
	return (neq);
}

// ---------------------------------------------------------------------------
double __fastcall TResumoTec::PM_GetPercDiagTrafo(int ano, int idiag)
{ // DVK 2013.12.20
	double neq = 0;

	if (idiag == diagRUIM)
		neq = (PD.energia[IndiceAno(ano)]).perc_tra_ruim;
	else if (idiag == diagREGULAR)
		neq = (PD.energia[IndiceAno(ano)]).perc_tra_reg;
	else if (idiag == diagBOM)
		neq = (PD.energia[IndiceAno(ano)]).perc_tra_bom;

	return (neq);
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetCalculo_OK(int ano, bool calculo_ok)
{
	(PD.energia[IndiceAno(ano)]).calculo_ok = calculo_ok;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetCompensacao_RS(int ano, double valor)
{
	(PD.energia[IndiceAno(ano)]).compensacao = valor;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetDiagBarra(int ano, int idiag, int neq)
{
	if (idiag == diagRUIM)
		(PD.energia[IndiceAno(ano)]).diag_bar_ruim = neq;
	else if (idiag == diagREGULAR)
		(PD.energia[IndiceAno(ano)]).diag_bar_reg = neq;
	else if (idiag == diagBOM)
		(PD.energia[IndiceAno(ano)]).diag_bar_bom = neq;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetDiagCarga(int ano, int idiag, int neq)
{
	if (idiag == diagRUIM)
		(PD.energia[IndiceAno(ano)]).diag_car_ruim = neq;
	else if (idiag == diagREGULAR)
		(PD.energia[IndiceAno(ano)]).diag_car_reg = neq;
	else if (idiag == diagBOM)
		(PD.energia[IndiceAno(ano)]).diag_car_bom = neq;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetDiagTrafo(int ano, int idiag, int neq)
{
	if (idiag == diagRUIM)
		(PD.energia[IndiceAno(ano)]).diag_tra_ruim = neq;
	else if (idiag == diagREGULAR)
		(PD.energia[IndiceAno(ano)]).diag_tra_reg = neq;
	else if (idiag == diagBOM)
		(PD.energia[IndiceAno(ano)]).diag_tra_bom = neq;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetDiagTrechoKm(int ano, int idiag, double comp_km)
{
	if (idiag == diagRUIM)
		(PD.energia[IndiceAno(ano)]).diag_tre_ruim_km = comp_km;
	else if (idiag == diagREGULAR)
		(PD.energia[IndiceAno(ano)]).diag_tre_reg_km = comp_km;
	else if (idiag == diagBOM)
		(PD.energia[IndiceAno(ano)]).diag_tre_bom_km = comp_km;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetDEC(int ano, double valor)
{ // DVK 2015.08.12
	(PD.energia[IndiceAno(ano)]).dec = valor;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetEND(int ano, double valor)
{
	(PD.energia[IndiceAno(ano)]).end = valor;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetFEC(int ano, double valor)
{ // DVK 2015.08.12
	(PD.energia[IndiceAno(ano)]).fec = valor;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetEntrada(int ano, double valor)
{
	(PD.energia[IndiceAno(ano)]).entrada_mwhm = valor;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetEntregue(int ano, double valor)
{
	(PD.energia[IndiceAno(ano)]).entregue_mwhm = valor;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetGerada(int ano, double valor)
{
	(PD.energia[IndiceAno(ano)]).gerada_mwhm = valor;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetPercDiagBarra(int ano, int idiag, double neq)
{ // DVK 2013.12.20
	if (idiag == diagRUIM)
		(PD.energia[IndiceAno(ano)]).perc_bar_ruim = neq;
	else if (idiag == diagREGULAR)
		(PD.energia[IndiceAno(ano)]).perc_bar_reg = neq;
	else if (idiag == diagBOM)
		(PD.energia[IndiceAno(ano)]).perc_bar_bom = neq;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetPercDiagCarga(int ano, int idiag, double neq)
{
	if (idiag == diagRUIM)
		(PD.energia[IndiceAno(ano)]).perc_car_ruim = neq;
	else if (idiag == diagREGULAR)
		(PD.energia[IndiceAno(ano)]).perc_car_reg = neq;
	else if (idiag == diagBOM)
		(PD.energia[IndiceAno(ano)]).perc_car_bom = neq;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetPercDiagTrafo(int ano, int idiag, double neq)
{ // DVK 2013.12.20
	if (idiag == diagRUIM)
		(PD.energia[IndiceAno(ano)]).perc_tra_ruim = neq;
	else if (idiag == diagREGULAR)
		(PD.energia[IndiceAno(ano)]).perc_tra_reg = neq;
	else if (idiag == diagBOM)
		(PD.energia[IndiceAno(ano)]).perc_tra_bom = neq;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetPerda_mwhm(int ano, double valor)
{
	(PD.energia[IndiceAno(ano)]).perda_mwhm = valor;
}

// ---------------------------------------------------------------------------
void __fastcall TResumoTec::PM_SetPerda_perc(int ano, double valor)
{
	(PD.energia[IndiceAno(ano)]).perda_perc = valor;
}

// ---------------------------------------------------------------------------
// eof
