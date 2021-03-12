// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TParamConf.h"
#include "VTVerifCircuito.h"
#include "VTVerifCjto.h"
#include "VTVerifSe.h"
#include "..\BDado\VTBDadoParam.h"//hk201808: reformula
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// COMENTÁRIOS
// hk201608 IniciaParamDefault(void):LimpaTList(lisVerifSe)
// hk201608 IniciaParamDefault(void):LimpaTList(lisVerifCircuito)
// hk201608 IniciaParamDefault(void):for(int n = 0; n < 24; n++) distFalhasPat[n] = 1/24;
// hk201702: ComprimentoBloco(VTBloco *bloco) para a inserção de novos paramBloco
// ---------------------------------------------------------------------------
VTParamConf* __fastcall NewObjParamConf(void)
{
	try
	{ // cria objeto
		return (new TParamConf());
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TParamConf::TParamConf(void)
{
	// cria objeto
	paramGerais = new strDadoParamGerais;
	// cria listas
	lisParamBloco = new TList();
	lisParamCarga = new TList();
	lisParamRede = new TList();
	lisParamEqpto = new TList();
	lisVerifCjto = new TList();
	lisVerifSe = new TList();
	lisVerifCircuito = new TList();
	lisChavesTelecom = new TList();
	//gh201806: possibilidade de salvar chaves da rede que possuem IF alocado
   lisChavesIF = new TList();
	lisTipoEqProt = new TList();
   lisEqptoIndisp = new TList();//hk201808: indisp:
	distFalhasPat = new double[24];
   eqptoIndispPadrao = new strDadoEqptoIndisp;
	// inicializa com parâmetros default
	IniciaParamDefault();
   //hk201808: reformula
   pathParam = "";
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/15456154
__fastcall TParamConf::~TParamConf(void)
{
    // destroi listas
    if(lisEqptoIndisp)
        {LimpaTList(lisEqptoIndisp);   delete lisEqptoIndisp;    lisEqptoIndisp    = NULL;}//hk201808: indisp:
    if(eqptoIndispPadrao)
        {                              delete eqptoIndispPadrao; eqptoIndispPadrao = NULL;}//hk201808: indisp:
    strDadoParamBloco *paramBloco;
    for(int npb = 0; npb < lisParamBloco->Count; npb++)
    {
        paramBloco = (strDadoParamBloco*)lisParamBloco->Items[npb];
        if(paramBloco->lisObrasBloco)
        {
            LimpaTList(paramBloco->lisObrasBloco);
            delete paramBloco->lisObrasBloco;
            paramBloco->lisObrasBloco = NULL;
        }
    }
    if(lisParamBloco)
        {LimpaTList(lisParamBloco);    delete lisParamBloco;     lisParamBloco     = NULL;}
    if(lisParamCarga)
        {LimpaTList(lisParamCarga);    delete lisParamCarga;     lisParamCarga     = NULL;}
    if(lisParamRede)
        {LimpaTList(lisParamRede);     delete lisParamRede;      lisParamRede      = NULL;}
    if(lisParamEqpto)
        {LimpaTList(lisParamEqpto);    delete lisParamEqpto;     lisParamEqpto     = NULL;}
    if(lisChavesTelecom)
        {LimpaTList(lisChavesTelecom); delete lisChavesTelecom;  lisChavesTelecom  = NULL;}
    //gh201806: destrutor de lista de chaves IF
    if(lisChavesIF)
        {LimpaTList(lisChavesIF);      delete lisChavesIF;       lisChavesIF       = NULL;}
    if(lisVerifCircuito)
        {LimpaTList(lisVerifCircuito); delete lisVerifCircuito;  lisVerifCircuito  = NULL;}
    if(lisVerifSe)
        {LimpaTList(lisVerifSe);       delete lisVerifSe;        lisVerifSe        = NULL;}
    if(lisVerifCjto)
        {LimpaTList(lisVerifCjto);     delete lisVerifCjto;      lisVerifCjto      = NULL;}
    if(distFalhasPat)
        {                              delete[]distFalhasPat;    distFalhasPat     = NULL;}
    // destroi objeto
    if(paramGerais)
        {                              delete paramGerais;       paramGerais       = NULL;}
}

// ---------------------------------------------------------------------------
VTParamConf* __fastcall TParamConf::Clone(VTParamConf* parConfOrig)
{
	// Falta implementar!!
	return (NULL);
}

// ---------------------------------------------------------------------------
VTVerifCircuito* __fastcall TParamConf::CriaVerifCircuito(void)
{
	return (NewObjVerifCircuito());
}

// ---------------------------------------------------------------------------
VTVerifCjto* __fastcall TParamConf::CriaVerifCjto(void)
{
	return (NewObjVerifCjto());
}

// ---------------------------------------------------------------------------
VTVerifSe* __fastcall TParamConf::CriaVerifSe(void)
{
	return (NewObjVerifSe());
}

// ---------------------------------------------------------------------------
VTVerifCircuito* __fastcall TParamConf::ExisteVerifCircuito(AnsiString codigo)
{
	// variáveis locais
	VTVerifCircuito *verifCircuito;

	for (int n = 0; n < lisVerifCircuito->Count; n++)
	{
		verifCircuito = (VTVerifCircuito*)lisVerifCircuito->Items[n];
		if (verifCircuito->Codigo.AnsiCompareIC(codigo) == 0)
			return (verifCircuito);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTVerifSe* __fastcall TParamConf::ExisteVerifSe(AnsiString codigo)
{
	// variáveis locais
	VTVerifSe *verifSe;

	for (int n = 0; n < lisVerifSe->Count; n++)
	{
		verifSe = (VTVerifSe*)lisVerifSe->Items[n];
		if (verifSe->Codigo.AnsiCompareIC(codigo) == 0)
			return (verifSe);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
strDadoParamGerais* __fastcall TParamConf::PM_GetParamGerais(void)
{
	return (paramGerais);
}

// ---------------------------------------------------------------------------
strDadoParamBloco* __fastcall TParamConf::PM_GetParamBloco(VTBloco *bloco)
{
	strDadoParamBloco *paramBloco = NULL;
	for (int np = 0; np < lisParamBloco->Count; np++)
	{
		paramBloco = (strDadoParamBloco*)lisParamBloco->Items[np];
		if (paramBloco->bloco == bloco)
			break;
		paramBloco = NULL;
	}
	return (paramBloco);
}

// ---------------------------------------------------------------------------
strDadoParamCarga* __fastcall TParamConf::PM_GetParamCarga(VTCarga *carga)
{
	strDadoParamCarga *paramCarga = NULL;
	for (int np = 0; np < lisParamCarga->Count; np++)
	{
		paramCarga = (strDadoParamCarga*)lisParamCarga->Items[np];
		if (paramCarga->carga == carga)
			break;
		paramCarga = NULL;
	}
	return (paramCarga);
}

// ---------------------------------------------------------------------------
strDadoParamRede* __fastcall TParamConf::PM_GetParamRede(VTRede *rede)
{
	strDadoParamRede* paramRede = NULL;
	if (rede == NULL)
		return (NULL);
	for (int np = 0; np < lisParamRede->Count; np++)
	{
		paramRede = (strDadoParamRede*)lisParamRede->Items[np];
		if (paramRede->rede == rede)
			break;
		paramRede = NULL;
	}
	return (paramRede);
}

// ---------------------------------------------------------------------------
strDadoParamRede* __fastcall TParamConf::PM_GetParamCodRede(AnsiString codRede)
{
	strDadoParamRede* paramRede = NULL;

	for (int np = 0; np < lisParamRede->Count; np++)
	{
		paramRede = (strDadoParamRede*)lisParamRede->Items[np];
		if (paramRede->codRede.AnsiCompare(codRede.Trim()) == 0)
			break;
		paramRede = NULL;
	}
	return (paramRede);
}

// ---------------------------------------------------------------------------
TList* __fastcall TParamConf::PM_GetLisParamBloco(void)
{
	return (lisParamBloco);
}

// ---------------------------------------------------------------------------
TList* __fastcall TParamConf::PM_GetLisParamCarga(void)
{
	return (lisParamCarga);
}

// ---------------------------------------------------------------------------
TList* __fastcall TParamConf::PM_GetLisParamEqpto(void)
{
	return (lisParamEqpto);
}

// ---------------------------------------------------------------------------
TList* __fastcall TParamConf::PM_GetLisParamRede(void)
{
	return (lisParamRede);
}

// ---------------------------------------------------------------------------
TList* __fastcall TParamConf::PM_GetLisVerifCircuito(void)
{
	return (lisVerifCircuito);
}

// ---------------------------------------------------------------------------
TList* __fastcall TParamConf::PM_GetLisVerifCjto(void)
{
	return (lisVerifCjto);
}

// ---------------------------------------------------------------------------
TList* __fastcall TParamConf::PM_GetLisVerifSe(void)
{
	return (lisVerifSe);
}

// ---------------------------------------------------------------------------
TList* __fastcall TParamConf::PM_GetLisChavesTelecom(void)
{
	return (lisChavesTelecom);
}

// ---------------------------------------------------------------------------
TList* __fastcall TParamConf::PM_GetLisChavesIF(void)
{
   //gh201809
	return (lisChavesIF);
}

// ---------------------------------------------------------------------------
TList* __fastcall TParamConf::PM_GetLisTipoEqProt(void)
{
	return (lisTipoEqProt);
}

// ---------------------------------------------------------------------------
void __fastcall TParamConf::ImprimeParametrosSimula(AnsiString arq)
{
	// Imprime parametros de entrada de simulação
	// flagRecurso;   //sim se utiliza recursos de socorro a jusante
	// flagRetorno;   //sim se considera influencia de manobras de retorno nos indicadores
	// tlocaliza;     //duração de preparação e localizacao de defeito
	// tman_manual;   //duração de manobras manuais
	// tman_auto;     //duração de manobra com telecomando
	// pu_lim;        //limite de sobrecarga na transferencia
	// percDefTrans;  //percentual de defeitos transitórios
	// iProfReliga;   //sensibilidade de religamento: =0, somente o bloco imediatamente a jusante: =1, até o próximo religador
	// tlimIC;        //duração máxima do desligamento para não acrescentar os indicadores de contin.
	// distFalhasPat[24]

	// strDadoParamGerais->lambda,trep,lamb[12],tr[12]
	// lisParamRede->strDadoParamRede->lambda,trep,lamb[12],tr[12]
	// lisParamBloco->strDadoParamBloco->lambda,trep,lamb_pu[12],tr_pu[12]
	// lisParamCarga->strDadoParamCarga->lambda,trep,lamb_pu[12],tr_pu[12]

	FILE *fout;
	strDadoParamRede *paramRede;
	strDadoParamBloco *paramBloco;
	strDadoParamCarga *paramCarga;
	strDadoParamEqpto *paramEqpto;
	AnsiString aux;
	double daux, soma1, soma2;
	int iaux;

	fout = fopen(arq.c_str(), "wt");

	fprintf(fout, "PARÂMETROS GERAIS\n");
	aux = paramGerais->flagRecurso ? "S" : "N";
	fprintf(fout, "%s ", aux.c_str());
	aux = paramGerais->flagRetorno ? "S" : "N";
	fprintf(fout, "%s\n", aux.c_str());
	daux = paramGerais->tlocaliza;

	fprintf(fout, "%.6f\n", daux);
	daux = paramGerais->tman_manual;
	fprintf(fout, "%.6f\n", daux);
	daux = paramGerais->tman_auto;
	fprintf(fout, "%.6f\n", daux);
	daux = paramGerais->pu_lim;
	fprintf(fout, "%.6f\n", daux);
	daux = paramGerais->percDefTrans;
	fprintf(fout, "%.6f\n", daux);
	iaux = paramGerais->iProfReliga;
	fprintf(fout, "%i\n", iaux);
	daux = paramGerais->tlimIC;
	fprintf(fout, "%.6f\n", daux);

	fprintf(fout, "DISTRIBUIÇÃO HORÁRIA DE FALHAS\n");
	for (int n = 0; n < 24; n++)
	{
		fprintf(fout, "%i; ", n);
		daux = distFalhasPat[n];
		fprintf(fout, "%.6f\n", daux);
	}

	double lambda, trep;
	// strDadoParamGerais->lambda,trep,lamb[12],tr[12]
	fprintf(fout, "GERAIS: TAXAS E TEMPOS\n");
	lambda = paramGerais->lambda;
	trep = paramGerais->trep;
	fprintf(fout, "%.6f; %.6f\n", lambda, trep);
	// for(int n = 0; n < 12; n++)
	// {
	// fprintf(fout, "%i, ", n);
	// lambda = paramGerais->lamb[n]; trep = paramGerais->tr[n];
	// fprintf(fout, "%.6f, %.6f\n", lambda, trep);
	// }

	// lisParamRede->strDadoParamRede->lambda,trep,lamb[12],tr[12]
	fprintf(fout, "REDE: TAXAS E TEMPOS\n");
	for (int n = 0; n < lisParamRede->Count; n++)
	{
		paramRede = (strDadoParamRede*)lisParamRede->Items[n];
		fprintf(fout, "%i; %s; ", n, paramRede->codRede.c_str());
		lambda = paramRede->lambda;
		trep = paramRede->trep;
		fprintf(fout, "%.6f; %.6f\n", lambda, trep);
		// for(int n = 0; n < 12; n++)
		// {
		// fprintf(fout, "%i, ", n);
		// lambda = paramRede->lamb[n]; trep = paramRede->tr[n];
		// fprintf(fout, "%.6f, %.6f\n", lambda, trep);
		// }
	}
	// lisParamBloco->strDadoParamEqpto->lambda,trep,lamb_pu[12],tr_pu[12]
	fprintf(fout, "EQUIPAMENTOS: TAXAS E TEMPOS\n");
	for (int n = 0; n < lisParamEqpto->Count; n++)
	{
		paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[n];
		fprintf(fout, "%i; %s; ", n, paramEqpto->codEqpto.c_str());
		fprintf(fout, "%.6f; ", paramEqpto->nFalhasEquip);
		fprintf(fout, "%.6f; ", paramEqpto->durTotal);
		fprintf(fout, "%.0f; ", paramEqpto->ndiasObs);
		lambda = paramEqpto->lambda;
		trep = paramEqpto->trep;
		fprintf(fout, "%.6f; %.6f\n", lambda, trep);
		// for(int n = 0; n < 12; n++)
		// {
		// fprintf(fout, "%i, ", n);
		// lambda = paramEqpto->lamb_pu[n]; trep = paramEqpto->tr_pu[n];
		// fprintf(fout, "%.6f, %.6f\n", lambda, trep);
		// }
	}

	// lisParamBloco->strDadoParamBloco->lambda,trep,lamb_pu[12],tr_pu[12]
	soma1 = soma2 = 0.0;
	fprintf(fout, "BLOCOS: TAXAS E TEMPOS\n");
	for (int n = 0; n < lisParamBloco->Count; n++)
	{
		paramBloco = (strDadoParamBloco*)lisParamBloco->Items[n];
		fprintf(fout, "%i; %s; ", n, paramBloco->codChave.c_str());
		lambda = paramBloco->lambda;
		trep = paramBloco->trep;
		fprintf(fout, "%.6f; %.6f\n", lambda, trep);
		soma1 += lambda;
		soma2 += trep;
		// for(int n = 0; n < 12; n++)
		// {
		// fprintf(fout, "%i, ", n);
		// lambda = paramBloco->lamb_pu[n]; trep = paramBloco->tr_pu[n];
		// fprintf(fout, "%.6f, %.6f\n", lambda, trep);
		// }
	}

	lambda = soma1;
	trep = soma2;
	fprintf(fout, "-----------------------------------\n");//, lambda, trep);
	fprintf(fout, "%.6f; %.6f\n", lambda, trep);
	fprintf(fout, "-----------------------------------\n");//, lambda, trep);

	// lisParamCarga->strDadoParamCarga->lambda,trep,lamb_pu[12],tr_pu[12]
	soma1 = soma2 = 0.0;
	fprintf(fout, "CARGAS BT: TAXAS E TEMPOS\n");
	for (int n = 0; n < lisParamCarga->Count; n++)
	{
		paramCarga = (strDadoParamCarga*)lisParamCarga->Items[n];
		fprintf(fout, "%i; %s; ", n, paramCarga->codCarga.c_str());
		lambda = paramCarga->lambda;
		trep = paramCarga->trep;
		fprintf(fout, "%.6f; %.6f\n", lambda, trep);
		soma1 += lambda;
		soma2 += trep;
		// for(int n = 0; n < 12; n++)
		// {
		// fprintf(fout, "%i, ", n);
		// lambda = paramCarga->lamb_pu[n]; trep = paramCarga->tr_pu[n];
		// fprintf(fout, "%.6f, %.6f\n", lambda, trep);
		// }
	}
	lambda = soma1;
	trep = soma2;
	fprintf(fout, "-----------------------------------\n");//, lambda, trep);
	fprintf(fout, "%.6f; %.6f\n", lambda, trep);
	fprintf(fout, "-----------------------------------\n");//, lambda, trep);

	fclose(fout);
}

// ---------------------------------------------------------------------------
void __fastcall TParamConf::ImprimeParametrosMes(AnsiString arq)
{
	FILE *fout;
	strDadoParamEqpto *paramEqpto;
	strDadoParamRede *paramRede;
	strDadoParamBloco *paramBloco;

	// lisParamEqpto --> nfalhasEquip, nfEquip[nm], durTot[nm];
	// lisParamBloco --> bloco, chave, neqProt, chaveProt, nFalhas, lamb_pu[nm], tr_pu[nm]
	// lisParamCarga --> carga, nfalhas, lamb_pu[nm], tr_pu[nm]

	fout = fopen(arq.c_str(), "wt");

	fprintf(fout, "-----------------------------------\n");
	fprintf(fout, "------PARAMETROS EQUIPAMENTO-------\n");
	fprintf(fout, "-----------------------------------\n");
	fprintf(fout, "codEqpto;");
	fprintf(fout, "redeCodigo;");
	fprintf(fout, "TipoRede;");
	fprintf(fout, "nf;");
	fprintf(fout, "nfr;");
	fprintf(fout, "nFalhasEquip;");
	fprintf(fout, "cli;");
	fprintf(fout, "clih;");
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "nfmes[%i]; ", nm);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "nfrmes[%i]; ", nm);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "nFEquip[%i]; ", nm);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "cli[%i]; ", nm);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "clih[%i]; ", nm);
	fprintf(fout, "\n");

	for (int n = 0; n < lisParamEqpto->Count; n++)
	{
		paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[n];
		if ((paramEqpto->eqpto == NULL) && (paramEqpto->carga == NULL))
			continue;

		fprintf(fout, "%s; ", paramEqpto->codEqpto.c_str());
		fprintf(fout, "%s; ", paramEqpto->rede->Codigo.c_str());
		fprintf(fout, "%s; ", paramEqpto->tipoRede.c_str());
		fprintf(fout, "%.6f; ", paramEqpto->nf);
		fprintf(fout, "%.6f; ", paramEqpto->nfr);
		fprintf(fout, "%.6f; ", paramEqpto->nFalhasEquip);
		fprintf(fout, "%.6f; ", paramEqpto->cli);
		fprintf(fout, "%.6f; ", paramEqpto->clih);
		for (int nm = 0; nm < 12; nm++)
			fprintf(fout, "%.6f; ", paramEqpto->nfmes[nm]);
		for (int nm = 0; nm < 12; nm++)
			fprintf(fout, "%.6f; ", paramEqpto->nfrmes[nm]);
		for (int nm = 0; nm < 12; nm++)
			fprintf(fout, "%.6f; ", paramEqpto->nFEquip[nm]);
		for (int nm = 0; nm < 12; nm++)
			fprintf(fout, "%.6f; ", paramEqpto->climes[nm]);
		for (int nm = 0; nm < 12; nm++)
			fprintf(fout, "%.6f; ", paramEqpto->clihmes[nm]);
		fprintf(fout, "\n");
	}

	fprintf(fout, "-----------------------------------\n");
	fprintf(fout, "------PARAMETROS GERAL-------------\n");
	fprintf(fout, "-----------------------------------\n");
	fprintf(fout, "nFalhasEquip; ");
	fprintf(fout, "cli; ");
	fprintf(fout, "clih; ");
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "nFEquip[%i]; ", nm);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "cli[%i]; ", nm);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "clih[%i]; ", nm);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "lamb_pu[%i]; ", nm);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "tr_pu[%i]; ", nm);
	fprintf(fout, "\n");

	fprintf(fout, "%.6f; ", paramGerais->nFalhasEquip);
	fprintf(fout, "%.6f; ", paramGerais->cli);
	fprintf(fout, "%.6f; ", paramGerais->clih);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "%.6f; ", paramGerais->nFEquip[nm]);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "%.6f; ", paramGerais->climes[nm]);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "%.6f; ", paramGerais->clihmes[nm]);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "%.6f; ", paramGerais->lamb_pu[nm]);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "%.6f; ", paramGerais->tr_pu[nm]);
	fprintf(fout, "\n");

	fprintf(fout, "-----------------------------------\n");
	fprintf(fout, "------PARAMETROS REDE-------------\n");
	fprintf(fout, "-----------------------------------\n");
	fprintf(fout, "CodRede; ");
	fprintf(fout, "nFalhasEquip; ");
	fprintf(fout, "cli; ");
	fprintf(fout, "clih; ");
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "nFEquip[%i]; ", nm);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "cli[%i]; ", nm);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "clih[%i]; ", nm);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "lamb_pu[%i]; ", nm);
	for (int nm = 0; nm < 12; nm++)
		fprintf(fout, "tr_pu[%i]; ", nm);
	fprintf(fout, "\n");
	for (int nr = 0; nr < lisParamRede->Count; nr++)
	{
		paramRede = (strDadoParamRede*)lisParamRede->Items[nr];
		fprintf(fout, "%s; ", paramRede->codRede.c_str());
		fprintf(fout, "%.6f; ", paramRede->nFalhasEquip);
		fprintf(fout, "%.6f; ", paramRede->durTotal);

		for (int nm = 0; nm < 12; nm++)
			fprintf(fout, "%.6f; ", paramRede->nFEquip[nm]);
		for (int nm = 0; nm < 12; nm++)
			fprintf(fout, "%.6f; ", paramRede->climes[nm]);
		for (int nm = 0; nm < 12; nm++)
			fprintf(fout, "%.6f; ", paramRede->clihmes[nm]);
		for (int nm = 0; nm < 12; nm++)
			fprintf(fout, "%.6f; ", paramRede->lamb_pu[nm]);
		for (int nm = 0; nm < 12; nm++)
			fprintf(fout, "%.6f; ", paramRede->tr_pu[nm]);
		fprintf(fout, "\n");
	}

	fclose(fout);
}

// ---------------------------------------------------------------------------
void __fastcall TParamConf::ImprimeParametrosEntrada(AnsiString arq)
{
	// TList *lisParamRede  = paramConf->LisParamRede;
	// TList *lisParamEqpto = paramConf->LisParamEqpto;
	// TList *lisParamBloco = paramConf->LisParamBloco;
	FILE *fout;
	strDadoParamEqpto *paramEqpto;
	strDadoParamRede *paramRede;

	fout = fopen(arq.c_str(), "wt");
	fprintf(fout, "-------------------------------------------------\n");
	fprintf(fout, "-------------Parametros Entrada  ----------------\n");
	fprintf(fout, "-------------------------------------------------\n");
	fprintf(fout, "Parametros gerais\n");
	fprintf(fout, "lambda;");
	fprintf(fout, "trep\n");
	fprintf(fout, "%6.2f;", paramGerais->lambda); // hkdef
	fprintf(fout, "%6.2f\n", paramGerais->trep); // hkdef

	fprintf(fout, "Recurso: %s\n", (paramGerais->flagRecurso ? "S" : "N"));
	fprintf(fout, "Retorno: %s\n", (paramGerais->flagRetorno ? "S" : "N"));
	fprintf(fout, "TLocaliza: %6.2f\n", paramGerais->tlocaliza);
	fprintf(fout, "TMan_manual: %6.2f\n", paramGerais->tman_manual);
	fprintf(fout, "TMan_auto:   %6.2f\n", paramGerais->tman_auto);
	fprintf(fout, "PuTransf:    %6.2f\n", paramGerais->pu_lim);
	fprintf(fout, "Perc.Def.Temp.: %6.2f\n", paramGerais->percDefTrans);
	fprintf(fout, "Prof.Relig:     %s\n", ((paramGerais->iProfReliga == 0) ? "Mínima" : "Máxima"));

	fprintf(fout, "Equipamentos de protecao\n");
	strDadoTipoEqProt *tipoEqProt;
	for (int n = 0; n < lisTipoEqProt->Count; n++)
	{
		tipoEqProt = (strDadoTipoEqProt*)lisTipoEqProt->Items[n];
		if (tipoEqProt->tipoChave == NULL)
			continue;
		fprintf(fout, "%s\n", tipoEqProt->codTipoEqProt.c_str());
	}

	fprintf(fout, "Chaves telecomandadas\n");
	strDadoChaveTelecom *dadoChave;
	for (int n = 0; n < lisChavesTelecom->Count; n++)
	{
		dadoChave = (strDadoChaveTelecom*)lisChavesTelecom->Items[n];

		if (dadoChave->chave == NULL)
			continue;

		fprintf(fout, "%s\n", dadoChave->codChave.c_str());
	}

	fprintf(fout, "Distribuicao de falhas\n");
	fprintf(fout, "hora; percFalhas\n");
	for (int n = 0; n < 24; n++)
	{
		fprintf(fout, "%i; %6.2f\n", (n + 1), distFalhasPat[n]);
	}

	fprintf(fout, "Parametros por rede\n");
	fprintf(fout, "Rede;");
	// fprintf(fout, "nFalhasEquip;");
	// fprintf(fout, "durTotal;");
	fprintf(fout, "lambda;");
	fprintf(fout, "trep\n");
	// fprintf(fout, "taxa_BT;");
	// fprintf(fout, "cli_bt;");
	// fprintf(fout, "clih_bt\n");
	for (int n = 0; n < lisParamRede->Count; n++)
	{
		paramRede = (strDadoParamRede*)lisParamRede->Items[n];
		if (paramRede->rede == NULL)
			continue; // fprintf(fout, "%s;", paramRede->codRede);
		else
			fprintf(fout, "%s;", paramRede->rede->Codigo.c_str());
		fprintf(fout, "%6.2f;", paramRede->lambda);
		fprintf(fout, "%6.2f\n", paramRede->trep);

		// fprintf(fout, "%6.2f;", paramRede->taxa_BT);
		// fprintf(fout, "%6.2f;", paramRede->cli_bt);
		// fprintf(fout, "%6.2f\n", paramRede->clih_bt);
	}

	fprintf(fout, "Parametros por Equipamento MT\n");
	fprintf(fout, "codEqpto;");
	fprintf(fout, "codRede;");
	fprintf(fout, "ndiasObs;");
	fprintf(fout, "nFalhasEquip;");
	fprintf(fout, "durTotal;");
	fprintf(fout, "lambda;");
	fprintf(fout, "trep\n");
	for (int n = 0; n < lisParamEqpto->Count; n++)
	{
		paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[n];
		// Se não foi encontrado na rede não grava
		if ((paramEqpto->eqpto == NULL) && (paramEqpto->carga == NULL))
			continue;
		if (!paramEqpto->eqpto)
			continue;

		fprintf(fout, "%s;", paramEqpto->eqpto->Codigo.c_str());
		if (paramEqpto->rede != NULL)
			fprintf(fout, "%s;", paramEqpto->rede->Codigo.c_str());
		else
			fprintf(fout, "NLOC;");

		fprintf(fout, "%6.2f;", paramEqpto->ndiasObs);
		fprintf(fout, "%6.2f;", paramEqpto->nFalhasEquip);
		fprintf(fout, "%6.2f;", paramEqpto->durTotal);

		fprintf(fout, "%6.2f;", paramEqpto->lambda);
		fprintf(fout, "%6.2f\n", paramEqpto->trep);

	}

	fprintf(fout, "Parametros por Equipamento BT\n");
	fprintf(fout, "codEqpto;");
	fprintf(fout, "codRede;");
	fprintf(fout, "ndiasObs;");
	fprintf(fout, "nFalhasEquip;");
	fprintf(fout, "durTotal;");
	fprintf(fout, "lambda;");
	fprintf(fout, "trep\n");
	for (int n = 0; n < lisParamEqpto->Count; n++)
	{
		paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[n];
		// Se não foi encontrado na rede não grava
		if ((paramEqpto->eqpto == NULL) && (paramEqpto->carga == NULL))
			continue;
		if (!paramEqpto->carga)
			continue;

		fprintf(fout, "%s;", paramEqpto->carga->Codigo.c_str());
		if (paramEqpto->rede != NULL)
			fprintf(fout, "%s;", paramEqpto->rede->Codigo.c_str());
		else
			fprintf(fout, "NLOC;");

		fprintf(fout, "%6.2f;", paramEqpto->ndiasObs);
		fprintf(fout, "%6.2f;", paramEqpto->nFalhasEquip);
		fprintf(fout, "%6.2f;", paramEqpto->durTotal);

		fprintf(fout, "%6.2f;", paramEqpto->lambda);
		fprintf(fout, "%6.2f\n", paramEqpto->trep);

	}

	fprintf(fout, "Parametros por Bloco\n");
	fprintf(fout, "iBloco;");
	fprintf(fout, "codRede;");
	fprintf(fout, "compBloco;");
	fprintf(fout, "lambda;");
	fprintf(fout, "trep\n");
	strDadoParamBloco *paramBloco;
	AnsiString aux;
	for (int n = 0; n < lisParamBloco->Count; n++)
	{
		paramBloco = (strDadoParamBloco*)lisParamBloco->Items[n];
		fprintf(fout, "%i;", n);
		aux = "";
		if (paramBloco->bloco->Rede != NULL)
			aux = paramBloco->bloco->Rede->Codigo;
		fprintf(fout, "%s;", aux.c_str());
		fprintf(fout, "%6.2f;", paramBloco->compBloco);
		fprintf(fout, "%6.2f;", paramBloco->lambda);
		fprintf(fout, "%6.2f\n", paramBloco->trep);
	}
	fclose(fout);
}

// ---------------------------------------------------------------------------
void __fastcall TParamConf::IniciaParamDefault(void)
{
	// parâmetros gerais
	// paramGerais->flagAutoSoc   = false;
	paramGerais->flagRecurso = true;
	paramGerais->flagRetorno = false;
	paramGerais->tlocaliza = 25;
	paramGerais->tman_manual = 10;
	paramGerais->tman_auto = 1;
	paramGerais->trep = 50; // hkdef
	paramGerais->lambda = 0.1; // hkdef
	paramGerais->pu_lim = 1.1;
	paramGerais->percDefTrans = 0.8;
	paramGerais->iProfReliga = 0; // novo
	paramGerais->tlimIC = 3;
   //hk201711: taxa de descoordenação de proteção
	paramGerais->taxa_descoord = -1.0;
	//gh201806: % reduçao média do tempo de preparo para instalação de IFs em fusíveis
	paramGerais->red_tprep_IF = 0.7;
   paramGerais->ptpre = paramGerais->ptdlo = paramGerais->ptexe = 0.0;//hk201805: inicialização: hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
	// limpa listas de parâmetros
	LimpaTList(lisParamBloco);
	LimpaTList(lisParamCarga);
	LimpaTList(lisParamRede);
	LimpaTList(lisParamEqpto);
	LimpaTList(lisVerifCjto);
	LimpaTList(lisVerifSe); // hkv201608 :LimpaTList(lisVerifSe)
	LimpaTList(lisVerifCircuito); // hkv201608 :LimpaTList(lisVerifCircuito)
	LimpaTList(lisChavesTelecom);
	LimpaTList(lisTipoEqProt);
	//gh201806: limpar lista de IFs
	LimpaTList(lisChavesIF);
	LimpaTList(lisEqptoIndisp); //hk201808: indisp
   eqptoIndispPadrao->cod_eqpto = "Indisp. Padrão";
   eqptoIndispPadrao->vindisp   = 0.5; //100%
   eqptoIndispPadrao->eqpto     = NULL;
	//gh201806: limpar lista de IFs
	LimpaTList(lisChavesIF);
	for (int n = 0; n < 24; n++)
		distFalhasPat[n] = 1 / 24;
	// hkv201608 : for(int n = 0; n < 24; n++) distFalhasPat[n] = 1/24;

   }

// ---------------------------------------------------------------------------
double __fastcall TParamConf::ComprimentoBloco(VTBloco *bloco)  //hk201808: reformula: tirar função
	// hk201702: ComprimentoBloco(VTBloco *bloco) para a inserção de novos paramBloco
{
	TList *lisLiga = bloco->LisLigacao();
	VTEqpto *pliga;
	double comp_km = 0.0;

	for (int n = 0; n < lisLiga->Count; n++)
	{
		pliga = (VTEqpto*)lisLiga->Items[n];
		if (pliga->Tipo() != eqptoTRECHO)
			continue;
		comp_km += ((VTTrecho*)pliga)->Comprimento_km;
	}
	return (comp_km);
}

//hk201808: reformula
//---------------------------------------------------------------------------
void __fastcall TParamConf::InsereLisTipoChaveProtecaoParamConf(TList *lisTipoEqProtExterno)
   {//hk201808: reformula
   //Popula lista paramConf->LisTipoEqProt
   strDadoTipoEqProt *dadoTipoEqProt;
   VTTipoChave       *tipoChave;
   //Atualiza lista tipo de chaves em paramConf
   LimpaTList(lisTipoEqProt);

   for(int n = 0; n < lisTipoEqProtExterno->Count; n++)
      {
      tipoChave = (VTTipoChave*)lisTipoEqProtExterno->Items[n];
      lisTipoEqProt->Add(dadoTipoEqProt = new strDadoTipoEqProt);
      dadoTipoEqProt->codTipoEqProt = tipoChave->Codigo;
      dadoTipoEqProt->tipoChave     = tipoChave;
      }

   }

//---------------------------------------------------------------------------
void __fastcall TParamConf::IniciaLisParamRede(TList *lisRede) //hk201808: reformula
   {
   //Funcao que adiciona paramRede em paramConf->LisParamRede
   //para redes até o momento não inseridas
   //Utilizada em LeArqMdb(AnsiString arqParam)
   //          em IniciaProblema(VTRede *rede)
   //VTRedes *redes   = (VTRedes*) apl->GetObject(__classid(VTRedes));
   VTRede  *rede;
   //TList   *lisRede = redes->LisRede();
   strDadoParamRede *paramRede;

   for(int n = 0; n < lisRede->Count; n++)
      {
      rede = (VTRede*)lisRede->Items[n];
		if (!rede->Carregada)
			continue;
      //Verifica se a rede corresponde a circuito primário, MT //hkv201608
      //hk201701: Inclusao de falhas em blocos em qualquer hierarquia de rede - MT, AT - antes: somente blocos de circuitos primários
      //if(!radial->ExistePrimario(rede)) continue;
		if ((paramRede = this->ParamRede[rede]) != NULL)
			continue;

      lisParamRede->Add(paramRede = new strDadoParamRede);
      paramRede->rede = rede;
      paramRede->compRede = ComprimentoRede(paramRede->rede);
      paramRede->codRede = rede->Codigo;
      //hkv201607: novas redes inicializadas com paramGerais
      paramRede->lambda  = paramGerais->lambda;//0.1;//0.0;
      paramRede->trep    = paramGerais->trep;//50.0;//0.0;
      //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
      paramRede->ptpre   = 0.0;
      paramRede->ptdlo   = 0.0;
      paramRede->ptexe   = 0.0;
      }

   }

//---------------------------------------------------------------------------
double __fastcall TParamConf::ComprimentoRede(VTRede *rede)//ssai
   {
   TList *lisLiga;
   VTEqpto *pliga;
   double    comp_km = 0.0;

	if (rede == NULL)
		return (0.0);

   lisLiga = rede->LisLigacao();

   for(int n = 0; n < lisLiga->Count; n++)
      {
      pliga = (VTEqpto*)lisLiga->Items[n];
		if (pliga->Tipo() != eqptoTRECHO)
			continue;
      comp_km += ((VTTrecho*)pliga)->Comprimento_km;
      }
   return(comp_km);
   }
//---------------------------------------------------------------------------
void __fastcall TParamConf::IniciaDistFalhas(int numPat)//hk201808: reformula: sai
   {
   //double *distFalhasPat = paramConf->DistFalhasPat;

	if (distFalhasPat != NULL)
		delete[]distFalhasPat;
   distFalhasPat = new double[numPat];
   for(int np = 0; np < numPat; np++)
                {
                distFalhasPat[np] = 1.0/numPat;
                }
   }
//---------------------------------------------------------------------------
void __fastcall TParamConf::RedistribuiFalhasHora(int numPat) //hk201808: reformula:
   {
   //double *distFalhasPat = paramConf->DistFalhasPat;

   double dTot = 0.0;

   for(int np = 0; np < numPat; np++)
      dTot += distFalhasPat[np];

   if(IsDoubleZero(dTot))
     {
     for(int np = 0; np < numPat; np++)
         distFalhasPat[np] = 1.0 / numPat;
     }
   else
     {
     for(int np = 0; np < numPat; np++)
         distFalhasPat[np] = distFalhasPat[np] / dTot;
     }
   }

//---------------------------------------------------------------------------
bool __fastcall TParamConf::GravaArqMdb(VTApl *apl, AnsiString arqParam) //hk201808: reformula:
   {
	VTBDadoParam* bdadoParam;

	bdadoParam = NewObjBDadoParam(apl);
	//Abre arquivo mdb, arqParam
   bdadoParam->DefineCaminho(arqParam);
   //Grava as informações em arqParam
   bdadoParam->GravaDado(this);
	delete bdadoParam;

   //Atualiza path de paramConf.mdb
   pathParam = ExtractFileDir(arqParam) + "\\";

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TParamConf::LeArqMdb(VTApl *apl, AnsiString arqParam)
   {
   VTBDadoParam *bdadoParam;

   //Atualiza path de paramConf.mdb
   pathParam = ExtractFileDir(arqParam) + "\\";

   //Cria objeto de leitura de paramConf.mdb
   bdadoParam = NewObjBDadoParam(apl);
   //Abre arquivo mdb definido, com path e nome: arqParam
   bdadoParam->DefineCaminho(arqParam);

   //Atualiza
   //paramConf->ParamGerais->flagRecurso, flagRetorno,tlocaliza,tman_manual,tman_auto,trep
   //                          lambda,pu_lim,percDefTrans,iProfReliga,tlimIC
   //--->LeDadoParamGerais();
   //Cria e preenche lisParamRede
   //param->codRede, param->lambda,param->trep
   //--->LeDadoParamRede();
   //Cria e preenche lisParamEqpto
   //param->codEqpto,codRede,tipoRede,ndiasObs,nf,nfr,td
   //param->nfmes[n=0,<12],nfrmes[n=0,<12],tdmes[n=0,<12]
   //param->nfhora[n=0,<24],nfrhora[n=0,<24]
   //--->LeDadoParamEqpto();
   //Cria e preenche lisParamBloco
   //param->codChave,lambda,trep
   //--->LeDadoParamBloco();
   //Cria e preenche lisParamCarga
   //param->codCarga,lambda,trep
   //--->LeDadoParamCarga();
   //Cria e preenche paramConf->LisVerifCjto
   //param->nomeSe,codSe,decVerif,fecVerif
   //--->LeDadoVerifCjto();
   //Cria e preenche paramConf->LisChavesTelecom
   //param->codChave
   //--->LeDadoChaveTelecom();
   //Cria e preenche paramConf->LisTipoEqProt
   //param->codTipoEqProt
   //     ->LeDadoTipoEqProt();

	//hkv201608 Reinicia parâmetros default e limpa listas
	//lisParamBloco;lisParamCarga;lisParamRede;lisParamEqpto;lisVerifCjto;lisChavesTelecom;lisTipoEqProt;
   IniciaParamDefault();

   //Preenche dados gerais e listas
   //1.inicializa parâmetros gerais
   //ParamGerais->flagRecurso,ParamGerais->flagRetorno,ParamGerais->tlocaliza,
   //ParamGerais->tman_manual,ParamGerais->tman_auto
   //ParamGerais->pu_lim,   ParamGerais->percDefTrans,   ParamGerais->iProfReliga
   //ParamGerais->tlimIC,   ParamGerais->lambda,   ParamGerais->trep
   //2.lê ParamRede, atualiza paramConf->LisParamRede param->codRede, param->lambda, param->trep
   //Cria e popula dados relativos a ocorrencias médias por circuito
   //codRede,lambda,trep
   //2.lê ParamRede, atualiza paramConf->LisParamRede param->codRede, param->lambda, param->trep
   //Cria e popula dados relativos a ocorrencias médias por circuito
   //codRede,lambda,trep
   //3.lê ParamEqpto: limpa e atualiza paramConf->LisParamEqpto, para as redes de lisCodRede
   //Cria e popula dados relativos a equipamentos atuados na rede
   //codEqpto,codRede,tipoRede,
   //ndiasObs,nf,nfr,td,cli,clih,
   //nfmes[0-12], nfrmes[0-12],tdmes[0-12],climes[0-12],clihmes[0-12],
   //nfhora[0-24],nfrhora[0-24]
   //4.lê ParamBloco: limpa e atualiza paramConf->LisParamBloco
   //Cria e popula dados ajustados relativos a blocos, identiicados por código da chave de suprimento
   //codChave,lambda,trep
   //5.lê ParamCarga: limpa e atualiza paramConf->LisParamCarga
   //Cria e popula dados ajustados relativos a cargas, identiicados por código do trafo de rede MT ou cargaA4
   //codCarga,lambda,trep
   //6lê VerifCircuito : limpa e atualiza paramConf->LisVerifCircuito->VTVerifCircuito->(CodCirc, decVerif, fecVerif)
   //7.lê VerifSe       : limpa e paramConf->LisVerifSe->VTVerifSe->(CodCirc, decVerif, fecVerif)
   //8.lê VerifCjto     : limpa e paramConf->LisVerifCjto->VTVerifCjto->(CodCjto, decVerif, fecVerif) e popula verifCjto->LisVerifSe
   //9.lê ChaveTelecom  : limpa e atualiza paramConf->LisChavesTelecom
   //10.lê TipoEqProt    : limpa e atualiza paramConf->LisTipoEqProt

   //Define lista de códigos de redes carregadas
   TStringList *lisCodRede;
   VTRedes     *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));
   VTRede      *rede;
   TList       *lisRede;
   lisCodRede = new TStringList();
   lisRede = redes->LisRede();
   for(int n = 0; n < lisRede->Count; n++)
      {
      rede = (VTRede*)lisRede->Items[n];
		if (!rede->Carregada)
			continue;
      //Verifica se a rede corresponde a circuito primário, MT //hkv201608
      //hk201701: Inclusao de falhas em blocos em qualquer hierarquia de rede - MT, AT - antes: somente blocos de circuitos primários
      //if(!radial->ExistePrimario(rede)) continue;
      lisCodRede->Add(rede->Codigo);
      }
   //Popula em memória paramConf para as redes carregadas
   //a partir das tabelas de paramconf.mdb
   bdadoParam->LeDado(this, lisCodRede);

   delete lisCodRede;
   delete bdadoParam;
   return(true);
   }
//---------------------------------------------------------------------------
void __fastcall TParamConf::GravaDirConf(VTApl *apl)//hk201808: reformula
   {
   VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arq;
   FILE       *fmed;
   //Grava arquivo com diretório para o próximo acesso
   arq = path->DirTmp() + "\\dirConf.tmp";
   fmed = fopen(arq.c_str(), "wt");
   fprintf(fmed, "%s", pathParam.c_str());
   fclose(fmed);
   }

//---------------------------------------------------------------------------
strDadoParamEqpto* __fastcall TParamConf::ParametroDadoEqpto(VTChave *chave) //hk201808: reformula
   {
   TList *lisParamEqpto = lisParamEqpto;
   strDadoParamEqpto *paramEqpto;
   //busca paramEqpto para o caso de rede BT representada por carga equivalente
   //do trafo de distribuição e da rede BT
   paramEqpto = NULL;
   for(int np = 0; np < lisParamEqpto->Count; np++)
      {
      paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[np];
		if (paramEqpto->eqpto == (VTEqpto*)chave)
			break;
	  paramEqpto = NULL;
      }
   return(paramEqpto);
   }
//---------------------------------------------------------------------------
strDadoParamEqpto* __fastcall TParamConf::ParametroDadoEqpto(AnsiString codChave,
	AnsiString codRede)
   {
   TList *lisParamEqpto = lisParamEqpto;
   strDadoParamEqpto *paramEqpto;
   //busca paramEqpto
   paramEqpto = NULL;
   for(int np = 0; np < lisParamEqpto->Count; np++)
      {
      paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[np];
		if ((paramEqpto->codEqpto.AnsiCompare(codChave) == 0) && (paramEqpto->codRede.AnsiCompare
			(codRede) == 0))
		{
			break;
		}
      paramEqpto = NULL;
      }
   return(paramEqpto);
   }

//---------------------------------------------------------------------------
strDadoParamEqpto* __fastcall TParamConf::ParametroDadoEqpto(VTCarga *carga)
   {
   TList *lisParamEqpto = lisParamEqpto;
   strDadoParamEqpto *paramEqpto;

   if(carga->Obj != NULL)
	{
		paramEqpto = (strDadoParamEqpto*)carga->Obj;
      return(paramEqpto);
     }

   //busca paramEqpto para o caso de rede BT representada por carga equivalente
   //do trafo de distribuição e da rede BT
   paramEqpto = NULL;
   for(int np = 0; np < lisParamEqpto->Count; np++)
      {
      paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[np];
		if (paramEqpto->carga == carga)
			break;
      paramEqpto = NULL;
      }
   return(paramEqpto);
   }

//---------------------------------------------------------------------------
bool __fastcall TParamConf::LeArquivosMdb(VTApl* apl, AnsiString pathParam)
   {
   AnsiString arqParam, arqCircVerif, arqOcorrVerif;
   bool       resposta = true;

   if(pathParam.AnsiCompare("") == 0)
	{

		this->pathParam = LePathParam(apl);
	} // leitura de tmp\dirConf.tmp
   else
	{
		this->pathParam = pathParam;
     }
   //Leitura de dados gerais
   arqParam = this->pathParam + "paramConf.mdb";

   //hk201808: reformula
   if (!LeArqMdb(apl, arqParam)) resposta = false;

	//if (!LeArqMdb(arqParam))
	//	resposta = false;

   return(resposta);
   }
//---------------------------------------------------------------------------
AnsiString __fastcall TParamConf::LePathParam(VTApl* apl)
   {
   VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arqParam, dirConf;
   FILE    *fparam;
   bool    flagEOF;

   arqParam = path->DirTmp() + "\\dirConf.tmp";
	if (!FileExists(arqParam))
		return (path->DirDat() + "\\Confiabilidade\\"); // return(path->DirTmp() + "\\");

   fparam = fopen(arqParam.c_str(), "r");
   dirConf = "";
   //dirConf = path->DirDat();
   dirConf.cat_sprintf("%s", LeDado(fparam, flagEOF).c_str());
   fclose(fparam);

	if (!DirectoryExists(dirConf))
		dirConf = path->DirDat() + "\\Confiabilidade\\";
   //if(!DirectoryExists(dirConf)) dirConf = path->DirTmp() + "\\";
   return(dirConf);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TParamConf::LeDado(FILE* fp, bool &flagEOF)
   {
   AnsiString x;
   char  aux;

   flagEOF = false;

   x = "";
   aux = fgetc(fp);
   while ((aux  != ';') && (aux  != '\n') && (aux  != EOF))
	{
		x += AnsiString(aux);
                  aux = fgetc(fp);
                 }
	if (aux == EOF)
		flagEOF = true;

   return(x);
   }

// ---------------------------------------------------------------------------
// eof
