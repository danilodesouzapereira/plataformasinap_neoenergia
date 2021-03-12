// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TBDadoParam.h"
#include <IniFiles.hpp>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Log\VTLog.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>

#include "..\Dados\VTParamConf.h"
#include "..\Dados\VTVerifCircuito.h"
#include "..\Dados\VTVerifCjto.h"
#include "..\Dados\VTVerifSe.h"
#include "..\Modela\VTGeraLisParamEqpto.h"
#include <PlataformaSinap\DLL_Inc\Funcao.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTBDadoParam* __fastcall NewObjBDadoParam(VTApl *apl_owner)
{
	try
	{ // cria objeto
		return (new TBDadoParam(apl_owner));
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TBDadoParam::TBDadoParam(VTApl *apl_owner)
{
	CoInitialize(NULL);
	// salva ponteiro p/ objeto
	this->apl = apl_owner;
   paramConf = NULL;
}

// ---------------------------------------------------------------------------
__fastcall TBDadoParam::~TBDadoParam(void)
{
	if (dcom)
	{
		delete dcom;
		dcom = NULL;
	}
	// fecha conexao se ainda aberta
	if (aberto)
		Fecha();
	if (conn)
	{
		delete conn;
		conn = NULL;
	}
	// destroi objetos
	// delete paramConf; paramConf = NULL;
	// destroi listas
	// hk201608 if(lisTMP) {delete lisTMP; lisTMP = NULL;}
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::Aberto(void)
{
	return (aberto);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/15456154
bool __fastcall TBDadoParam::Abre(void)
{
    // variáveis locais
    AnsiString conStr;
    AnsiString base = dbPath;
//    AnsiString senha;

    try
    {
        if (dcom)
        {
            delete dcom;
            dcom = NULL;
        }
        Fecha();
        if (conn)
        {
            delete conn;
            conn = NULL;
        }
        aberto = false;

        // cria e abre conexão com a base ACCESS
//        senha = "";
//        conStr = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + base +
//                 ";Jet OLEDB:Database Password=" + senha;
        conStr = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + base +
            ";Jet OLEDB:Database";
        conn = CriaObjetoConexao(conStr);
        dcom = new TADOCommand(NULL);
        dcom->Connection = conn;
    }
    catch (...)
    {
        aberto = false;
        return false;
    }

    aberto = true;
    return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::AtualizaDadoParamGerais(sdi_ParamGerais &sdi)
{
	// inicializa parâmetros
	paramConf->ParamGerais->flagRecurso = sdi.flagRecurso; // 1
	paramConf->ParamGerais->flagRetorno = sdi.flagRetorno; // 2
	paramConf->ParamGerais->tlocaliza = sdi.tlocaliza; // 3
	paramConf->ParamGerais->tman_manual = sdi.tman_manual; // 4
	paramConf->ParamGerais->tman_auto = sdi.tman_auto; // 5
	// hkdef paramConf->ParamGerais->trrepDefault   = sdi.trepDefault;
	// hkdef paramConf->ParamGerais->llambdaDefault = sdi.lambdaDefault;
	paramConf->ParamGerais->pu_lim = sdi.pu_lim; // 6
	paramConf->ParamGerais->percDefTrans = sdi.percDefTrans; // 7
	paramConf->ParamGerais->iProfReliga = sdi.iProfReliga; // 8
	paramConf->ParamGerais->tlimIC = sdi.tlimIC; // 9

	// paramConf->ParamGerais->lambda = (IsDoubleZero(sdi.lambda) ? 0.1 : sdi.lambda); //10
	paramConf->ParamGerais->lambda = sdi.lambda; // 10
	paramConf->ParamGerais->trep = sdi.trep; // 11
	// gh201806: acréscimo de parâmetro de porcentagem de redução de tempo de preparo por alocação de IF
	paramConf->ParamGerais->red_tprep_IF = sdi.red_tprep_IF; // 12
	// hk201808: reformulação: faltou o indicador geral taxa_descoord

	paramConf->EqptoIndispPadrao->vindisp = sdi.vindisp; // hk201808: indisp
	return (true);
}

// ---------------------------------------------------------------------------
strDadoChaveIF* __fastcall TBDadoParam::CriaDadoChaveIF(sdi_ChaveIF &sdi, TList *lisEXT)
{
	// variáveis locais
	strDadoChaveIF *param;

	// cria objeto
	param = new strDadoChaveIF;
	// inicializa parâmetros
	param->codChave = sdi.codChave;
	// insere objeto na lista
	lisEXT->Add(param);
	// retorna objeto
	return (param);
}

// ---------------------------------------------------------------------------
strDadoChaveTelecom* __fastcall TBDadoParam::CriaDadoChaveTelecom(sdi_ChaveTelecom &sdi,
	TList *lisEXT)
{
	// variáveis locais
	strDadoChaveTelecom *param;

	// cria objeto
	param = new strDadoChaveTelecom;
	// inicializa parâmetros
	param->codChave = sdi.codChave;
	// insere objeto na lista
	lisEXT->Add(param);
	// retorna objeto
	return (param);
}

// ---------------------------------------------------------------------------
strDadoParamBloco* __fastcall TBDadoParam::CriaDadoParamBloco(sdi_ParamBloco &sdi, TList *lisEXT)
{
	// variáveis locais
	strDadoParamBloco *param;

	// cria objeto
	param = new strDadoParamBloco;
	// inicializa parâmetros
	param->codChave = sdi.codChave;
	param->lambda = sdi.lambda;
	param->trep = sdi.trep;
	// hk201710IF
	param->ptpre = sdi.ptpre;
	param->ptdlo = sdi.ptdlo;
	param->ptexe = sdi.ptexe;

	param->codBarra = sdi.codBarraChave;
	// código da barra de referência para definição do bloco hk201802a

	param->fatTMA = 1.0; // hk201703: adaptações para planTurmas: acrescimo de fator tma
	param->lisObrasBloco = NULL;
	// insere objeto na lista
	lisEXT->Add(param);
	// retorna objeto
	return (param);
}

// ---------------------------------------------------------------------------
strDadoParamCarga* __fastcall TBDadoParam::CriaDadoParamCarga(sdi_ParamCarga &sdi, TList *lisEXT)
{
	// variáveis locais
	strDadoParamCarga *param;

	// cria objeto
	param = new strDadoParamCarga;
	// inicializa parâmetros
	param->codCarga = sdi.codCarga;
	param->lambda = sdi.lambda;
	param->trep = sdi.trep;
	// hk201710IF
	param->ptpre = sdi.ptpre;
	param->ptdlo = sdi.ptdlo;
	param->ptexe = sdi.ptexe;

	param->fatTMA = 1.0; // hk201703: adaptações para planTurmas: acrescimo de fator tma
	lisEXT->Add(param);
	// retorna objeto
	return (param);
}

// ---------------------------------------------------------------------------
strDadoParamEqpto* __fastcall TBDadoParam::CriaDadoParamEqpto(sdi_ParamEqpto &sdi, TList *lisEXT)
{
	// Cria e popula dados relativos a equipamentos atuados na rede
	// codEqpto,codRede,tipoRede,
	// ndiasObs,nf,nfr,td,cli,clih,
	// nfmes[0-12], nfrmes[0-12],tdmes[0-12],climes[0-12],clihmes[0-12],
	// nfhora[0-24],nfrhora[0-24]

	// variáveis locais
	strDadoParamEqpto *param;

	// cria objeto
	param = new strDadoParamEqpto;

	// inicializa parâmetros
	param->codEqpto = sdi.codEqpto;
	param->codRede = sdi.codRede;
	param->tipoRede = sdi.tipoRede;
	param->ndiasObs = sdi.ndiasObs;
	// hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
	param->ptpre = sdi.ptpre;
	param->ptdlo = sdi.ptdlo;
	param->ptexe = sdi.ptexe;

	param->nf = sdi.nf;
	param->nfr = sdi.nfr;
	param->td = sdi.td;
	param->cli = sdi.cli;
	param->clih = sdi.clih;
	// meses
	for (int n = 0; n < 12; n++)
	{
		param->nfmes[n] = sdi.nfMes[n];
		param->nfrmes[n] = sdi.nfrMes[n];
		param->tdmes[n] = sdi.tdMes[n];
		param->climes[n] = sdi.climes[n];
		param->clihmes[n] = sdi.clihmes[n];
	}
	// horas
	for (int n = 0; n < 24; n++)
	{
		param->nfhora[n] = sdi.nfHora[n];
		param->nfrhora[n] = sdi.nfrHora[n];
	}

	// insere objeto na lista
	lisEXT->Add(param);
	// retorna objeto
	return (param);
}

// ---------------------------------------------------------------------------
strDadoParamRede* __fastcall TBDadoParam::CriaDadoParamRede(sdi_ParamRede &sdi, TList *lisEXT)
{
	// variáveis locais
	strDadoParamRede *param;

	param = NULL;
	for (int n = 0; n < lisEXT->Count; n++)
	{
		param = (strDadoParamRede*)lisEXT->Items[n];
		if (param->codRede.AnsiCompare(sdi.codRede) == 0)
			break;
		param = NULL;
	}

	if (param == NULL)
	{ // cria objeto
		param = new strDadoParamRede;
		// insere objeto na lista
		lisEXT->Add(param);
	}
	// inicializa parâmetros
	param->codRede = sdi.codRede;
	param->lambda = sdi.lambda;
	param->trep = sdi.trep;
	// hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
	param->ptpre = 0.0;
	param->ptdlo = 0.0;
	param->ptexe = 0.0;
	// retorna objeto
	return (param);
}

// ---------------------------------------------------------------------------
strDadoTipoEqProt* __fastcall TBDadoParam::CriaDadoTipoEqProt(sdi_TipoEqProt &sdi, TList *lisEXT)
{
	// variáveis locais
	strDadoTipoEqProt *param;

	// cria objeto
	param = new strDadoTipoEqProt;
	// inicializa parâmetros
	param->codTipoEqProt = sdi.codTipoEqProt;
	// insere objeto na lista
	lisEXT->Add(param);
	// retorna objeto
	return (param);
}

// ---------------------------------------------------------------------------
VTVerifCircuito* __fastcall TBDadoParam::CriaDadoVerifCircuito(sdi_VerifCircuito &sdi,
	TList *lisEXT)
{
	// variáveis locais
	VTVerifCircuito *circ;

	// cria objeto
	circ = NewObjVerifCircuito();
	// inicializa parâmetros
	circ->Codigo = sdi.codCircuito;
	circ->DecVerif = sdi.decVerif;
	circ->FecVerif = sdi.fecVerif;
	// insere objeto na lista
	lisEXT->Add(circ);
	// retorna objeto
	return (circ);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/15456154
VTVerifCjto* __fastcall TBDadoParam::CriaDadoVerifCjto(sdi_VerifCjto &sdi, TList *lisEXT)
{
	// variáveis locais
	VTVerifCjto *cjto;

	// cria objeto
	cjto = NewObjVerifCjto();
	// inicializa parâmetros
	cjto->Codigo = sdi.codCjto;
	cjto->DecVerif = sdi.decVerif;
	cjto->FecVerif = sdi.fecVerif;
    cjto->DecAjust = -1.0;
    cjto->FecAjust = -1.0;
	// insere objeto na lista
	lisEXT->Add(cjto);
	// retorna objeto
	return (cjto);
}

// ---------------------------------------------------------------------------
VTVerifSe* __fastcall TBDadoParam::CriaDadoVerifSe(sdi_VerifSe &sdi, TList *lisEXT)
{
	// variáveis locais
	VTVerifSe *se;

	// cria objeto
	se = NewObjVerifSe();
	// inicializa parâmetros
	se->Codigo = sdi.codSe;
	se->DecVerif = sdi.decVerif;
	se->FecVerif = sdi.fecVerif;
	// insere objeto na lista
	lisEXT->Add(se);
	// retorna objeto
	return (se);
}

// ---------------------------------------------------------------------------
TADOConnection* __fastcall TBDadoParam::CriaObjetoConexao(AnsiString conStr)
{
	// variáveis locais
	TADOConnection *con;
	// cria e abre objeto conexão
	con = new TADOConnection(NULL);
	con->Connected = false;
	con->ConnectionString = conStr;
	// con->CursorLocation   = clUseServer;
	con->CursorLocation = clUseClient;
	con->LoginPrompt = false;
	// con->Mode             = modo;
	con->Connected = true;
	return con;
}

// ---------------------------------------------------------------------------
TADOQuery* __fastcall TBDadoParam::CriaObjetoQuery(TCursorType tipo)
{
	// variáveis locais
	TADOQuery *rs;

	// proteção
	if (!conn)
		return NULL;

	// cria objeto query
	rs = new TADOQuery(NULL);
	rs->Connection = conn;
	rs->CursorType = tipo;
	// rs->LockType   = ltBatchOptimistic;
	rs->LockType = ltReadOnly;
	rs->Prepared = true;
	// retorna objeto query
	return rs;
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::CriaTabelaChavesTelecom(void)
{
	// variáveis locais
	bool resposta = false;
	AnsiString qryTxt;

	// deleta tabela
	DeletaTabela("ChavesTelecom");

	try
	{ // monta sql
		qryTxt.sprintf("CREATE TABLE ChavesTelecom (");
		qryTxt.cat_sprintf("codChave TEXT(255)");
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::CriaTabelaChavesIF(void)
{
	// variáveis locais
	bool resposta = false;
	AnsiString qryTxt;

	// deleta tabela
	DeletaTabela("ChavesIF");

	try
	{ // monta sql
		qryTxt.sprintf("CREATE TABLE ChavesIF (");
		qryTxt.cat_sprintf("codChave TEXT(255)");
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::CriaTabelaParamBloco(void)
{
	// variáveis locais
	bool resposta = false;
	AnsiString qryTxt;

	// deleta tabela
	// hkv201610 DeletaTabela("ParamBloco"); : CriaTabelaParamBloco(void)

	try
	{ // monta sql
		qryTxt.sprintf("CREATE TABLE ParamBloco (");
		qryTxt.cat_sprintf("codChave TEXT(255), ");
		qryTxt.cat_sprintf("codBarraChave TEXT(255), "); // hk201802a
		qryTxt.cat_sprintf("codRede TEXT(255), ");
		// hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede : CriaTabelaParamBloco(void)
		qryTxt.cat_sprintf("lambda FLOAT, ");
		qryTxt.cat_sprintf("trep FLOAT, ");
		// hk201710IF: Acréscimo de ptpre, ptdlo, ptexe
		qryTxt.cat_sprintf("ptpre FLOAT, ");
		qryTxt.cat_sprintf("ptdlo FLOAT, ");
		qryTxt.cat_sprintf("ptexe FLOAT");
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::CriaTabelaParamCarga(void)
{
	// variáveis locais
	bool resposta = false;
	AnsiString qryTxt;

	// deleta tabela
	// hkv201610 DeletaTabela("ParamCarga"); : CriaTabelaParamCarga(void)

	try
	{ // monta sql
		qryTxt.sprintf("CREATE TABLE ParamCarga (");
		qryTxt.cat_sprintf("codCarga TEXT(255), ");
		qryTxt.cat_sprintf("codRede TEXT(255), ");
		// hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede : CriaTabelaParamCarga(void)
		qryTxt.cat_sprintf("lambda FLOAT, ");
		qryTxt.cat_sprintf("trep FLOAT, ");
		// hk201710IF: Acréscimo de ptpre, ptdlo, ptexe
		qryTxt.cat_sprintf("ptpre FLOAT, ");
		qryTxt.cat_sprintf("ptdlo FLOAT, ");
		qryTxt.cat_sprintf("ptexe FLOAT");
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::CriaTabelaParamEqpto(void)
{
	// variáveis locais
	bool resposta = false;
	AnsiString qryTxt;
	AnsiString aux;

	// deleta tabela
	DeletaTabela("ParamEqpto");

	try
	{ // monta sql
		qryTxt.sprintf("CREATE TABLE ParamEqpto (");
		qryTxt.cat_sprintf("codEqpto TEXT(255), ");
		qryTxt.cat_sprintf("codRede TEXT(255), ");
		qryTxt.cat_sprintf("tipoRede TEXT(255), ");
		qryTxt.cat_sprintf("ndiasObs FLOAT, ");
		qryTxt.cat_sprintf("ptpre FLOAT, ");
		// hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
		qryTxt.cat_sprintf("ptdlo FLOAT, ");
		qryTxt.cat_sprintf("ptexe FLOAT, ");
		qryTxt.cat_sprintf("nf FLOAT, ");
		qryTxt.cat_sprintf("nfr FLOAT, ");
		qryTxt.cat_sprintf("td FLOAT, ");
		qryTxt.cat_sprintf("cli FLOAT, "); // hkmar
		qryTxt.cat_sprintf("clih FLOAT, "); // hkmar
		// meses
		for (int n = 0; n < 12; n++)
		{
			aux = "nfMes" + IntToStr(n + 1) + " FLOAT, ";
			qryTxt.cat_sprintf(aux.c_str());
			aux = "nfrMes" + IntToStr(n + 1) + " FLOAT, ";
			qryTxt.cat_sprintf(aux.c_str());
			aux = "tdMes" + IntToStr(n + 1) + " FLOAT, ";
			qryTxt.cat_sprintf(aux.c_str());
			aux = "cliMes" + IntToStr(n + 1) + " FLOAT, "; // hkmar
			qryTxt.cat_sprintf(aux.c_str());
			aux = "clihMes" + IntToStr(n + 1) + " FLOAT, "; // hkmar
			qryTxt.cat_sprintf(aux.c_str());
		}
		// horas
		for (int n = 0; n < 24; n++)
		{
			aux = "nfHora" + IntToStr(n) + " FLOAT, ";
			qryTxt.cat_sprintf(aux.c_str());
			if (n < 23)
				aux = "nfrHora" + IntToStr(n) + " FLOAT, ";
			else
				aux = "nfrHora" + IntToStr(n) + " FLOAT";
			qryTxt.cat_sprintf(aux.c_str());
		}

		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::CriaTabelaParamGerais(void)
{
	// variáveis locais
	bool resposta = false;
	AnsiString qryTxt;

	// delete tabela
	DeletaTabela("ParamGerais");

	try
	{ // monta sql
		qryTxt.sprintf("CREATE TABLE ParamGerais (");
		qryTxt.cat_sprintf("flagRecurso INTEGER, ");
		qryTxt.cat_sprintf("flagRetorno INTEGER, ");
		qryTxt.cat_sprintf("tlocaliza FLOAT, ");
		qryTxt.cat_sprintf("tman_manual FLOAT, ");
		qryTxt.cat_sprintf("tman_auto FLOAT, ");
		qryTxt.cat_sprintf("pu_lim FLOAT, ");
		qryTxt.cat_sprintf("percDefTrans FLOAT, ");
		qryTxt.cat_sprintf("iProfReliga INTEGER, ");
		qryTxt.cat_sprintf("tlimIC FLOAT, ");
		qryTxt.cat_sprintf("lambda FLOAT, ");
		qryTxt.cat_sprintf("trep FLOAT, ");
		qryTxt.cat_sprintf("red_tprep_IF FLOAT");
		// gh201806: % reduçao média do tempo de preparo para instalação de IFs em fusíveis
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::CriaTabelaParamRede(void)
{
	// variáveis locais
	bool resposta = false;
	AnsiString qryTxt;

	// delete tabela
	DeletaTabela("ParamRede");

	try
	{ // monta sql
		qryTxt.sprintf("CREATE TABLE ParamRede (");
		qryTxt.cat_sprintf("codRede TEXT(255), ");
		qryTxt.cat_sprintf("lambda FLOAT, ");
		qryTxt.cat_sprintf("trep FLOAT");
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::CriaTabelaTipoEqProt(void)
{
	// variáveis locais
	bool resposta = false;
	AnsiString qryTxt;

	// deleta tabela
	DeletaTabela("TipoEqProt");

	try
	{ // monta sql
		qryTxt.sprintf("CREATE TABLE TipoEqProt (");
		qryTxt.cat_sprintf("codTipoEqProt TEXT(255)");
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::CriaTabelaVerifCircuito(void)
{
	// variáveis locais
	bool resposta = false;
	AnsiString qryTxt;

	// delete tabela
	DeletaTabela("VerifCircuito");

	try
	{ // monta sql
		qryTxt.sprintf("CREATE TABLE VerifCircuito(");
		qryTxt.cat_sprintf("codCircuito TEXT(255), ");
		qryTxt.cat_sprintf("decVerif FLOAT, ");
		qryTxt.cat_sprintf("fecVerif FLOAT");
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::CriaTabelaVerifCjto(void)
{
	// variáveis locais
	bool resposta = false;
	AnsiString qryTxt;

	// delete tabela
	DeletaTabela("VerifCjto");

	try
	{ // monta sql
		qryTxt.sprintf("CREATE TABLE VerifCjto(");
		qryTxt.cat_sprintf("codCjto TEXT(255), ");
		qryTxt.cat_sprintf("decVerif FLOAT, ");
		qryTxt.cat_sprintf("fecVerif FLOAT");
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::CriaTabelaVerifCjtoSe(void)
{
	// variáveis locais
	bool resposta = false;
	AnsiString qryTxt;

	// delete tabela
	DeletaTabela("VerifCjtoSe");

	try
	{ // monta sql
		qryTxt.sprintf("CREATE TABLE VerifCjtoSe(");
		qryTxt.cat_sprintf("codCjto TEXT(255), ");
		qryTxt.cat_sprintf("codSe TEXT(255)");
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::CriaTabelaVerifSe(void)
{
	// variáveis locais
	bool resposta = false;
	AnsiString qryTxt;

	// delete tabela
	DeletaTabela("VerifSe");

	try
	{ // monta sql
		qryTxt.sprintf("CREATE TABLE VerifSe(");
		qryTxt.cat_sprintf("codSe TEXT(255), ");
		qryTxt.cat_sprintf("decVerif FLOAT, ");
		qryTxt.cat_sprintf("fecVerif FLOAT");
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::DeletaTabela(AnsiString tabela)
{
	// variáveis locais
	bool resposta = false;
	AnsiString qryTxt;

	// verifica se existe tabela
	if (ExisteTabela(tabela))
	{
		try
		{ // monta sql
			qryTxt.sprintf("DROP TABLE ");
			qryTxt.cat_sprintf(tabela.c_str());
			qryTxt.cat_sprintf(";");
			// executa
			dcom->CommandText = qryTxt;
			dcom->Execute();
			resposta = true;
		}
		catch (...)
		{
			resposta = false;
		}
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
/*
 bool __fastcall TBDadoParam::DeletaTabelas(void)
 {
 //variáveis locais
 bool         resposta = true;
 TStringList* ptrTableList = NULL;
 AnsiString   tabela;

 try{//cria lista
 ptrTableList = new TStringList();
 //lista de tabelas da base destino
 dcom->Connection->GetTableNames(ptrTableList, false);
 //lista de tabelas
 for (int n = 0; n < ptrTableList->Count; n++)
 {//código da tabela
 tabela = ptrTableList->Strings[n];
 //deleta tabela
 DeletaTabela(tabela);
 }
 //deleta objeto
 delete ptrTableList;
 resposta = true;
 }
 catch(...)
 {
 resposta = false;
 }

 return(resposta);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TBDadoParam::DefineCaminho(AnsiString caminho)
{
	AnsiString aux = caminho;
	// Usa caminho da aplicacao
	this->dbPath = caminho;
	// verifica se o banco foi aberto em dbPath
	if (!Abre()) // Abre(void) em dbPath
	{
		// aux.sprintf("Erro ao abrir Banco de Dados\n");
		// aux.sprintf("Configurar caminho da base de dados");
		aux.sprintf("Erro ao abrir Banco de Dados!");
		// aux.cat_sprintf("(%s)", this->dbPath);
		Aviso(aux);
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/15456154
void __fastcall TBDadoParam::DefineDirBase(AnsiString dir)
{
    // fecha conexao
//    if (aberto)
//    {
        if (dcom)
        {
            delete dcom;
            dcom = NULL;
        }
        Fecha();
        if (conn)
        {
            delete conn;
            conn = NULL;
        }
        // LimpaTList(lisDADO);
        aberto = false;
//    }
    // atualiza camino
    dirBase = dir;
    // lê dados   //return(FileExists(dirBase));
    if (ValidaDirBase()) // Verifica se existe o arquivo dirBase, FileExists(dirBase)
    { // this->dbPath = caminho = dirBase
        // Abre(void) em dbPath
        DefineCaminho(dirBase);
        // LeBanco();
    }
    else
    {
        // Aviso("Configurar caminho da base de dados");
    }
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TBDadoParam::DirBase(void)
{
    return (dirBase);
}

// ---------------------------------------------------------------------------
void __fastcall TBDadoParam::EncerraQuery(TADOQuery *qry)
{
	qry->Close();
	delete qry;
}

// ---------------------------------------------------------------------------
TADOQuery* __fastcall TBDadoParam::ExecutaQuery(AnsiString comSql, bool escrita)
{
	// variáveis locais
	TADOQuery *rs;
	VTLog *log = (VTLog*)apl->GetObject(__classid(VTLog));

	try
	{ // log
		if (log)
		{
			log->Add("Consulta: " + comSql);
		}
		// cria objeto tquery
		if (escrita)
		{
			if ((rs = CriaObjetoQuery(ctStatic)) == NULL)
				return (NULL);
		}
		else
		{
			if ((rs = CriaObjetoQuery(ctOpenForwardOnly)) == NULL)
				return (NULL);
		}
		// executa SQL
		rs->Close();
		rs->SQL->Clear();
		rs->SQL->Add(comSql);
		rs->Open();
		rs->Connection = NULL;
		// desabilita controles - HOLY GRAIL!!!
		rs->DisableControls();
	}
	catch (Exception &e)
	{
		if (log)
		{
			log->Add("Erro ao executar consulta: " + comSql);
			log->Add("Erro detectado: " + e.Message);
		}
		return NULL;
	}
	// retorna objeto
	return rs;
}

// ---------------------------------------------------------------------------
// VTRede* __fastcall TBDadoParam::ExisteRede(AnsiString codigo)
// {
// variáveis locais
// VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

// proteção
// if (!redes) return(NULL);
// verifica existência de rede pelo código
// return(redes->ExisteRede(codigo));
// }

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::ExisteCampoTabela(AnsiString tabela, AnsiString campo)
{
	// variáveis locais
	TStringList* ptrFieldList = NULL;
	AnsiString nome;
	bool resposta = false;

	try
	{ // cria lista
		ptrFieldList = new TStringList();
		// lista de campos da tabela
		dcom->Connection->GetFieldNames(tabela, ptrFieldList);
		// lista de tabelas
		for (int n = 0; n < ptrFieldList->Count; n++)
		{ // nome da tabela
			nome = ptrFieldList->Strings[n];
			// existe tabela?
			if (nome.AnsiCompareIC(campo) == 0)
			{
				resposta = true;
				break;
			}
		}
		// deleta objeto
		delete ptrFieldList;
		return (resposta);
	}
	catch (...)
	{
		return (false);
	}

}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::ExisteTabela(AnsiString tabela)
{
	// variáveis locais
	TStringList* ptrTableList = NULL;
	AnsiString nome;
	bool resposta = false;

	try
	{ // cria lista
		ptrTableList = new TStringList();
		// lista de tabelas da base destino
		dcom->Connection->GetTableNames(ptrTableList, false);
		// lista de tabelas
		for (int n = 0; n < ptrTableList->Count; n++)
		{ // nome da tabela
			nome = ptrTableList->Strings[n];
			// existe tabela?
			if (nome.AnsiCompareIC(tabela) == 0)
			{
				resposta = true;
				break;
			}
		}
		// deleta objeto
		delete ptrTableList;
		return (resposta);
	}
	catch (...)
	{
		return (false);
	}

}

// ---------------------------------------------------------------------------
VTVerifSe* __fastcall TBDadoParam::ExisteVerifSe(AnsiString codigo)
{
	// variáveis locais
	VTVerifSe *verifSe;

	for (int n = 0; n < paramConf->LisVerifSe->Count; n++)
	{
		verifSe = (VTVerifSe*)paramConf->LisVerifSe->Items[n];
		if (verifSe->Codigo.AnsiCompareIC(codigo) == 0)
			return (verifSe);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::Fecha(void)
{
	// fecha conexão com a base ACCESS
	try
	{
		if (conn)
			conn->Close();
	}
	catch (...)
	{
		return false;
	}
	return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/15456154
void __fastcall TBDadoParam::SetParamConf(VTParamConf *paramConf)
{
    this->paramConf = paramConf;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/15456154
bool __fastcall TBDadoParam::GravaDado(VTParamConf *paramConf)
{
	/* dados são gravados a partir de um objeto paramConf já pronto */
	// ponteiro para objeto externo
//  this->paramConf = paramConf;
    SetParamConf(paramConf);
	// grava dados - ParamGerais: CriaTabela e GravaDado
	GravaDadoParamGerais();
	// grava dados ParamRede na tabela ParamRede: CriaTabela e GravaDado
	GravaDadoParamRede();
	// grava dados - ParamEqpto
	// hkgrava gravado somente pelo extrator?? GravaDadoParamEqpto();
	// grava dados - chave telecomandadas: CriaTabela e GravaDado
	GravaDadoChavesTelecom();
	// grava dados - chaves de protecao: CriaTabela e GravaDado
	GravaDadoTipoEqProt();
	// grava dados - VerifCjto
	GravaDadoVerifCjto();
	// grava dados - VerifSe
	GravaDadoVerifSe();
	// grava dados - VerifCjtoSe
	GravaDadoVerifCjtoSe();
	// grava dados - VerifCircuito
	GravaDadoVerifCircuito();
	// grava dados - ParamBloco: DeletaTabParamBlocoRedes e GravaDadoParamBloco
	GravaDadoParamBloco();
	// grava dados - ParamCarga: DeletaTabParamCargaRedes e GravaDadoParamCarga
	GravaDadoParamCarga();
	// grava dados - identificador de falta
	GravaDadoChaveIF();
	// grava dados - indisponibilidade
	GravaDadoIndisponibilidade(paramConf->LisEqptoIndisp); // hk201808: indisp
	// grava dados - indisponibilidade padrao
	GravaDadoIndisponibilidadePadrao(paramConf->EqptoIndispPadrao->vindisp); // hk201808: indisp

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoIndisponibilidadePadrao(double vindisp) // hk201808: indisp
{
	AnsiString qryTxt;

	if (!ExisteCampoTabela("ParamGerais", "VIndisp"))
	{
		qryTxt.sprintf("ALTER TABLE ParamGerais ADD COLUMN VIndisp FLOAT;");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
	}

	qryTxt.sprintf("UPDATE (SELECT * FROM ParamGerais) SET ParamGerais.VIndisp = %.6f", vindisp);
	dcom->CommandText = qryTxt;
	dcom->Execute();
	return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoIndisponibilidade(TList *lisEqptoIndisp) // hk201808: indisp
{
	strDadoEqptoIndisp *dadoEqptoIndisp;
	AnsiString qryTxt;
	bool resposta;
	// Grava dados de indisponibilidade
	// cria tabela de Chaves Telecomandadas
	// deleta tabela
	DeletaTabela("ParamIndisp");

	try
	{ // monta sql
		qryTxt.sprintf("CREATE TABLE ParamIndisp (");
		qryTxt.cat_sprintf("codEqpto TEXT(255), ");
		qryTxt.cat_sprintf("vindisp FLOAT");
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	for (int n = 0; n < lisEqptoIndisp->Count; n++)
	{
		dadoEqptoIndisp = (strDadoEqptoIndisp*)lisEqptoIndisp->Items[n];

		try
		{ // monta sql
			qryTxt.sprintf("INSERT INTO ParamIndisp ");
			qryTxt.cat_sprintf("VALUES (");
			qryTxt.cat_sprintf("'%s', ", dadoEqptoIndisp->cod_eqpto.c_str());
			qryTxt.cat_sprintf("%.6f", dadoEqptoIndisp->vindisp);
			qryTxt.cat_sprintf(");");
			// executa
			dcom->CommandText = qryTxt;
			dcom->Execute();
			resposta = true;
		}
		catch (...)
		{
			resposta = false;
		}
	}
	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoChavesTelecom(void)
{
	// variáveis locais
	strDadoChaveTelecom *chvTelecom;
	sdi_ChaveTelecom sdi;

	// cria tabela de Chaves Telecomandadas
	CriaTabelaChavesTelecom();

	// proteção:
	if (!paramConf)
		return (false);

	// loop p/ todos dados lidos
	for (int n = 0; n < paramConf->LisChavesTelecom->Count; n++)
	{
		chvTelecom = (strDadoChaveTelecom*)paramConf->LisChavesTelecom->Items[n];
		// atribui valores
		sdi.codChave = chvTelecom->codChave;
		// grava em base
		if (!GravaDadoChavesTelecom(sdi))
			return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoChavesTelecom(sdi_ChaveTelecom &sdi)
{
	// variáveis locais
	bool resposta = true;
	AnsiString qryTxt;

	try
	{ // monta sql
		qryTxt.sprintf("INSERT INTO ChavesTelecom ");
		qryTxt.cat_sprintf("VALUES (");
		qryTxt.cat_sprintf("'%s'", sdi.codChave.c_str());
		qryTxt.cat_sprintf(");");



		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoChaveIF(void)
{
	// variáveis locais
	strDadoChaveIF *chvIF;
	sdi_ChaveTelecom sdi;

	// cria tabela de Chaves Telecomandadas
	CriaTabelaChavesIF();

	// proteção:
	if (!paramConf)
		return (false);

	// loop p/ todos dados lidos
	for (int n = 0; n < paramConf->LisChavesIF->Count; n++)
	{
		chvIF = (strDadoChaveIF*)paramConf->LisChavesIF->Items[n];
		// atribui valores
		sdi.codChave = chvIF->codChave;
		// grava em base
		if (!GravaDadoChaveIF(sdi))
			return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoChaveIF(sdi_ChaveTelecom &sdi)
{
	// variáveis locais
	bool resposta = true;
	AnsiString qryTxt;

	try
	{ // monta sql
		qryTxt.sprintf("INSERT INTO ChavesIF ");
		qryTxt.cat_sprintf("VALUES (");
		qryTxt.cat_sprintf("'%s'", sdi.codChave.c_str());
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoParamBloco(void)
{
	// variáveis locais
	strDadoParamBloco *paramBloco;
	sdi_ParamBloco sdi;

	if (!ExisteTabela("ParamBloco"))
	{
		// cria tabela ParamBloco
		// hkv201610 código da rede MT associada a carga //hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede
		CriaTabelaParamBloco();
	}
	else
	{

		// hk201710IF
		// if(!ExisteCampoTabela("ParamBloco", "codRede"))
		if ((!ExisteCampoTabela("ParamBloco", "codRede")) || (!ExisteCampoTabela("ParamBloco",
			"ptpre")))
		{
			DeletaTabela("ParamBloco");
			// recria a tabela com campo codRede
			CriaTabelaParamBloco();
		}
		else
		{
			if (!ExisteCampoTabela("ParamBloco", "codBarraChave"))
			{
				// ALTER TABLE ParamBloco ADD COLUMN codBarraChave TEXT(255)
				InsereCampoTabela("ParamBloco", "codBarraChave");
			}
			// hkv201610: ajustes de paramBloco e paramCarga em etapas carregando partes da rede -
			// retirada de eliminação de paramBloco não localizado na rede
			// Funcao para deletar registros da tabela TabParamBloco
			// carregados em memória no estudo
			DeletaTabParamBlocoRedes();
		}
	}
	// proteção:
	if (!paramConf)
		return (false);
	// loop p/ todos dados lidos
	for (int n = 0; n < paramConf->LisParamBloco->Count; n++)
	{
		paramBloco = (strDadoParamBloco*)paramConf->LisParamBloco->Items[n];
		// atribui valores
		sdi.codChave = paramBloco->codChave;
		sdi.codBarraChave = paramBloco->codBarra;
		sdi.codRede = paramBloco->codRede;
		sdi.lambda = paramBloco->lambda;
		sdi.trep = paramBloco->trep;

		sdi.ptpre = paramBloco->ptpre;
		sdi.ptdlo = paramBloco->ptdlo;
		sdi.ptexe = paramBloco->ptexe;
		// for(int nm = 0; nm < 12; nm++)
		// {
		// sdi.lamb[nm] = paramBloco->lamb[nm];
		// sdi.tr[nm]   = paramBloco->tr[nm];
		// }
		// grava em base
		if (!GravaDadoParamBloco(sdi))
			return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::InsereCampoTabela(AnsiString tabela, AnsiString campo)
{ // Função para inserir novo campo em tabela
	// variáveis locais
	bool resposta = false;
	AnsiString qryTxt;

	// ALTER TABLE ParamBloco ADD COLUMN codBarraChave TEXT(255)

	// Verifica se o campo já existe
	if (ExisteCampoTabela(tabela, campo))
		return (false);

	try
	{ // monta sql
		qryTxt.sprintf("ALTER TABLE %s ADD COLUMN %s TEXT(255);", tabela.c_str(), campo.c_str());
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoParamBloco(sdi_ParamBloco &sdi)
{
	// variáveis locais
	bool resposta = true;
	AnsiString qryTxt;
	bool flagCampo = ExisteCampoTabela("ParamBloco", "ptpre");

	try
	{ // monta sql
		qryTxt.sprintf("INSERT INTO ParamBloco ");
		qryTxt.cat_sprintf("VALUES (");
		qryTxt.cat_sprintf("'%s', ", sdi.codChave.c_str());
		qryTxt.cat_sprintf("'%s', ", sdi.codRede.c_str());
		// código da rede MT associada a carga //hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede : GravaDadoParamBloco(sdi_ParamBloco &sdi) : sdi.codRede
		qryTxt.cat_sprintf("%.6f, ", sdi.lambda);
		// hk201710IF: Acréscimo de perc. do tempo de reparo
		if (!flagCampo)
			qryTxt.cat_sprintf("%.6f, ", sdi.trep);
		else
		{
			qryTxt.cat_sprintf("%.6f, ", sdi.trep);
			qryTxt.cat_sprintf("%.6f, ", sdi.ptpre);
			qryTxt.cat_sprintf("%.6f, ", sdi.ptdlo);
			qryTxt.cat_sprintf("%.6f, ", sdi.ptexe);
		}

		// hk201802a: código da barra de referência para definição do bloco
		qryTxt.cat_sprintf("'%s'", sdi.codBarraChave.c_str());
		// hk201710IF: Acréscimo de perc. do tempo de reparo

		// for(int nm = 0; nm < 12; nm++)
		// {
		// qryTxt.cat_sprintf("%.6f"   , sdi.lamb[nm]);
		// qryTxt.cat_sprintf("%.6f"   , sdi.tr[nm]);
		// }
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoParamCarga(void)
{
	// variáveis locais
	strDadoParamCarga *paramCarga;
	sdi_ParamCarga sdi;

	if (!ExisteTabela("ParamCarga"))
	{
		// cria tabela ParamCarga
		// hkv201610 código da rede MT associada a carga :GravaDadoParamCarga(void) //hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede
		CriaTabelaParamCarga();
	}
	else
	{
		// if(!ExisteCampoTabela("ParamCarga", "codRede"))
		// hk201710IF
		if ((!ExisteCampoTabela("ParamCarga", "codRede")) || (!ExisteCampoTabela("ParamCarga",
			"ptpre")))
		{
			DeletaTabela("ParamCarga");
			// recria a tabela com campo codRede
			CriaTabelaParamCarga();
		}
		else
		{
			// hkv201610: ajustes de paramBloco e paramCarga em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede
			// Funcao para deletar registros da tabela TabParamCarga
			// carregados em memória no estudo
			DeletaTabParamCargaRedes();
		}
	}

	// proteção:
	if (!paramConf)
		return (false);
	// loop p/ todos dados lidos
	for (int n = 0; n < paramConf->LisParamCarga->Count; n++)
	{
		paramCarga = (strDadoParamCarga*)paramConf->LisParamCarga->Items[n];
		// atribui valores
		sdi.codCarga = paramCarga->codCarga;
		sdi.codRede = paramCarga->codRede;
		// hkv201610 Acrescenta registros carregadas no estudo código da rede MT associada a carga //hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede
		sdi.lambda = paramCarga->lambda;
		sdi.trep = paramCarga->trep;
		// hk201710IF
		sdi.ptpre = paramCarga->ptpre;
		sdi.ptdlo = paramCarga->ptdlo;
		sdi.ptexe = paramCarga->ptexe;
		// for(int nm = 0; nm < 12; nm++)
		// {
		// sdi.lamb[nm] = paramCarga->lamb[nm];
		// sdi.tr[nm]   = paramCarga->tr[nm];
		// }
		// grava em base
		if (!GravaDadoParamCarga(sdi))
			return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TBDadoParam::DeletaTabParamCargaRedes(void)
{
	// Funcao para deletar registros da tabela TabParamCarga
	// carregados em memória no estudo
	// hkv201610: ajustes de paramBloco e paramCarga em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede : DeletaTabParamCargaRedes(void)

	// variáveis locais
	strDadoParamCarga *paramCarga;
	TStringList *lisStrRedes;
	AnsiString codRede;

	lisStrRedes = new TStringList();
	// Gera lista de códigos de rede em lisParamCarga
	for (int n = 0; n < paramConf->LisParamCarga->Count; n++)
	{
		paramCarga = (strDadoParamCarga*)paramConf->LisParamCarga->Items[n];
		codRede = "";
		for (int nr = 0; nr < lisStrRedes->Count; nr++)
		{
			if ((codRede = lisStrRedes->Strings[nr]).AnsiCompare(paramCarga->codRede) == 0)
				break;
			codRede = "";
		}
		if (codRede == "")
			lisStrRedes->Add(paramCarga->codRede);
	}
	AnsiString aux;

	aux = "DELETE ParamCarga.codRede FROM ParamCarga ";
	aux.cat_sprintf("WHERE ((");
	for (int nr = 0; nr < lisStrRedes->Count; nr++)
	{
		codRede = lisStrRedes->Strings[nr];
		aux.cat_sprintf("[ParamCarga]![codRede]=");
		aux.cat_sprintf("'%s'", codRede.c_str());
		if (nr == (lisStrRedes->Count - 1))
			continue;
		aux.cat_sprintf(" Or ");
	}
	aux.cat_sprintf("));");

	// hk201701: protecao para lista de redes nula : DeletaTabParamCargaRedes(void)
	if (lisStrRedes->Count > 0)
	{
		dcom->CommandText = aux;
		dcom->Execute();
	}
	delete lisStrRedes;

	// DELETE ParamBloco.codRede
	// FROM ParamBloco
	// WHERE (([ParamBloco]![codRede]="RDUT  0102" Or [ParamBloco]![codRede]="RDUT  0103"));

}

// ---------------------------------------------------------------------------
void __fastcall TBDadoParam::DeletaTabParamBlocoRedes(void)
{
	// Funcao para deletar registros da tabela TabParamCarga
	// carregados em memória no estudo
	// hkv201610: ajustes de paramBloco e paramCarga em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede : DeletaTabParamBlocoRedes(void)

	// variáveis locais
	strDadoParamBloco *paramBloco;
	TStringList *lisStrRedes;
	AnsiString codRede, aux;

	lisStrRedes = new TStringList();
	// loop p/ todos dados lidos
	for (int n = 0; n < paramConf->LisParamBloco->Count; n++)
	{
		paramBloco = (strDadoParamBloco*)paramConf->LisParamBloco->Items[n];
		codRede = "";
		for (int nr = 0; nr < lisStrRedes->Count; nr++)
		{
			if ((codRede = lisStrRedes->Strings[nr]).AnsiCompare(paramBloco->codRede) == 0)
				break;
			codRede = "";
		}
		if (codRede == "")
			lisStrRedes->Add(paramBloco->codRede);
	}

	// 31.01.2018 - correção GH: alterado o aux, retirado ParamBloco.codRede, corrigido para lisStrRedes->Count >= 1
	aux = "DELETE FROM ParamBloco ";
	if (lisStrRedes->Count >= 1)
	{
		aux.cat_sprintf("WHERE ((");
	}

	for (int nr = 0; nr < lisStrRedes->Count; nr++)
	{
		codRede = lisStrRedes->Strings[nr];
		aux.cat_sprintf("[ParamBloco]![codRede]=");
		aux.cat_sprintf("'%s'", codRede.c_str());
		if (nr == (lisStrRedes->Count - 1))
			continue;
		aux.cat_sprintf(" Or ");
	}
	if (lisStrRedes->Count >= 1)
	{
		aux.cat_sprintf("));");
	}

	delete lisStrRedes;
	// executa
	dcom->CommandText = aux;
	dcom->Execute();

	// DELETE ParamBloco.codRede
	// FROM ParamBloco
	// WHERE (([ParamBloco]![codRede]="RDUT  0102" Or [ParamBloco]![codRede]="RDUT  0103"));

}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoParamCarga(sdi_ParamCarga &sdi)
{
	// variáveis locais
	bool resposta = true;
	AnsiString qryTxt;
	bool flagCampo = ExisteCampoTabela("ParamCarga", "ptpre");

	try
	{ // monta sql
		qryTxt.sprintf("INSERT INTO ParamCarga ");
		qryTxt.cat_sprintf("VALUES (");
		qryTxt.cat_sprintf("'%s', ", sdi.codCarga.c_str());
		qryTxt.cat_sprintf("'%s', ", sdi.codRede.c_str());
		// hkv201610 código da rede MT associada a carga : GravaDadoParamCarga(sdi_ParamCarga &sdi)//hkv201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede
		qryTxt.cat_sprintf("%.6f, ", sdi.lambda);

		// hk201710IF: Acréscimo de perc. do tempo de reparo
		if (!flagCampo)
			qryTxt.cat_sprintf("%.6f", sdi.trep);
		else
		{
			qryTxt.cat_sprintf("%.6f, ", sdi.trep);
			qryTxt.cat_sprintf("%.6f, ", sdi.ptpre);
			qryTxt.cat_sprintf("%.6f, ", sdi.ptdlo);
			qryTxt.cat_sprintf("%.6f", sdi.ptexe);
		}

		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/15456154
void __fastcall TBDadoParam::LimpaRegistrosParamEqpto(TList *lisParamEqpto)
{
    // variáveis locais
    strDadoParamEqpto*  paramEqpto;
    sdi_ParamEqpto      sdi;
    const char*         codRedeCC;
    TADOQuery*          tdb          = NULL;
    bool                flagCamposIF = false;
    TField*             field;
    AnsiString          aux, qryTxt;

    // Se a lista estiver vazia, não tem por que fazer os deletes, né? :)
    if (lisParamEqpto->Count <= 0)
    {
        return;
    }
    TStringList* lisCodRede;
    lisCodRede = new TStringList();
    AnsiString codRede;
    for (int i = 0; i < lisParamEqpto->Count; i++)
    {
        paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[i];
        codRede = paramEqpto->codRede.Trim();
        if (lisCodRede->IndexOf(codRede) == -1)
            lisCodRede->Add(codRede);
    }

    for (int nc = 0; nc < lisCodRede->Count; nc++)
    {
        codRede = lisCodRede->Strings[nc];
        qryTxt.sprintf("DELETE FROM ParamEqpto ");
        qryTxt.cat_sprintf("WHERE codRede = '%s';", codRede.c_str());
        // qryTxt.cat_sprintf(codRede);
        // qryTxt.cat_sprintf("';");
        dcom->CommandText = qryTxt;
        dcom->Execute();
    }
    delete lisCodRede;

    /* for (int i = 0; i < lisParamEqpto->Count; i++)
     {
     qryTxt.sprintf("DELETE FROM ParamEqpto ");
     qryTxt.cat_sprintf("WHERE codRede = '");

     paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[i];
     codRedeCC = paramEqpto->codRede.c_str();

     qryTxt.cat_sprintf(codRedeCC);
     qryTxt.cat_sprintf("';");

     dcom->CommandText = qryTxt;
     dcom->Execute();
     }
     */
    // Exemplo da query
    // DELETE FROM ParamEqpto WHERE codRede IN ('JGQ-01Y1','JEQ-01V3','CRV-01J1','KQT-01P2');
    /* Query dando erro, pois tem muitos parâmetros.
     qryTxt.sprintf("DELETE FROM ParamEqpto ");
     qryTxt.cat_sprintf("WHERE codRede IN(");
     for (int i = 0; i < lisParamEqpto->Count; i++)
     {
     paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[i];
     codRedeCC = paramEqpto->codRede.c_str();
     qryTxt.cat_sprintf("'");
     qryTxt.cat_sprintf(codRedeCC);
     qryTxt.cat_sprintf("'");
     if (i + 1 < lisParamEqpto->Count)
     {
     qryTxt.cat_sprintf(", ");
     }
     }
     qryTxt.cat_sprintf(");");

     dcom->CommandText = qryTxt;
     dcom->Execute();
     */
}

// ---------------------------------------------------------------------------
void __fastcall TBDadoParam::LimpaRegistrosVerifSe(TList *lisVerifSe)
{
	// variáveis locais
	VTVerifSe *verifSe;
	AnsiString qryTxt;
	const char *codigoCC;

	// Se a lista estiver vazia, não tem por que fazer os deletes, né? :)
	if (lisVerifSe->Count <= 0)
	{
		return;
	}

	for (int i = 0; i < lisVerifSe->Count; i++)
	{
		qryTxt.sprintf("DELETE FROM VerifSe ");
		qryTxt.cat_sprintf("WHERE codSe = '");

		verifSe = (VTVerifSe*)lisVerifSe->Items[i];
		codigoCC = verifSe->Codigo.c_str();

		qryTxt.cat_sprintf(codigoCC);
		qryTxt.cat_sprintf("';");

		dcom->CommandText = qryTxt;
		dcom->Execute();
	}

}

// ---------------------------------------------------------------------------
void __fastcall TBDadoParam::LimpaRegistrosVerifCjtoSe(TList *lisVerifCjto)
{
	// variáveis locais
	VTVerifCjto *verifCjto;
	VTVerifSe *verifSe;
	AnsiString qryTxt;
	const char *codigoSeCC, *codigoCjtoCC;

	// Se a lista estiver vazia, não tem por que fazer os deletes, né? :)
	if (lisVerifCjto->Count <= 0)
	{
		return;
	}

	for (int n = 0; n < lisVerifCjto->Count; n++)
	{
		verifCjto = (VTVerifCjto*)paramConf->LisVerifCjto->Items[n];

		// lê dados de subestação que fazem parte do conjunto
		for (int i = 0; i < verifCjto->LisVerifSe->Count; i++)
		{

			verifSe = (VTVerifSe*)verifCjto->LisVerifSe->Items[i];
			// atribui valores
			codigoSeCC = verifSe->Codigo.c_str();
			codigoCjtoCC = verifSe->CodigoCjto.c_str();

			qryTxt.sprintf("DELETE FROM VerifCjtoSe ");
			qryTxt.cat_sprintf("WHERE codSe = '");
			qryTxt.cat_sprintf(codigoSeCC);
			qryTxt.cat_sprintf("' AND codCjto = '");
			qryTxt.cat_sprintf(codigoCjtoCC);
			qryTxt.cat_sprintf("';");

			dcom->CommandText = qryTxt;
			dcom->Execute();

		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TBDadoParam::LimpaRegistrosVerifCjto(TList *lisVerifCjto)
{
	// variáveis locais
	VTVerifCjto *verifCjto;
	AnsiString qryTxt;
	const char *codigoCC;

	// Se a lista estiver vazia, não tem por que fazer os deletes, né? :)
	if (lisVerifCjto->Count <= 0)
	{
		return;
	}

	for (int i = 0; i < lisVerifCjto->Count; i++)
	{
		qryTxt.sprintf("DELETE FROM VerifCjto ");
		qryTxt.cat_sprintf("WHERE codCjto = '");

		verifCjto = (VTVerifCjto*)lisVerifCjto->Items[i];
		codigoCC = verifCjto->Codigo.c_str();

		qryTxt.cat_sprintf(codigoCC);
		qryTxt.cat_sprintf("';");

		dcom->CommandText = qryTxt;
		dcom->Execute();
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoParamEqpto(TList *lisParamEqpto)
{
	// variáveis locais
	strDadoParamEqpto *paramEqpto;
	sdi_ParamEqpto sdi;

	// cria tabela ParamEqpto
	// CriaTabelaParamEqpto();
	LimpaRegistrosParamEqpto(lisParamEqpto);

	// hk201705: define se a tabela já está atualizada com campos ptpre, ptdlo e ptexe
	TADOQuery *tdb = NULL;
	bool flagCamposIF = false;
	TField *field;
	AnsiString aux, qryTxt;

	qryTxt.sprintf("SELECT * FROM ParamEqpto;");
	// executa query
	tdb = ExecutaQuery(qryTxt);
	for (int nn = 0; nn < tdb->FieldList->Count; nn++)
	{
		field = tdb->FieldList->Fields[nn];
		aux = field->FieldName;
		if (aux.AnsiCompare("ptpre") == 0)
			flagCamposIF = true;
	}
	EncerraQuery(tdb);

	// loop p/ todos dados lidos
	for (int n = 0; n < lisParamEqpto->Count; n++)
	{
		paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[n];
		// atribui valores
		sdi.codEqpto = paramEqpto->codEqpto;
		sdi.codRede = paramEqpto->codRede;
		sdi.tipoRede = paramEqpto->tipoRede;
		sdi.ndiasObs = paramEqpto->ndiasObs;
		// hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
		sdi.ptpre = paramEqpto->ptpre;
		sdi.ptdlo = paramEqpto->ptdlo;
		sdi.ptexe = paramEqpto->ptexe;

		sdi.nf = paramEqpto->nf;
		sdi.nfr = paramEqpto->nfr;
		sdi.td = paramEqpto->td;
		sdi.cli = paramEqpto->cli;
		sdi.clih = paramEqpto->clih;

		// meses
		for (int n = 0; n < 12; n++)
		{
			sdi.nfMes[n] = paramEqpto->nfmes[n];
			sdi.nfrMes[n] = paramEqpto->nfrmes[n];
			sdi.tdMes[n] = paramEqpto->tdmes[n];
			sdi.climes[n] = paramEqpto->climes[n];
			sdi.clihmes[n] = paramEqpto->clihmes[n];
		}
		// horas
		for (int n = 0; n < 24; n++)
		{
			sdi.nfHora[n] = paramEqpto->nfhora[n];
			sdi.nfrHora[n] = paramEqpto->nfrhora[n];
		}

		// grava em base
		if (!GravaDadoParamEqpto(sdi, flagCamposIF))
			return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoParamEqpto(sdi_ParamEqpto &sdi, bool flagCamposIF)
{
	// variáveis locais
	bool resposta = true;
	AnsiString qryTxt;

	try
	{ // monta sql
		qryTxt.sprintf("INSERT INTO ParamEqpto ");
		qryTxt.cat_sprintf("VALUES (");
		qryTxt.cat_sprintf("'%s', ", sdi.codEqpto.c_str());
		qryTxt.cat_sprintf("'%s', ", sdi.codRede.c_str());
		qryTxt.cat_sprintf("'%s', ", sdi.tipoRede.c_str());
		qryTxt.cat_sprintf("%.6f, ", sdi.ndiasObs);
		if (flagCamposIF == true)
		{
			qryTxt.cat_sprintf("%.6f, ", sdi.ptpre);
			qryTxt.cat_sprintf("%.6f, ", sdi.ptdlo);
			qryTxt.cat_sprintf("%.6f, ", sdi.ptexe);
		}

		qryTxt.cat_sprintf("%.6f, ", sdi.nf);
		qryTxt.cat_sprintf("%.6f, ", sdi.nfr);
		qryTxt.cat_sprintf("%.6f, ", sdi.td);
		qryTxt.cat_sprintf("%.6f, ", sdi.cli);
		qryTxt.cat_sprintf("%.6f, ", sdi.clih);
		// meses
		for (int n = 0; n < 12; n++)
		{
			qryTxt.cat_sprintf("%.6f, ", sdi.nfMes[n]);
			qryTxt.cat_sprintf("%.6f, ", sdi.nfrMes[n]);
			qryTxt.cat_sprintf("%.6f, ", sdi.tdMes[n]);
			qryTxt.cat_sprintf("%.6f, ", sdi.climes[n]);
			qryTxt.cat_sprintf("%.6f, ", sdi.clihmes[n]);
		}
		// horas
		for (int n = 0; n < 24; n++)
		{
			qryTxt.cat_sprintf("%.6f, ", sdi.nfHora[n]);
			if (n < 23)
				qryTxt.cat_sprintf("%.6f, ", sdi.nfrHora[n]);
			else
				qryTxt.cat_sprintf("%.6f", sdi.nfrHora[n]);

		}
		// fecha
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoParamGerais(void)
{
	// variáveis locais
	strDadoParamGerais *paramGerais;
	sdi_ParamGerais sdi;

	// cria tabela ParamGerais
	CriaTabelaParamGerais();

	// gh201806: define se a tabela já está atualizada com o campo red_tprep_IF
	TADOQuery *tdb = NULL;
	bool flagTprepIF = false;
	TField *field;
	AnsiString aux, qryTxt;

	qryTxt.sprintf("SELECT * FROM ParamGerais;");
	// executa query
	tdb = ExecutaQuery(qryTxt);
	for (int nn = 0; nn < tdb->FieldList->Count; nn++)
	{
		field = tdb->FieldList->Fields[nn];
		aux = field->FieldName;
		if (aux.AnsiCompare("red_tprep_IF") == 0)
			flagTprepIF = true;
	}

	// proteção:
	if (!paramConf)
		return (false);
	// atribui valores
	sdi.flagRecurso = paramConf->ParamGerais->flagRecurso;
	sdi.flagRetorno = paramConf->ParamGerais->flagRetorno;
	sdi.tlocaliza = paramConf->ParamGerais->tlocaliza;
	sdi.tman_manual = paramConf->ParamGerais->tman_manual;
	sdi.tman_auto = paramConf->ParamGerais->tman_auto;
	// hkdef sdi.trepDefault   = paramConf->ParamGerais->trrepDefault;
	// hkdef sdi.lambdaDefault = paramConf->ParamGerais->llambdaDefault;
	sdi.pu_lim = paramConf->ParamGerais->pu_lim;
	sdi.percDefTrans = paramConf->ParamGerais->percDefTrans;
	sdi.iProfReliga = paramConf->ParamGerais->iProfReliga;
	sdi.tlimIC = paramConf->ParamGerais->tlimIC;

	sdi.lambda = paramConf->ParamGerais->lambda;
	sdi.trep = paramConf->ParamGerais->trep;
	// gh201806: acréscimo de parâmetro de porcentagem de redução de tempo de preparo por alocação de IF
	if (flagTprepIF == true)
	{
		sdi.red_tprep_IF = paramConf->ParamGerais->red_tprep_IF;
	}

	// grava em base
	if (!GravaDadoParamGerais(sdi, flagTprepIF))
		return (false);

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoParamGerais(sdi_ParamGerais &sdi, bool flagTprepIF)
{
	// variáveis locais
	bool resposta = true;
	AnsiString qryTxt;

	try
	{ // monta sql
		qryTxt.sprintf("INSERT INTO ParamGerais ");
		qryTxt.cat_sprintf("VALUES (");
		qryTxt.cat_sprintf("%i, ", sdi.flagRecurso ? 1 : 0);
		qryTxt.cat_sprintf("%i, ", sdi.flagRetorno ? 1 : 0);
		qryTxt.cat_sprintf("%.6f, ", sdi.tlocaliza);
		qryTxt.cat_sprintf("%.6f, ", sdi.tman_manual);
		qryTxt.cat_sprintf("%.6f, ", sdi.tman_auto);
		// hkdef qryTxt.cat_sprintf("%.6f, " , sdi.ttrepDefault);
		// hkdef qryTxt.cat_sprintf("%.6f, " , sdi.llambdaDefault);
		qryTxt.cat_sprintf("%.6f, ", sdi.pu_lim);
		qryTxt.cat_sprintf("%.6f, ", sdi.percDefTrans);
		qryTxt.cat_sprintf("%i, ", sdi.iProfReliga);
		qryTxt.cat_sprintf("%.6f, ", sdi.tlimIC);

		qryTxt.cat_sprintf("%.6f, ", sdi.lambda);
		qryTxt.cat_sprintf("%.6f, ", sdi.trep);
		if (flagTprepIF == true)
		{
			qryTxt.cat_sprintf("%.6f", sdi.red_tprep_IF);
		}
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoParamRede(void)
{
	// variáveis locais
	strDadoParamRede *paramRede;
	sdi_ParamRede sdi;

	// cria tabela ParamRede
	CriaTabelaParamRede();

	// proteção:
	if (!paramConf)
		return (false);
	// loop p/ todos dados lidos
	for (int n = 0; n < paramConf->LisParamRede->Count; n++)
	{
		paramRede = (strDadoParamRede*)paramConf->LisParamRede->Items[n];
		// atribui valores
		sdi.codRede = paramRede->codRede;
		sdi.lambda = paramRede->lambda;
		sdi.trep = paramRede->trep;

		// grava em base
		if (!GravaDadoParamRede(sdi))
			return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoParamRede(sdi_ParamRede &sdi)
{
	// variáveis locais
	bool resposta = true;
	AnsiString qryTxt;

	try
	{ // monta sql
		qryTxt.sprintf("INSERT INTO ParamRede ");
		qryTxt.cat_sprintf("VALUES (");
		qryTxt.cat_sprintf("'%s', ", sdi.codRede.c_str());
		qryTxt.cat_sprintf("%.6f, ", sdi.lambda);
		qryTxt.cat_sprintf("%.6f", sdi.trep);
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoTipoEqProt(void)
{
	// variáveis locais
	strDadoTipoEqProt *tipoEqProt;
	sdi_TipoEqProt sdi;

	// cria tabela de Tipos de Eqpto de proteção
	CriaTabelaTipoEqProt();

	// proteção:
	if (!paramConf)
		return (false);

	// loop p/ todos dados lidos
	for (int n = 0; n < paramConf->LisTipoEqProt->Count; n++)
	{
		tipoEqProt = (strDadoTipoEqProt*)paramConf->LisTipoEqProt->Items[n];
		// atribui valores
		sdi.codTipoEqProt = tipoEqProt->codTipoEqProt;
		// grava em base
		if (!GravaDadoTipoEqProt(sdi))
			return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoTipoEqProt(sdi_TipoEqProt &sdi)
{
	// variáveis locais
	bool resposta = true;
	AnsiString qryTxt;

	try
	{ // monta sql
		qryTxt.sprintf("INSERT INTO TipoEqProt ");
		qryTxt.cat_sprintf("VALUES (");
		qryTxt.cat_sprintf("'%s'", sdi.codTipoEqProt.c_str());
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoVerifCircuito(void)
{
	// variáveis locais
	VTVerifCircuito *verifCircuito;
	sdi_VerifCircuito sdi;

	// cria tabela VerifCircuito
	CriaTabelaVerifCircuito();

	// proteção:
	if (!paramConf)
		return (false);
	// loop p/ todos dados lidos
	for (int n = 0; n < paramConf->LisVerifCircuito->Count; n++)
	{
		verifCircuito = (VTVerifCircuito*)paramConf->LisVerifCircuito->Items[n];
		// atribui valores
		sdi.codCircuito = verifCircuito->Codigo;
		sdi.decVerif = verifCircuito->DecVerif;
		sdi.fecVerif = verifCircuito->FecVerif;
		// grava em base
		if (!GravaDadoVerifCircuito(sdi))
			return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoVerifCircuito(sdi_VerifCircuito &sdi)
{
	// variáveis locais
	bool resposta = true;
	AnsiString qryTxt;

	try
	{ // monta sql
		qryTxt.sprintf("INSERT INTO VerifCircuito ");
		qryTxt.cat_sprintf("VALUES (");
		qryTxt.cat_sprintf("'%s', ", sdi.codCircuito.c_str());
		qryTxt.cat_sprintf("%.6f, ", sdi.decVerif);
		qryTxt.cat_sprintf("%.6f", sdi.fecVerif);
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoVerifCjto(void)
{
	// variáveis locais
	VTVerifCjto *verifCjto;
	VTVerifSe *se;
	sdi_VerifCjto sdi;
	sdi_VerifCjtoSe sdi_se;

	// cria tabela VerifCjto
	// CriaTabelaVerifCjto();
	LimpaRegistrosVerifCjto(paramConf->LisVerifCjto);
	// cria tabela VerifCjtoSe
	// CriaTabelaVerifCjtoSe();

	// proteção:
	if (!paramConf)
		return (false);
	// loop p/ todos dados lidos
	for (int n = 0; n < paramConf->LisVerifCjto->Count; n++)
	{
		verifCjto = (VTVerifCjto*)paramConf->LisVerifCjto->Items[n];
		// atribui valores
		sdi.codCjto = verifCjto->Codigo;
		sdi.decVerif = verifCjto->DecVerif;
		sdi.fecVerif = verifCjto->FecVerif;
		// grava em base
		if (!GravaDadoVerifCjto(sdi))
			return (false);
		// lê dados de subestação que fazem parte do conjunto
		/*
		 for (int i = 0; i < verifCjto->LisVerifSe->Count; i++)
		 {
		 se = (VTVerifSe*)verifCjto->LisVerifSe->Items[i];
		 // atribui valores
		 sdi_se.codSe = se->Codigo;
		 sdi_se.codCjto = verifCjto->Codigo;
		 // grava em base
		 if (!GravaDadoVerifCjtoSe(sdi_se)) return(false);
		 }
		 */
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoVerifCjtoSe(void)
{
	// variáveis locais
	VTVerifCjto *verifCjto;
	VTVerifSe *se;
	sdi_VerifCjto sdi;
	sdi_VerifCjtoSe sdi_se;

	// cria tabela VerifCjtoSe
	// CriaTabelaVerifCjtoSe();
	LimpaRegistrosVerifCjtoSe(paramConf->LisVerifCjto);

	// proteção:
	if (!paramConf)
		return (false);
	// loop p/ todos dados lidos
	for (int n = 0; n < paramConf->LisVerifCjto->Count; n++)
	{
		verifCjto = (VTVerifCjto*)paramConf->LisVerifCjto->Items[n];

		// lê dados de subestação que fazem parte do conjunto
		for (int i = 0; i < verifCjto->LisVerifSe->Count; i++)
		{
			se = (VTVerifSe*)verifCjto->LisVerifSe->Items[i];
			// atribui valores
			sdi_se.codSe = se->Codigo;
			sdi_se.codCjto = verifCjto->Codigo;
			// grava em base
			if (!GravaDadoVerifCjtoSe(sdi_se))
				return (false);
		}
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoVerifSe(void)
{
	// variáveis locais
	VTVerifSe *verifSe;
	sdi_VerifSe sdi;

	// cria tabela VerifSe
	// CriaTabelaVerifSe();
	LimpaRegistrosVerifSe(paramConf->LisVerifSe);

	// proteção:
	if (!paramConf)
		return (false);
	// loop p/ todos dados lidos
	for (int n = 0; n < paramConf->LisVerifSe->Count; n++)
	{
		verifSe = (VTVerifSe*)paramConf->LisVerifSe->Items[n];
		// atribui valores
		sdi.codSe = verifSe->Codigo;
		sdi.decVerif = verifSe->DecVerif;
		sdi.fecVerif = verifSe->FecVerif;
		// grava em base
		if (!GravaDadoVerifSe(sdi))
			return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoVerifCjto(sdi_VerifCjto &sdi)
{
	// variáveis locais
	bool resposta = true;
	AnsiString qryTxt;

	try
	{ // monta sql
		qryTxt.sprintf("INSERT INTO VerifCjto ");
		qryTxt.cat_sprintf("VALUES (");
		qryTxt.cat_sprintf("'%s', ", sdi.codCjto.c_str());
		qryTxt.cat_sprintf("%.6f, ", sdi.decVerif);
		qryTxt.cat_sprintf("%.6f", sdi.fecVerif);
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoVerifCjtoSe(sdi_VerifCjtoSe &sdi)
{
	// variáveis locais
	bool resposta = true;
	AnsiString qryTxt;

	try
	{ // monta sql
		qryTxt.sprintf("INSERT INTO VerifCjtoSe ");
		qryTxt.cat_sprintf("VALUES (");
		qryTxt.cat_sprintf("'%s', ", sdi.codCjto.c_str());
		qryTxt.cat_sprintf("'%s'", sdi.codSe.c_str());
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::GravaDadoVerifSe(sdi_VerifSe &sdi)
{
	// variáveis locais
	bool resposta = true;
	AnsiString qryTxt;

	try
	{ // monta sql
		qryTxt.sprintf("INSERT INTO VerifSe ");
		qryTxt.cat_sprintf("VALUES (");
		qryTxt.cat_sprintf("'%s', ", sdi.codSe.c_str());
		qryTxt.cat_sprintf("%.6f, ", sdi.decVerif);
		qryTxt.cat_sprintf("%.6f", sdi.fecVerif);
		qryTxt.cat_sprintf(");");
		// executa
		dcom->CommandText = qryTxt;
		dcom->Execute();
		resposta = true;
	}
	catch (...)
	{
		resposta = false;
	}

	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::ImportaDadoChaveIF(TStringList *lisGrupoSensores, AnsiString arq)
{
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString termArq, aux, dirMed;
	FILE *fin;
	int id_grupo, iaux;
	TDateTime dataIni, dataFim;
	bool flagEOF, flagFIMLINHA;
	AnsiString texto;
	TSearchRec sr;
	bool resposta = false;
	AnsiString d[13];
	AnsiString cod_chave;

	fin = fopen(arq.c_str(), "r");
	// Leitura da 1a linha //correcao da primeira linha Grupo 2165 e 227
	texto = LeDadoTexto(fin, flagFIMLINHA, flagEOF);
	// Verifica se é sem medicao
	if (texto.AnsiCompare("Informacoes de sensores") == 0)
	{
		// Reinicia a lista de sensores
		lisGrupoSensores->Clear();
		// vai até o fim da primeira linha
		while ((!flagEOF) && (!flagFIMLINHA))
			LeDadoTexto(fin, flagFIMLINHA, flagEOF);
		// vai até o fim da segunda linha
		flagEOF = flagFIMLINHA = false;
		while ((!flagEOF) && (!flagFIMLINHA))
			aux = LeDadoTexto(fin, flagFIMLINHA, flagEOF);
		// hk201712: se tem a informação de código da barra inicial e final
		// if((aux.AnsiCompare("DEFASAGEM") == 0)||(aux.AnsiCompare("FPBAIXA") == 0)||(aux.AnsiCompare("SEQ") == 0)) flagCodBarra = true;
		// 0:Grupo;
		// 1:Chave;
		// 2:Alimentador;
		// 3:ID_SENSOR1;
		// 4:ID_SENSOR2;
		// 5:ID_SENSOR3;
		// 6:flagConv;
		// 7:FP
		// se flagCodBarra = true 8:CodBarra1, 9: CodBarra2
		flagEOF = false;
		while (!flagEOF)
		{
			d[0] = LeDadoTexto(fin, flagFIMLINHA, flagEOF);
			if (!flagEOF)
			{
				for (int n = 1; n < 13; n++)
					d[n] = LeDadoTexto(fin, flagFIMLINHA, flagEOF);

				cod_chave = d[1].Trim();
				lisGrupoSensores->Add(cod_chave);

			}
		}
		resposta = true;
	}
	fclose(fin);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::ImportaDadoParamEqpto(TList *lisParamEqpto)
{
	bool resposta;
	resposta = GravaDadoParamEqpto(lisParamEqpto);
	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::ImportaDadoTipoEqProt(TList *lisTipoEqProt)
{
	// variáveis locais
	VTTipoChave *tipoEqProt;
	sdi_TipoEqProt sdi;

	// cria tabela TipoEqProt
	CriaTabelaTipoEqProt();

	// loop p/ todos dados lidos
	for (int n = 0; n < lisTipoEqProt->Count; n++)
	{
		tipoEqProt = (VTTipoChave*)lisTipoEqProt->Items[n];
		// atribui valores
		sdi.codTipoEqProt = tipoEqProt->Codigo;
		// grava em base
		if (!GravaDadoTipoEqProt(sdi))
			return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::LeDado(VTParamConf *paramConf, TStringList *lisCodRede)
{
	// ponteiro para objeto externo
	this->paramConf = paramConf;
	// 1.lê ParamGerais
	// inicializa parâmetros gerais
	// ParamGerais->flagRecurso,ParamGerais->flagRetorno,ParamGerais->tlocaliza,
	// ParamGerais->tman_manual,ParamGerais->tman_auto
	// ParamGerais->pu_lim,	ParamGerais->percDefTrans,	ParamGerais->iProfReliga
	// ParamGerais->tlimIC,	ParamGerais->lambda,	ParamGerais->trep
	LeDadoParamGerais();
	// 2.lê ParamRede, atualiza paramConf->LisParamRede param->codRede, param->lambda, param->trep
	// Cria e popula dados relativos a ocorrencias médias por circuito
	// codRede,lambda,trep
	LeDadoParamRede();
	// 3.lê ParamEqpto: limpa e atualiza paramConf->LisParamEqpto, para as redes de lisCodRede
	// Cria e popula dados relativos a equipamentos atuados na rede
	// codEqpto,codRede,tipoRede,
	// ndiasObs,nf,nfr,td,cli,clih,
	// nfmes[0-12], nfrmes[0-12],tdmes[0-12],climes[0-12],clihmes[0-12],
	// nfhora[0-24],nfrhora[0-24]
	LeDadoParamEqpto(lisCodRede);
	// 4.lê paramConf->LisParamBloco->ParamBloco: limpa e atualiza paramConf->LisParamBloco
	// Cria e popula dados ajustados relativos a blocos, identiicados por código da chave de suprimento
	// codChave,lambda,trep
	LeDadoParamBloco();
	// Aviso("passou");
	// 5.lê ParamCarga: limpa e atualiza paramConf->LisParamCarga
	// Cria e popula dados ajustados relativos a cargas, identiicados por código do trafo de rede MT ou cargaA4
	// codCarga,lambda,trep
	LeDadoParamCarga();
	// 6lê VerifCircuito : limpa e atualiza paramConf->LisVerifCircuito->VTVerifCircuito->(CodCirc, decVerif, fecVerif)
	LeDadoVerifCircuito();
	// 7.lê VerifSe       : limpa e paramConf->LisVerifSe->VTVerifSe->(CodCirc, decVerif, fecVerif)
	LeDadoVerifSe();
	// 8.lê VerifCjto     : limpa e paramConf->LisVerifCjto->VTVerifCjto->(CodCjto, decVerif, fecVerif) e popula verifCjto->LisVerifSe
	LeDadoVerifCjto();
	// 9.lê ChaveTelecom  : limpa e atualiza paramConf->LisChavesTelecom
	LeDadoChaveTelecom();
	// 10.lê TipoEqProt    : limpa e atualiza paramConf->LisTipoEqProt
	LeDadoTipoEqProt();
	// 11.lê IF   : limpa e atualiza paramConf->LisChavesIF
	LeDadoChaveIF();
	// 12. lê dados de indisponibilidade hk201808: indisp
	LeDadoParamIndisp();

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::LeDadoChaveIF(void)
{
	// variáveis locais
	AnsiString qryTxt;
	TADOQuery *tdb = NULL;
	sdi_ChaveIF sdi;
	sdi_Fld_ChaveIF sdi_fld;
	VTLog *log = (VTLog*)apl->GetObject(__classid(VTLog));

	try
	{
		if (!ExisteTabela("ChavesIF"))
			return (true); // hk201808: reformula

		// monta sql
		qryTxt.sprintf("SELECT * FROM ChavesIF;");
		try
		{
			// executa query
			tdb = ExecutaQuery(qryTxt);
		}
		catch (...)
		{
			if (log)
			{
				log->Add("CriaTabelaChavesIF");
			}
			CriaTabelaChavesIF();
		}

		if (tdb == NULL)
		{
			if (log)
			{
				log->Add("Resultado da consulta nulo");
				return (false);
			}
		}


		if (tdb->RecordCount == 0)
			return (true);
		// limpa lista
		LimpaTList(paramConf->LisChavesIF);
		// atribui referências p/ os campos a serem pesquisados
		sdi_fld.codChave = tdb->FieldByName("codChave");
		// percorre registros da tabela
		for (tdb->First(); !tdb->Eof; tdb->Next())
		{ // preenche dados
			sdi.codChave = sdi_fld.codChave->AsString;
			// cria objeto strDadoChaveTelecom
			if (CriaDadoChaveIF(sdi, paramConf->LisChavesIF) == NULL)
			{
                if (log)
				{
					log->Add("CriaDadoChaveIF() nulo");
				}
				EncerraQuery(tdb);
				return (false);
			}
		}
		EncerraQuery(tdb);
		return (true);
	}
	catch (Exception &e)
	{
        if (log)
		{
			log->Add(__PRETTY_FUNCTION__);
			log->Add("Erro detectado: " + e.Message);
		}
		return false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::LeDadoChaveTelecom(void)
{
	// variáveis locais
	AnsiString qryTxt;
	TADOQuery *tdb = NULL;
	sdi_ChaveTelecom sdi;
	sdi_Fld_ChaveTelecom sdi_fld;
	VTLog *log = (VTLog*)apl->GetObject(__classid(VTLog));

	try
	{ // monta sql
		qryTxt.sprintf("SELECT * FROM ChavesTelecom;");
		// executa query
		tdb = ExecutaQuery(qryTxt);
		if (tdb == NULL)
			return (false);

		if (tdb->RecordCount == 0)
			return (true);
		// limpa lista
		LimpaTList(paramConf->LisChavesTelecom);
		// atribui referências p/ os campos a serem pesquisados
		sdi_fld.codChave = tdb->FieldByName("codChave");
		// percorre registros da tabela
		for (tdb->First(); !tdb->Eof; tdb->Next())
		{ // preenche dados
			sdi.codChave = sdi_fld.codChave->AsString;
			// cria objeto strDadoChaveTelecom
			if (CriaDadoChaveTelecom(sdi, paramConf->LisChavesTelecom) == NULL)
			{
				EncerraQuery(tdb);
				return (false);
			}
		}
		EncerraQuery(tdb);
		return (true);
	}
	catch (Exception &e)
	{
		if (log)
		{
			log->Add(__PRETTY_FUNCTION__);
			log->Add("Erro detectado: " + e.Message);
		}
		return false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::LeDadoParamBloco(void)
{
	// variáveis locais
	AnsiString qryTxt;
	TADOQuery *tdb = NULL;
	sdi_ParamBloco sdi;
	sdi_Fld_ParamBloco sdi_fld;
	bool flagCampo = ExisteCampoTabela("ParamBloco", "ptpre");
	bool flagBarraChave = ExisteCampoTabela("ParamBloco", "codBarraChave");
	VTLog *log = (VTLog*)apl->GetObject(__classid(VTLog));

	try
	{ // limpa lista
		LimpaTList(paramConf->LisParamBloco);
		// monta sql
		qryTxt.sprintf("SELECT * FROM ParamBloco;");
		// executa query
		tdb = ExecutaQuery(qryTxt);
		if (tdb == NULL)
			return (false);
		// atribui referências p/ os campos a serem pesquisados
		sdi_fld.codChave = tdb->FieldByName("codChave");
		sdi_fld.lambda = tdb->FieldByName("lambda");
		sdi_fld.trep = tdb->FieldByName("trep");
		if (flagCampo)
		{
			sdi_fld.ptpre = tdb->FieldByName("ptpre");
			sdi_fld.ptdlo = tdb->FieldByName("ptdlo");
			sdi_fld.ptexe = tdb->FieldByName("ptexe");
		}
		if (flagBarraChave)
		{
			sdi_fld.codBarraChave = tdb->FieldByName("codBarraChave");
		}

		// percorre registros da tabela

		sdi.ptpre = sdi.ptdlo = sdi.ptexe = 0.0;
		for (tdb->First(); !tdb->Eof; tdb->Next())
		{ // preenche dados
			sdi.codChave = sdi_fld.codChave->AsString;
			sdi.lambda = sdi_fld.lambda->AsFloat;
			sdi.trep = sdi_fld.trep->AsFloat;

			if (flagCampo)
			{
				sdi.ptpre = sdi_fld.ptpre->AsFloat;
				sdi.ptdlo = sdi_fld.ptdlo->AsFloat;
				sdi.ptexe = sdi_fld.ptexe->AsFloat;
			}
			// código da barra de referência para definição do bloco hk201802a
			sdi.codBarraChave = "";
			if (flagBarraChave)
				sdi.codBarraChave = sdi_fld.codBarraChave->AsString;
			// cria objeto strDadoParamBloco
			if (CriaDadoParamBloco(sdi, paramConf->LisParamBloco) == NULL)
			{
				EncerraQuery(tdb);
				return (false);
			}
		}
		EncerraQuery(tdb);

		return (true);
	}
	catch (Exception &e)
	{
		if (log)
		{
			log->Add(__PRETTY_FUNCTION__);
			log->Add("Erro detectado: " + e.Message);
		}
		return false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::LeDadoParamCarga(void)
{
	// variáveis locais
	AnsiString qryTxt;
	TADOQuery *tdb = NULL;
	sdi_ParamCarga sdi;
	sdi_Fld_ParamCarga sdi_fld;
	bool flagCampo = ExisteCampoTabela("ParamCarga", "ptpre");
	VTLog *log = (VTLog*)apl->GetObject(__classid(VTLog));

	try
	{ // limpa lista
		LimpaTList(paramConf->LisParamCarga);
		// monta sql
		qryTxt.sprintf("SELECT * FROM ParamCarga;");
		// executa query
		tdb = ExecutaQuery(qryTxt);
		if (tdb == NULL)
			return (false);
		// atribui referências p/ os campos a serem pesquisados
		sdi_fld.codCarga = tdb->FieldByName("codCarga");
		sdi_fld.lambda = tdb->FieldByName("lambda");
		sdi_fld.trep = tdb->FieldByName("trep");
		// hk201710IF
		if (flagCampo)
		{
			sdi_fld.ptpre = tdb->FieldByName("ptpre");
			sdi_fld.ptdlo = tdb->FieldByName("ptdlo");
			sdi_fld.ptexe = tdb->FieldByName("ptexe");
		}
		// percorre registros da tabela
		sdi.ptpre = sdi.ptdlo = sdi.ptexe = 0.0;
		for (tdb->First(); !tdb->Eof; tdb->Next())
		{ // preenche dados
			sdi.codCarga = sdi_fld.codCarga->AsString;
			sdi.lambda = sdi_fld.lambda->AsFloat;
			sdi.trep = sdi_fld.trep->AsFloat;
			// hk201710IF
			if (flagCampo)
			{
				sdi.ptpre = sdi_fld.ptpre->AsFloat;
				sdi.ptdlo = sdi_fld.ptdlo->AsFloat;
				sdi.ptexe = sdi_fld.ptexe->AsFloat;
			}

			// cria objeto strDadoParamCarga
			if (CriaDadoParamCarga(sdi, paramConf->LisParamCarga) == NULL)
			{
				EncerraQuery(tdb);
				return (false);
			}
		}
		EncerraQuery(tdb);
		return (true);
	}
	catch (Exception &e)
	{
		if (log)
		{
			log->Add(__PRETTY_FUNCTION__);
			log->Add("Erro detectado: " + e.Message);
		}
		return false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::LeDadoParamEqpto(TStringList *lisCodRede)
{
	// variáveis locais
	AnsiString qryTxt;
	TList *lisParamEqpto;
	AnsiString codRede;
	TStringList *slisConsulta;
	int num_consulta = 1;
	int limite = 10;
	int ind_rede = 0;
	bool sucesso = false;
	VTLog *log = (VTLog*)apl->GetObject(__classid(VTLog));

	try
	{ // cria slisConsulta
		slisConsulta = new TStringList();
		// inicia slisConsulta
		slisConsulta->Clear();
		// guarda lisParamEqpto
		lisParamEqpto = paramConf->LisParamEqpto;
		// limpa lista
		LimpaTList(lisParamEqpto);
		// monta sql
		if (lisCodRede == NULL)
		{
			qryTxt.sprintf("SELECT * FROM ParamEqpto;");
			slisConsulta->Add(qryTxt);
			sucesso = LeDadoParamEqptoLimitado(slisConsulta, lisParamEqpto);
		}
		else
		{
			if (lisCodRede->Count == 0)
			{
				delete slisConsulta;
				return (false);
			}
			// inicia
			ind_rede = 0;
			// verifica quantidade de redes
if(log) log->Add("lisCodRede->Count = " + IntToStr(lisCodRede->Count));
			if (lisCodRede->Count >= limite)
			{ // determina quantidade de consultas, usando limite de redes
				num_consulta = int(lisCodRede->Count / limite);
if(log) log->Add("num_consulta = " + IntToStr(num_consulta));
				for (int n = 0; n < num_consulta; n++) //cria X SQLs com a quantidade de redes no OR defindas pelo limte
				{
					qryTxt.sprintf("SELECT ParamEqpto.codRede, * FROM ParamEqpto WHERE ((");
					// primeira rede da consulta n
					codRede = lisCodRede->Strings[ind_rede];
					qryTxt.cat_sprintf("[ParamEqpto]![codRede]='%s'", codRede.c_str());
					//verifica se alcançou total de redes
					ind_rede++;
					if (ind_rede < lisCodRede->Count)
					{// demais redes até limite (1 a 9)
						for(int nr = 1; nr < limite; nr++) //coloca o numero de redes no OR conforme definido pelo limete
						{
							if (ind_rede == lisCodRede->Count)
                                break;
							codRede = lisCodRede->Strings[ind_rede];
							qryTxt.cat_sprintf(" Or [ParamEqpto]![codRede]='%s'", codRede.c_str());
                            ind_rede++;
						}
					}
					qryTxt.cat_sprintf("));");
if(log) log->Add("SQL[" + IntToStr(n) + "] = " + (qryTxt));
					slisConsulta->Add(qryTxt);
				}
			}
if(log) log->Add("slisConsulta->Count = " + IntToStr(slisConsulta->Count));
			// verifica se há mais 1 consulta a ser feita
			num_consulta = (lisCodRede->Count % limite);
if(log) log->Add("lisCodRede->Count % limite = " + IntToStr(num_consulta));
if(log) log->Add("ind_rede = " + IntToStr(ind_rede));
			if (num_consulta > 0)
			{
				qryTxt.sprintf("SELECT ParamEqpto.codRede, * FROM ParamEqpto WHERE ((");
				// primeira rede da consulta n
				codRede = lisCodRede->Strings[ind_rede];
				qryTxt.cat_sprintf("[ParamEqpto]![codRede]='%s'", codRede.c_str());
				// demais redes até o fim
				ind_rede++;
				for (int nr = ind_rede; nr < lisCodRede->Count; nr++, ind_rede++)
				{
					codRede = lisCodRede->Strings[ind_rede];
					qryTxt.cat_sprintf(" Or [ParamEqpto]![codRede]='%s'", codRede.c_str());
				}
				qryTxt.cat_sprintf("));");
				slisConsulta->Add(qryTxt);
			}
			sucesso = LeDadoParamEqptoLimitado(slisConsulta, lisParamEqpto);
		}
		delete slisConsulta;
		return (sucesso);
	}
	catch (Exception &e)
	{
		if (log)
		{
			log->Add(__PRETTY_FUNCTION__);
			log->Add("Erro detectado: " + e.Message);
		}
		return false;
	}
	// qryTxt.sprintf("
	// WHERE (([ParamEqpto]![codRede]="RDUT  0102" Or [ParamEqpto]![codRede]="RDUT  0103"));
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::LeDadoParamEqptoLimitado(TStringList *slisConsulta,
	TList *lisParamEqpto)
{
	// variáveis locais
	AnsiString qryTxt;
	TADOQuery *tdb = NULL;
	sdi_ParamEqpto sdi;
	sdi_Fld_ParamEqpto sdi_fld;
	VTLog *log = (VTLog*)apl->GetObject(__classid(VTLog));
	// hk201705: define se a tabela já está atualizada com campos ptpre, ptdlo e ptexe
	bool flagCamposIF = false;
	TField *field;
	AnsiString aux;

	try
	{ // executa query
		for (int n = 0; n < slisConsulta->Count; n++)
		{
			qryTxt = slisConsulta->Strings[n];
			tdb = ExecutaQuery(qryTxt);

			if (tdb == NULL)
			{
				delete slisConsulta;
				return (false);
			}

			for (int nn = 0; nn < tdb->FieldList->Count; nn++)
			{
				field = tdb->FieldList->Fields[nn];
				aux = field->FieldName;
				if (aux.AnsiCompare("ptpre") == 0)
				{
					flagCamposIF = true;
					break;
				}
			}


			int indexRegistrosSelecionados = 1;


			// atribui referências p/ os campos a serem pesquisados
			sdi_fld.codEqpto = tdb->FieldByName("codEqpto");
			sdi_fld.codRede = tdb->FieldByName("codRede");
			sdi_fld.tipoRede = tdb->FieldByName("tipoRede");
			sdi_fld.ndiasObs = tdb->FieldByName("ndiasObs");
			// hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
			if (flagCamposIF == true)
			{
				sdi_fld.ptpre = tdb->FieldByName("ptpre");
				sdi_fld.ptdlo = tdb->FieldByName("ptdlo");
				sdi_fld.ptexe = tdb->FieldByName("ptexe");
			}

			sdi_fld.nf = tdb->FieldByName("nf");
			sdi_fld.nfr = tdb->FieldByName("nfr");
			sdi_fld.td = tdb->FieldByName("td");
			sdi_fld.cli = tdb->FieldByName("cli");
			sdi_fld.clih = tdb->FieldByName("clih");

			sdi_fld.nfMes1 = tdb->FieldByName("nfMes1");
			sdi_fld.nfMes2 = tdb->FieldByName("nfMes2");
			sdi_fld.nfMes3 = tdb->FieldByName("nfMes3");
			sdi_fld.nfMes4 = tdb->FieldByName("nfMes4");
			sdi_fld.nfMes5 = tdb->FieldByName("nfMes5");
			sdi_fld.nfMes6 = tdb->FieldByName("nfMes6");
			sdi_fld.nfMes7 = tdb->FieldByName("nfMes7");
			sdi_fld.nfMes8 = tdb->FieldByName("nfMes8");
			sdi_fld.nfMes9 = tdb->FieldByName("nfMes9");
			sdi_fld.nfMes10 = tdb->FieldByName("nfMes10");
			sdi_fld.nfMes11 = tdb->FieldByName("nfMes11");
			sdi_fld.nfMes12 = tdb->FieldByName("nfMes12");
			sdi_fld.nfrMes1 = tdb->FieldByName("nfrMes1");
			sdi_fld.nfrMes2 = tdb->FieldByName("nfrMes2");
			sdi_fld.nfrMes3 = tdb->FieldByName("nfrMes3");
			sdi_fld.nfrMes4 = tdb->FieldByName("nfrMes4");
			sdi_fld.nfrMes5 = tdb->FieldByName("nfrMes5");
			sdi_fld.nfrMes6 = tdb->FieldByName("nfrMes6");
			sdi_fld.nfrMes7 = tdb->FieldByName("nfrMes7");
			sdi_fld.nfrMes8 = tdb->FieldByName("nfrMes8");
			sdi_fld.nfrMes9 = tdb->FieldByName("nfrMes9");
			sdi_fld.nfrMes10 = tdb->FieldByName("nfrMes10");
			sdi_fld.nfrMes11 = tdb->FieldByName("nfrMes11");
			sdi_fld.nfrMes12 = tdb->FieldByName("nfrMes12");
			sdi_fld.tdMes1 = tdb->FieldByName("tdMes1");
			sdi_fld.tdMes2 = tdb->FieldByName("tdMes2");
			sdi_fld.tdMes3 = tdb->FieldByName("tdMes3");
			sdi_fld.tdMes4 = tdb->FieldByName("tdMes4");
			sdi_fld.tdMes5 = tdb->FieldByName("tdMes5");
			sdi_fld.tdMes6 = tdb->FieldByName("tdMes6");
			sdi_fld.tdMes7 = tdb->FieldByName("tdMes7");
			sdi_fld.tdMes8 = tdb->FieldByName("tdMes8");
			sdi_fld.tdMes9 = tdb->FieldByName("tdMes9");
			sdi_fld.tdMes10 = tdb->FieldByName("tdMes10");
			sdi_fld.tdMes11 = tdb->FieldByName("tdMes11");
			sdi_fld.tdMes12 = tdb->FieldByName("tdMes12");
			// hkmar
			sdi_fld.cliMes1 = tdb->FieldByName("cliMes1");
			sdi_fld.cliMes2 = tdb->FieldByName("cliMes2");
			sdi_fld.cliMes3 = tdb->FieldByName("cliMes3");
			sdi_fld.cliMes4 = tdb->FieldByName("cliMes4");
			sdi_fld.cliMes5 = tdb->FieldByName("cliMes5");
			sdi_fld.cliMes6 = tdb->FieldByName("cliMes6");
			sdi_fld.cliMes7 = tdb->FieldByName("cliMes7");
			sdi_fld.cliMes8 = tdb->FieldByName("cliMes8");
			sdi_fld.cliMes9 = tdb->FieldByName("cliMes9");
			sdi_fld.cliMes10 = tdb->FieldByName("cliMes10");
			sdi_fld.cliMes11 = tdb->FieldByName("cliMes11");
			sdi_fld.cliMes12 = tdb->FieldByName("cliMes12");

			// hkmar

			sdi_fld.clihMes1 = tdb->FieldByName("clihMes1");
			sdi_fld.clihMes2 = tdb->FieldByName("clihMes2");
			sdi_fld.clihMes3 = tdb->FieldByName("clihMes3");
			sdi_fld.clihMes4 = tdb->FieldByName("clihMes4");
			sdi_fld.clihMes5 = tdb->FieldByName("clihMes5");
			sdi_fld.clihMes6 = tdb->FieldByName("clihMes6");
			sdi_fld.clihMes7 = tdb->FieldByName("clihMes7");
			sdi_fld.clihMes8 = tdb->FieldByName("clihMes8");
			sdi_fld.clihMes9 = tdb->FieldByName("clihMes9");
			sdi_fld.clihMes10 = tdb->FieldByName("clihMes10");
			sdi_fld.clihMes11 = tdb->FieldByName("clihMes11");
			sdi_fld.clihMes12 = tdb->FieldByName("clihMes12");

			sdi_fld.nfHora0 = tdb->FieldByName("nfHora0");
			sdi_fld.nfHora1 = tdb->FieldByName("nfHora1");
			sdi_fld.nfHora2 = tdb->FieldByName("nfHora2");
			sdi_fld.nfHora3 = tdb->FieldByName("nfHora3");
			sdi_fld.nfHora4 = tdb->FieldByName("nfHora4");
			sdi_fld.nfHora5 = tdb->FieldByName("nfHora5");
			sdi_fld.nfHora6 = tdb->FieldByName("nfHora6");
			sdi_fld.nfHora7 = tdb->FieldByName("nfHora7");
			sdi_fld.nfHora8 = tdb->FieldByName("nfHora8");
			sdi_fld.nfHora9 = tdb->FieldByName("nfHora9");
			sdi_fld.nfHora10 = tdb->FieldByName("nfHora10");
			sdi_fld.nfHora11 = tdb->FieldByName("nfHora11");
			sdi_fld.nfHora12 = tdb->FieldByName("nfHora12");
			sdi_fld.nfHora13 = tdb->FieldByName("nfHora13");
			sdi_fld.nfHora14 = tdb->FieldByName("nfHora14");
			sdi_fld.nfHora15 = tdb->FieldByName("nfHora15");
			sdi_fld.nfHora16 = tdb->FieldByName("nfHora16");
			sdi_fld.nfHora17 = tdb->FieldByName("nfHora17");
			sdi_fld.nfHora18 = tdb->FieldByName("nfHora18");
			sdi_fld.nfHora19 = tdb->FieldByName("nfHora19");
			sdi_fld.nfHora20 = tdb->FieldByName("nfHora20");
			sdi_fld.nfHora21 = tdb->FieldByName("nfHora21");
			sdi_fld.nfHora22 = tdb->FieldByName("nfHora22");
			sdi_fld.nfHora23 = tdb->FieldByName("nfHora23");
			sdi_fld.nfrHora0 = tdb->FieldByName("nfrHora0");
			sdi_fld.nfrHora1 = tdb->FieldByName("nfrHora1");
			sdi_fld.nfrHora2 = tdb->FieldByName("nfrHora2");
			sdi_fld.nfrHora3 = tdb->FieldByName("nfrHora3");
			sdi_fld.nfrHora4 = tdb->FieldByName("nfrHora4");
			sdi_fld.nfrHora5 = tdb->FieldByName("nfrHora5");
			sdi_fld.nfrHora6 = tdb->FieldByName("nfrHora6");
			sdi_fld.nfrHora7 = tdb->FieldByName("nfrHora7");
			sdi_fld.nfrHora8 = tdb->FieldByName("nfrHora8");
			sdi_fld.nfrHora9 = tdb->FieldByName("nfrHora9");
			sdi_fld.nfrHora10 = tdb->FieldByName("nfrHora10");
			sdi_fld.nfrHora11 = tdb->FieldByName("nfrHora11");
			sdi_fld.nfrHora12 = tdb->FieldByName("nfrHora12");
			sdi_fld.nfrHora13 = tdb->FieldByName("nfrHora13");
			sdi_fld.nfrHora14 = tdb->FieldByName("nfrHora14");
			sdi_fld.nfrHora15 = tdb->FieldByName("nfrHora15");
			sdi_fld.nfrHora16 = tdb->FieldByName("nfrHora16");
			sdi_fld.nfrHora17 = tdb->FieldByName("nfrHora17");
			sdi_fld.nfrHora18 = tdb->FieldByName("nfrHora18");
			sdi_fld.nfrHora19 = tdb->FieldByName("nfrHora19");
			sdi_fld.nfrHora20 = tdb->FieldByName("nfrHora20");
			sdi_fld.nfrHora21 = tdb->FieldByName("nfrHora21");
			sdi_fld.nfrHora22 = tdb->FieldByName("nfrHora22");
			sdi_fld.nfrHora23 = tdb->FieldByName("nfrHora23");

			// percorre registros da tabela
			for (tdb->First(); !tdb->Eof; tdb->Next())
			{

				++indexRegistrosSelecionados;

				// preenche dados
				sdi.codEqpto = sdi_fld.codEqpto->AsString;
				sdi.codRede = sdi_fld.codRede->AsString;
				sdi.tipoRede = sdi_fld.tipoRede->AsString;
				sdi.ndiasObs = sdi_fld.ndiasObs->AsFloat;

				// hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
				if (flagCamposIF == true)
				{
					sdi.ptpre = sdi_fld.ptpre->AsFloat;
					sdi.ptdlo = sdi_fld.ptdlo->AsFloat;
					sdi.ptexe = sdi_fld.ptexe->AsFloat;
				}
				else
				{
					sdi.ptpre = 0;
					sdi.ptdlo = 0;
					sdi.ptexe = 0;
				}

				sdi.nf = sdi_fld.nf->AsFloat;
				sdi.nfr = sdi_fld.nfr->AsFloat;
				sdi.td = sdi_fld.td->AsFloat;

				// hkmar
				sdi.cli = sdi_fld.cli->AsFloat;
				sdi.clih = sdi_fld.clih->AsFloat;

				sdi.nfMes[0] = sdi_fld.nfMes1->AsFloat;
				sdi.nfMes[1] = sdi_fld.nfMes2->AsFloat;
				sdi.nfMes[2] = sdi_fld.nfMes3->AsFloat;
				sdi.nfMes[3] = sdi_fld.nfMes4->AsFloat;
				sdi.nfMes[4] = sdi_fld.nfMes5->AsFloat;
				sdi.nfMes[5] = sdi_fld.nfMes6->AsFloat;
				sdi.nfMes[6] = sdi_fld.nfMes7->AsFloat;
				sdi.nfMes[7] = sdi_fld.nfMes8->AsFloat;
				sdi.nfMes[8] = sdi_fld.nfMes9->AsFloat;
				sdi.nfMes[9] = sdi_fld.nfMes10->AsFloat;
				sdi.nfMes[10] = sdi_fld.nfMes11->AsFloat;
				sdi.nfMes[11] = sdi_fld.nfMes12->AsFloat;
				sdi.nfrMes[0] = sdi_fld.nfrMes1->AsFloat;
				sdi.nfrMes[1] = sdi_fld.nfrMes2->AsFloat;
				sdi.nfrMes[2] = sdi_fld.nfrMes3->AsFloat;
				sdi.nfrMes[3] = sdi_fld.nfrMes4->AsFloat;
				sdi.nfrMes[4] = sdi_fld.nfrMes5->AsFloat;
				sdi.nfrMes[5] = sdi_fld.nfrMes6->AsFloat;
				sdi.nfrMes[6] = sdi_fld.nfrMes7->AsFloat;
				sdi.nfrMes[7] = sdi_fld.nfrMes8->AsFloat;
				sdi.nfrMes[8] = sdi_fld.nfrMes9->AsFloat;
				sdi.nfrMes[9] = sdi_fld.nfrMes10->AsFloat;
				sdi.nfrMes[10] = sdi_fld.nfrMes11->AsFloat;
				sdi.nfrMes[11] = sdi_fld.nfrMes12->AsFloat;
				sdi.tdMes[0] = sdi_fld.tdMes1->AsFloat;
				sdi.tdMes[1] = sdi_fld.tdMes2->AsFloat;
				sdi.tdMes[2] = sdi_fld.tdMes3->AsFloat;
				sdi.tdMes[3] = sdi_fld.tdMes4->AsFloat;
				sdi.tdMes[4] = sdi_fld.tdMes5->AsFloat;
				sdi.tdMes[5] = sdi_fld.tdMes6->AsFloat;
				sdi.tdMes[6] = sdi_fld.tdMes7->AsFloat;
				sdi.tdMes[7] = sdi_fld.tdMes8->AsFloat;
				sdi.tdMes[8] = sdi_fld.tdMes9->AsFloat;
				sdi.tdMes[9] = sdi_fld.tdMes10->AsFloat;
				sdi.tdMes[10] = sdi_fld.tdMes11->AsFloat;
				sdi.tdMes[11] = sdi_fld.tdMes12->AsFloat;

				// hkmar
				sdi.climes[0] = sdi_fld.cliMes1->AsFloat;
				sdi.climes[1] = sdi_fld.cliMes2->AsFloat;
				sdi.climes[2] = sdi_fld.cliMes3->AsFloat;
				sdi.climes[3] = sdi_fld.cliMes4->AsFloat;
				sdi.climes[4] = sdi_fld.cliMes5->AsFloat;
				sdi.climes[5] = sdi_fld.cliMes6->AsFloat;
				sdi.climes[6] = sdi_fld.cliMes7->AsFloat;
				sdi.climes[7] = sdi_fld.cliMes8->AsFloat;
				sdi.climes[8] = sdi_fld.cliMes9->AsFloat;
				sdi.climes[9] = sdi_fld.cliMes10->AsFloat;
				sdi.climes[10] = sdi_fld.cliMes11->AsFloat;
				sdi.climes[11] = sdi_fld.cliMes12->AsFloat;
				// hkmar
				sdi.clihmes[0] = sdi_fld.clihMes1->AsFloat;
				sdi.clihmes[1] = sdi_fld.clihMes2->AsFloat;
				sdi.clihmes[2] = sdi_fld.clihMes3->AsFloat;
				sdi.clihmes[3] = sdi_fld.clihMes4->AsFloat;
				sdi.clihmes[4] = sdi_fld.clihMes5->AsFloat;
				sdi.clihmes[5] = sdi_fld.clihMes6->AsFloat;
				sdi.clihmes[6] = sdi_fld.clihMes7->AsFloat;
				sdi.clihmes[7] = sdi_fld.clihMes8->AsFloat;
				sdi.clihmes[8] = sdi_fld.clihMes9->AsFloat;
				sdi.clihmes[9] = sdi_fld.clihMes10->AsFloat;
				sdi.clihmes[10] = sdi_fld.clihMes11->AsFloat;
				sdi.clihmes[11] = sdi_fld.clihMes12->AsFloat;

				sdi.nfHora[0] = sdi_fld.nfHora0->AsFloat;
				sdi.nfHora[1] = sdi_fld.nfHora1->AsFloat;
				sdi.nfHora[2] = sdi_fld.nfHora2->AsFloat;
				sdi.nfHora[3] = sdi_fld.nfHora3->AsFloat;
				sdi.nfHora[4] = sdi_fld.nfHora4->AsFloat;
				sdi.nfHora[5] = sdi_fld.nfHora5->AsFloat;
				sdi.nfHora[6] = sdi_fld.nfHora6->AsFloat;
				sdi.nfHora[7] = sdi_fld.nfHora7->AsFloat;
				sdi.nfHora[8] = sdi_fld.nfHora8->AsFloat;
				sdi.nfHora[9] = sdi_fld.nfHora9->AsFloat;
				sdi.nfHora[10] = sdi_fld.nfHora10->AsFloat;
				sdi.nfHora[11] = sdi_fld.nfHora11->AsFloat;
				sdi.nfHora[12] = sdi_fld.nfHora12->AsFloat;
				sdi.nfHora[13] = sdi_fld.nfHora13->AsFloat;
				sdi.nfHora[14] = sdi_fld.nfHora14->AsFloat;
				sdi.nfHora[15] = sdi_fld.nfHora15->AsFloat;
				sdi.nfHora[16] = sdi_fld.nfHora16->AsFloat;
				sdi.nfHora[17] = sdi_fld.nfHora17->AsFloat;
				sdi.nfHora[18] = sdi_fld.nfHora18->AsFloat;
				sdi.nfHora[19] = sdi_fld.nfHora19->AsFloat;
				sdi.nfHora[20] = sdi_fld.nfHora20->AsFloat;
				sdi.nfHora[21] = sdi_fld.nfHora21->AsFloat;
				sdi.nfHora[22] = sdi_fld.nfHora22->AsFloat;
				sdi.nfHora[23] = sdi_fld.nfHora23->AsFloat;
				sdi.nfrHora[0] = sdi_fld.nfrHora0->AsFloat;
				sdi.nfrHora[1] = sdi_fld.nfrHora1->AsFloat;
				sdi.nfrHora[2] = sdi_fld.nfrHora2->AsFloat;
				sdi.nfrHora[3] = sdi_fld.nfrHora3->AsFloat;
				sdi.nfrHora[4] = sdi_fld.nfrHora4->AsFloat;
				sdi.nfrHora[5] = sdi_fld.nfrHora5->AsFloat;
				sdi.nfrHora[6] = sdi_fld.nfrHora6->AsFloat;
				sdi.nfrHora[7] = sdi_fld.nfrHora7->AsFloat;
				sdi.nfrHora[8] = sdi_fld.nfrHora8->AsFloat;
				sdi.nfrHora[9] = sdi_fld.nfrHora9->AsFloat;
				sdi.nfrHora[10] = sdi_fld.nfrHora10->AsFloat;
				sdi.nfrHora[11] = sdi_fld.nfrHora11->AsFloat;
				sdi.nfrHora[12] = sdi_fld.nfrHora12->AsFloat;
				sdi.nfrHora[13] = sdi_fld.nfrHora13->AsFloat;
				sdi.nfrHora[14] = sdi_fld.nfrHora14->AsFloat;
				sdi.nfrHora[15] = sdi_fld.nfrHora15->AsFloat;
				sdi.nfrHora[16] = sdi_fld.nfrHora16->AsFloat;
				sdi.nfrHora[17] = sdi_fld.nfrHora17->AsFloat;
				sdi.nfrHora[18] = sdi_fld.nfrHora18->AsFloat;
				sdi.nfrHora[19] = sdi_fld.nfrHora19->AsFloat;
				sdi.nfrHora[20] = sdi_fld.nfrHora20->AsFloat;
				sdi.nfrHora[21] = sdi_fld.nfrHora21->AsFloat;
				sdi.nfrHora[22] = sdi_fld.nfrHora22->AsFloat;
				sdi.nfrHora[23] = sdi_fld.nfrHora23->AsFloat;

				// cria objeto strDadoParamEqpto
				if (CriaDadoParamEqpto(sdi, lisParamEqpto) == NULL)
				{
					EncerraQuery(tdb);
					if (log)
					{
						log->Add("Total de registros percorridos: " + IntToStr(indexRegistrosSelecionados));
						log->Add("Total de itens da lista: " + IntToStr(lisParamEqpto->Count));
					}
					return (false);
				}
			}

			if (log)
			{
				log->Add("Total de registros percorridos: " + IntToStr(indexRegistrosSelecionados));
				log->Add("Total de itens da lista: " + IntToStr(lisParamEqpto->Count));
			}

			EncerraQuery(tdb);
		}
		return (true);
	}
	catch (Exception &e)
	{
		if (log)
		{
			log->Add(__PRETTY_FUNCTION__);
			log->Add("Erro detectado: " + e.Message);
		}
		return false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::LeDadoParamGerais(void)
{
	// variáveis locais
	AnsiString qryTxt;
	TADOQuery *tdb = NULL;
	sdi_ParamGerais sdi;
	sdi_Fld_ParamGerais sdi_fld;
	VTLog *log = (VTLog*)apl->GetObject(__classid(VTLog));

	try
	{ // monta sql
		qryTxt.sprintf("SELECT * FROM ParamGerais;");
		// executa query
		tdb = ExecutaQuery(qryTxt);

		// gh201806: define se a tabela já está atualizada com o campo red_tprep_IF
		bool flagTprepIF = false;
		TField *field;
		AnsiString aux;
		for (int nn = 0; nn < tdb->FieldList->Count; nn++)
		{
			field = tdb->FieldList->Fields[nn];
			aux = field->FieldName;
			if (aux.AnsiCompare("red_tprep_IF") == 0)
			{
				flagTprepIF = true;
				break;
			}
		}
		if (tdb == NULL)
			return (false);
		// atribui referências p/ os campos a serem pesquisados
		sdi_fld.flagRecurso = tdb->FieldByName("flagRecurso");
		sdi_fld.flagRetorno = tdb->FieldByName("flagRetorno");
		sdi_fld.tlocaliza = tdb->FieldByName("tlocaliza");
		sdi_fld.tman_manual = tdb->FieldByName("tman_manual");
		sdi_fld.tman_auto = tdb->FieldByName("tman_auto");
		sdi_fld.pu_lim = tdb->FieldByName("pu_lim");
		sdi_fld.percDefTrans = tdb->FieldByName("percDefTrans");
		sdi_fld.iProfReliga = tdb->FieldByName("iProfReliga");
		sdi_fld.tlimIC = tdb->FieldByName("tlimIC");
		sdi_fld.lambda = tdb->FieldByName("lambda");
		sdi_fld.trep = tdb->FieldByName("trep");
		if (ExisteCampoTabela("ParamGerais", "vindisp")) // hk201808: indisp
				sdi_fld.vindisp = tdb->FieldByName("vindisp");
		// gh201806: proteção para verificar se o paramConf já possui o campo de % redução de tempo de preparo
		// por alocação de IF em fusível
		if (flagTprepIF == true)
		{
			sdi_fld.red_tprep_IF = tdb->FieldByName("red_tprep_IF");
		}
		// percorre registros da tabela
		for (tdb->First(); !tdb->Eof; tdb->Next())
		{ // preenche dados
			sdi.flagRecurso = sdi_fld.flagRecurso->AsInteger ? true : false;
			sdi.flagRetorno = sdi_fld.flagRetorno->AsInteger ? true : false;
			sdi.tlocaliza = sdi_fld.tlocaliza->AsFloat;
			sdi.tman_manual = sdi_fld.tman_manual->AsFloat;
			sdi.tman_auto = sdi_fld.tman_auto->AsFloat;
			sdi.pu_lim = sdi_fld.pu_lim->AsFloat;
			sdi.percDefTrans = sdi_fld.percDefTrans->AsFloat;
			sdi.iProfReliga = sdi_fld.iProfReliga->AsInteger;
			sdi.tlimIC = sdi_fld.tlimIC->AsFloat;
			// hk201707: sem inicialização forçada
			// double lambdaAux = sdi_fld.lambda->AsFloat;
			// sdi.lambda        = (IsDoubleZero(lambdaAux) ? 0.1 : lambdaAux);
			sdi.lambda = sdi_fld.lambda->AsFloat;
			sdi.trep = sdi_fld.trep->AsFloat;
			// gh201806: acrescimo do campo de % redução de tempo de preparo por alocação de IF em fusível
			if (flagTprepIF == true)
			{
				sdi.red_tprep_IF = sdi_fld.red_tprep_IF->AsFloat;
			}
			else
				sdi.red_tprep_IF = paramConf->ParamGerais->red_tprep_IF;
			// hk201808: reformula: 0.7;

			if (ExisteCampoTabela("ParamGerais", "vindisp")) // hk201808: indisp
					sdi.vindisp = sdi_fld.vindisp->AsFloat;
			else
				sdi.vindisp = paramConf->EqptoIndispPadrao->vindisp;

			// atualiza informação parâmetros gerais, paramConf->ParamGerais
			if (AtualizaDadoParamGerais(sdi) == NULL)
			{
				EncerraQuery(tdb);
				return (false);
			}
		}
		EncerraQuery(tdb);
		return (true);
	}
	catch (Exception &e)
	{
		if (log)
		{
			log->Add(__PRETTY_FUNCTION__);
			log->Add("Erro detectado: " + e.Message);
		}
		return false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::LeDadoParamIndisp(void) // hk201808: indisp
{
	// variáveis locais
	AnsiString qryTxt;
	TADOQuery *tdb = NULL;
	AnsiString codEqpto;
	double vindisp;
	strDadoEqptoIndisp *dadoEqptoIndisp;
	VTLog *log = (VTLog*)apl->GetObject(__classid(VTLog));

	try
	{
		if (!ExisteTabela("ParamIndisp"))
		{
			if(log)
			{
				log->Add("Não existe tabela ParamIndisp");
            }
			return (true);
		}

		// Inicia lista de paramConf
		LimpaTList(paramConf->LisEqptoIndisp);

		// monta sql
		qryTxt.sprintf("SELECT * FROM ParamIndisp;");
		// executa query
		tdb = ExecutaQuery(qryTxt);
		if (tdb == NULL)
			return (false);
		if (tdb->RecordCount == 0)
			return (true);
		// percorre registros da tabela
		for (tdb->First(); !tdb->Eof; tdb->Next())
		{
			codEqpto = tdb->FieldByName("codEqpto")->AsString;
			vindisp = tdb->FieldByName("vindisp")->AsFloat;
			dadoEqptoIndisp = NULL;
			for (int n = 0; n < paramConf->LisEqptoIndisp->Count; n++)
			{
				dadoEqptoIndisp = (strDadoEqptoIndisp*)paramConf->LisEqptoIndisp->Items[n];
				if (dadoEqptoIndisp->cod_eqpto.AnsiCompare(codEqpto) == 0)
					break;
				dadoEqptoIndisp = NULL;
			}
			if (dadoEqptoIndisp == NULL)
			{
				paramConf->LisEqptoIndisp->Add(dadoEqptoIndisp = new strDadoEqptoIndisp);
				dadoEqptoIndisp->cod_eqpto = codEqpto;
				dadoEqptoIndisp->eqpto = NULL;
			}
			dadoEqptoIndisp->vindisp = vindisp;
		}
		return true;
	}
	catch (Exception &e)
	{
		if (log)
		{
			log->Add(__PRETTY_FUNCTION__);
			log->Add("Erro detectado: " + e.Message);
		}
		return false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::LeDadoParamRede(void)
{
	// variáveis locais
	AnsiString qryTxt;
	TADOQuery *tdb = NULL;
	sdi_ParamRede sdi;
	sdi_Fld_ParamRede sdi_fld;
	VTLog *log = (VTLog*)apl->GetObject(__classid(VTLog));

	try
	{ // monta sql
		qryTxt.sprintf("SELECT * FROM ParamRede;");
		// executa query
		tdb = ExecutaQuery(qryTxt);
		if (tdb == NULL)
			return (false);
		if (tdb->RecordCount == 0)
			return (true);
		// limpa lista
		// LimpaTList(paramConf->LisParamRede);

		// atribui referências p/ os campos a serem pesquisados
		sdi_fld.codRede = tdb->FieldByName("codRede");
		sdi_fld.lambda = tdb->FieldByName("lambda");
		sdi_fld.trep = tdb->FieldByName("trep");
		// percorre registros da tabela
		for (tdb->First(); !tdb->Eof; tdb->Next())
		{ // preenche dados
			sdi.codRede = sdi_fld.codRede->AsString;
			sdi.lambda = sdi_fld.lambda->AsFloat;
			sdi.trep = sdi_fld.trep->AsFloat;
			// cria objeto strDadoParamRede
			if (CriaDadoParamRede(sdi, paramConf->LisParamRede) == NULL)
			{
				EncerraQuery(tdb);
				return (false);
			}
		}
		EncerraQuery(tdb);
		return (true);
	}
	catch (Exception &e)
	{
		if (log)
		{
			log->Add(__PRETTY_FUNCTION__);
			log->Add("Erro detectado: " + e.Message);
		}
		return false;
	}
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TBDadoParam::LeDadoTexto(FILE* fp, bool &flagFIMLINHA, bool &flagEOF)
{
	AnsiString x;
	char aux;

	flagEOF = false;
	flagFIMLINHA = false;

	x = "";
	aux = fgetc(fp);
	while ((aux != ';') && (aux != '\n') && (aux != EOF))
	{
		if (aux != '"')
			x += AnsiString(aux);
		aux = fgetc(fp);
	}
	if (aux == EOF)
		flagEOF = true;
	if (aux == '\n')
		flagFIMLINHA = true;

	return (x);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::LeDadoTipoEqProt(void)
{
	// variáveis locais
	AnsiString qryTxt;
	TADOQuery *tdb = NULL;
	sdi_TipoEqProt sdi;
	sdi_Fld_TipoEqProt sdi_fld;
	VTLog *log = (VTLog*)apl->GetObject(__classid(VTLog));

	try
	{ // monta sql
		qryTxt.sprintf("SELECT * FROM TipoEqProt;");
		// executa query
		tdb = ExecutaQuery(qryTxt);
		if (tdb == NULL)
			return (false);

		if (tdb->RecordCount == 0)
			return (false);
		// limpa lista
		LimpaTList(paramConf->LisTipoEqProt);
		// atribui referências p/ os campos a serem pesquisados
		sdi_fld.codTipoEqProt = tdb->FieldByName("codTipoEqProt");
		// percorre registros da tabela
		for (tdb->First(); !tdb->Eof; tdb->Next())
		{ // preenche dados
			sdi.codTipoEqProt = sdi_fld.codTipoEqProt->AsString;
			// cria objeto strDadoTipoEqProt
			if (CriaDadoTipoEqProt(sdi, paramConf->LisTipoEqProt) == NULL)
			{
				EncerraQuery(tdb);
				return (false);
			}
		}
		EncerraQuery(tdb);
		return (true);
	}
	catch (Exception &e)
	{
		if (log)
		{
			log->Add(__PRETTY_FUNCTION__);
			log->Add("Erro detectado: " + e.Message);
		}
		return false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::LeDadoVerifCircuito(void)
{
	// variáveis locais
	AnsiString qryTxt;
	TADOQuery *tdb = NULL;
	sdi_VerifCircuito sdi;
	sdi_Fld_VerifCircuito sdi_fld;
	VTVerifCircuito *verifCircuito;
	VTLog *log = (VTLog*)apl->GetObject(__classid(VTLog));

	try
	{ // monta sql
		qryTxt.sprintf("SELECT * FROM VerifCircuito;");
		// executa query
		tdb = ExecutaQuery(qryTxt);
		if (tdb == NULL)
			return (false);
		// Não destroi lista default antes de saber se existem elementos na base
		if (tdb->RecordCount == 0)
			return (true);

		// limpa lista
		LimpaTList(paramConf->LisVerifCircuito);
		// atribui referências p/ os campos a serem pesquisados
		sdi_fld.codCircuito = tdb->FieldByName("codCircuito");
		sdi_fld.decVerif = tdb->FieldByName("decVerif");
		sdi_fld.fecVerif = tdb->FieldByName("fecVerif");
		// percorre registros da tabela
		for (tdb->First(); !tdb->Eof; tdb->Next())
		{ // preenche dados
			sdi.codCircuito = sdi_fld.codCircuito->AsString;
			sdi.decVerif = sdi_fld.decVerif->AsFloat;
			sdi.fecVerif = sdi_fld.fecVerif->AsFloat;
			// cria objeto VTVerifCircuito
			if ((CriaDadoVerifCircuito(sdi, paramConf->LisVerifCircuito)) == NULL)
			{
				EncerraQuery(tdb);
				return (false);
			}
		}
		EncerraQuery(tdb);
		return (true);
	}
	catch (Exception &e)
	{
		if (log)
		{
			log->Add(__PRETTY_FUNCTION__);
			log->Add("Erro detectado: " + e.Message);
		}
		return false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::LeDadoVerifCjto(void)
{
	// variáveis locais
	AnsiString qryTxt;
	TADOQuery *tdb = NULL;
	sdi_VerifCjto sdi;
	sdi_Fld_VerifCjto sdi_fld;
	VTVerifCjto *verifCjto;
	VTLog *log = (VTLog*)apl->GetObject(__classid(VTLog));

	try
	{ // monta sql
		qryTxt.sprintf("SELECT * FROM VerifCjto;");
		// executa query
		tdb = ExecutaQuery(qryTxt);
		if (tdb == NULL)
			return (false);
		// Não destroi lista default antes de saber se existem elementos na base
		if (tdb->RecordCount == 0)
			return (true);

		// limpa lista
		LimpaTList(paramConf->LisVerifCjto);
		// atribui referências p/ os campos a serem pesquisados
		sdi_fld.codCjto = tdb->FieldByName("codCjto");
		sdi_fld.decVerif = tdb->FieldByName("decVerif");
		sdi_fld.fecVerif = tdb->FieldByName("fecVerif");
		// percorre registros da tabela
		for (tdb->First(); !tdb->Eof; tdb->Next())
		{ // preenche dados
			sdi.codCjto = sdi_fld.codCjto->AsString;
			sdi.decVerif = sdi_fld.decVerif->AsFloat;
			sdi.fecVerif = sdi_fld.fecVerif->AsFloat;
			// cria objeto VTVerifCjto
			if ((verifCjto = CriaDadoVerifCjto(sdi, paramConf->LisVerifCjto)) == NULL)
			{
				EncerraQuery(tdb);
				return (false);
			}
			// leitura das SEDs
			LeDadoVerifCjtoSe(verifCjto);
		}
		EncerraQuery(tdb);
		return (true);
	}
	catch (Exception &e)
	{
		if (log)
		{
			log->Add(__PRETTY_FUNCTION__);
			log->Add("Erro detectado: " + e.Message);
		}
		return false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::LeDadoVerifCjtoSe(VTVerifCjto *verifCjto)
{
	// variáveis locais
	AnsiString qryTxt;
	TADOQuery *tdb = NULL;
	sdi_VerifCjtoSe sdi;
	sdi_Fld_VerifCjtoSe sdi_fld;
	VTVerifSe *verifSe;
	VTLog *log = (VTLog*)apl->GetObject(__classid(VTLog));

	try
	{ // limpa lista
		verifCjto->LisVerifSe->Clear();
		// monta sql
		qryTxt.sprintf("SELECT * FROM VerifCjtoSe ");
		qryTxt.cat_sprintf("WHERE (codCjto='");
		qryTxt.cat_sprintf(verifCjto->Codigo.c_str());
		qryTxt.cat_sprintf("');");
		// executa query
		tdb = ExecutaQuery(qryTxt);
		if (tdb == NULL)
			return (false);
		// atribui referências p/ os campos a serem pesquisados
		sdi_fld.codCjto = tdb->FieldByName("codCjto");
		sdi_fld.codSe = tdb->FieldByName("codSe");
		// percorre registros da tabela
		for (tdb->First(); !tdb->Eof; tdb->Next())
		{ // preenche dados
			sdi.codCjto = sdi_fld.codCjto->AsString;
			sdi.codSe = sdi_fld.codSe->AsString;
			// verifica se existe VTVerifSe
			verifSe = ExisteVerifSe(sdi.codSe);
			// se não existe objeto
			if (verifSe == NULL)
			{ // cria novo objeto e insere em lisVerifSe
				verifSe = paramConf->CriaVerifSe();
				paramConf->LisVerifSe->Add(verifSe);
				// atribui propriedades
				verifSe->Codigo = sdi.codSe;
			}
			// insere na lista de se selecionada do conjunto
			verifCjto->LisVerifSe->Add(verifSe);
			// atribui propriedades
			verifSe->CodigoCjto = sdi.codCjto;
			verifSe->Conjunto = verifCjto;
			verifSe->Selecionada = true;
		}
		EncerraQuery(tdb);
		return (true);
	}
	catch (Exception &e)
	{
		if (log)
		{
			log->Add(__PRETTY_FUNCTION__);
			log->Add("Erro detectado: " + e.Message);
		}
		return false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::LeDadoVerifSe(void)
{
	// variáveis locais
	AnsiString qryTxt;
	TADOQuery *tdb = NULL;
	sdi_VerifSe sdi;
	sdi_Fld_VerifSe sdi_fld;
	VTLog *log = (VTLog*)apl->GetObject(__classid(VTLog));

	try
	{ // monta sql
		qryTxt.sprintf("SELECT * FROM VerifSe;");
		// executa query
		tdb = ExecutaQuery(qryTxt);
		if (tdb == NULL)
			return (false);
		// Não destroi lista default antes de saber se existem elementos na base
		if (tdb->RecordCount == 0)
			return (true);

		// limpa lista
		LimpaTList(paramConf->LisVerifSe);
		// atribui referências p/ os campos a serem pesquisados
		sdi_fld.codSe = tdb->FieldByName("codSe");
		sdi_fld.decVerif = tdb->FieldByName("decVerif");
		sdi_fld.fecVerif = tdb->FieldByName("fecVerif");
		// percorre registros da tabela
		for (tdb->First(); !tdb->Eof; tdb->Next())
		{ // preenche dados
			sdi.codSe = sdi_fld.codSe->AsString;
			sdi.decVerif = sdi_fld.decVerif->AsFloat;
			sdi.fecVerif = sdi_fld.fecVerif->AsFloat;
			// cria objeto VTVerifSe
			if ((CriaDadoVerifSe(sdi, paramConf->LisVerifSe)) == NULL)
			{
				EncerraQuery(tdb);
				return (false);
			}
		}
		EncerraQuery(tdb);
		return (true);
	}
	catch (Exception &e)
	{
		if (log)
		{
			log->Add(__PRETTY_FUNCTION__);
			log->Add("Erro detectado: " + e.Message);
		}
		return false;
	}
}

// ---------------------------------------------------------------------------
VTParamConf* __fastcall TBDadoParam::ParamConf(void)
{
	return (paramConf);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDadoParam::ValidaDirBase(void)
{
	return (FileExists(dirBase));
}

// ---------------------------------------------------------------------------
// eof
