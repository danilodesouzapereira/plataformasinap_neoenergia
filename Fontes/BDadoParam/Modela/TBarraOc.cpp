// ---------------------------------------------------------------------------
#pragma hdrstop
#include <DateUtils.hpp>
#include "TBarraOc.h"
// #include "VTOcorr.h"
#include <PlataformaSinap\DLL_Inc\Funcao.h>

// ---------------------------------------------------------------------------
// hk201703: adaptacoes planTurmas: aplicação de fator para o tma
// hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
// hk201706: multiplas bases módulo de planejamento de turmas
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTBarraOc* __fastcall NewObjBarraOc(void)
{
	try
	{ // cria objeto
		return (new TBarraOc());
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TBarraOc::TBarraOc(void)
{
	// cria listas
	lisOcorr_f = new TList();
	lisTrataOcorr = new TList();
	lisCircuitos = new TList(); // lista de objetos da classe VTCircuito
	eqpOcorr = NULL;
	redePri = NULL;
	// Parametros para cálculo de continuidade
	parEqptoExt = NULL;
	cli = 0;
	clih = 0.0;
	ndiasObs = 365;
	nconsBT = 0; // numero de consumidores BT hknova
	flagEncontraRede = false; // se foi encontrado equipamento correspondente na rede aberta

	nocorPond = 0; // numero de ocorrencias, se em BT ponderada pela abrangencias das falhas
	tdeslMedio = 0.0; // tempo de desligamento médio

	nocorRamal = 0.0; // Numero de ocorrencias em ramal da BT
	nocorNRamal = 0.0; // Número de ocorrencias cheias em trafoBT ou na rede MT

	for (int n = 0; n < 23; n++)
		nfhora[n] = 0.0;
	for (int n = 0; n < 12; n++)
	{
		nfmes[n] = nfrmes[n] = tdmes[n] = 0.0;
	}
	// seta parâmetros
	// x = y = 0.0; //hk
	// polo = "";   //hk
	// pabr = NULL;
	lisParamCarga = new TList(); // hk201703: adaptacoes planTurmas: aplicação de fator para o tma
	lisParamBloco = new TList(); // hk201703: adaptacoes planTurmas: aplicação de fator para o tma

	// hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
	ptpre = 0.0;
	ptdlo = 0.0;
	ptexe = 0.0;

	// hk201706: multiplas bases módulo de planejamento de turmas
	tmaReal = tmaBase = 30.0;
}

// ---------------------------------------------------------------------------
__fastcall TBarraOc::~TBarraOc(void)
{
	// destroi listas, mas não objetos
	if (lisOcorr_f)
	{
		delete lisOcorr_f;
		lisOcorr_f = NULL;
	}
	if (lisTrataOcorr)
	{
		delete lisTrataOcorr;
		lisTrataOcorr = NULL;
	}
	if (lisCircuitos)
	{
		delete lisCircuitos;
		lisCircuitos = NULL;
	}
	if (lisParamCarga)
	{
		delete lisParamCarga;
		lisParamCarga = NULL;
	} // hk201703: adaptacoes planTurmas: aplicação de fator para o tma
	if (lisParamBloco)
	{
		delete lisParamBloco;
		lisParamBloco = NULL;
	} // hk201703: adaptacoes planTurmas: aplicação de fator para o tma
}

// ---------------------------------------------------------------------------
// eof
