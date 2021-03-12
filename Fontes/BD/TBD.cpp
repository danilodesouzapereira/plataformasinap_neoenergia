// ---------------------------------------------------------------------------
#include <vcl.h>
#include <Oracle.hpp>
// #include <OracleCI.hpp>
#pragma hdrstop
#include "TBD.h"
#include "VTSNPDBConn.h"
#include "Figura\VTBDfigura.h"
#include "Rede\VTBDeqpto.h"
#include "Rede\BD_inc.h"
#include "Rede\TBDfunc.h"
#include "Rede\VTBDgeral.h"
#include "Hist\VTBDhist.h"
#include "..\Apl\VTApl.h"
#include "..\Cronometro\VTCronometro.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Importa\Monta\VTMonta.h"
#include "..\Log\VTLog.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\Produto\VTProduto.h"
#include "..\Registro\VTRegistro.h"
#include "..\Progresso\VTProgresso.h"
#include "..\..\DLL_Inc\Cronometro.h"
#include "..\..\DLL_Inc\Importa.h"
#include "..\..\DLL_Inc\Log.h"
#include "..\..\DLL_Inc\MontaZ.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Registro.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"

// ---------------------------------------------------------------------------
VTBD* NewObjBD(VTApl *apl_owner)
{
	return (new TBD(apl_owner));
}

// ---------------------------------------------------------------------------
__fastcall TBD::TBD(VTApl *apl_owner)
{
	AnsiString straux;
	VTLog* plog;

	// cria objetos
	apl = NewObjApl(NULL, apl_owner);
	apl->Add(plog = DLL_NewObjLog(apl));
	apl->Add(DLL_NewObjMonta(apl));
	apl->Add(DLL_NewObjMontaZ(apl));
	apl->Add(new TBDfunc(apl));
	// apl->Add(DLL_NewObjProgresso(NULL, NULL));
	// apl->Add(DLL_NewObjCronometro(apl));

	// inicia dados
	revisao = "Rev3.0";
	cronometro_enabled = false;
	progresso_enabled = false;
	base_remota = false;
	bd_geral = NULL;
	bd_eqpto = NULL;
	bd_hist = NULL;
	oracleSessionHist = NULL;
	oracleSessionRede = NULL;

	// configura Log
	plog->ExibirDataHora = true;
	if ((path = (VTPath*)apl->GetObject(__classid(VTPath))) != NULL)
	{
		straux = path->DirTmp() + "\\";
		straux += SNPLOGFILENAME;
		plog->Open(straux, true /* Append */);
	}
}

// ---------------------------------------------------------------------------
__fastcall TBD::~TBD(void)
{
	VTLog* plog = (VTLog*)apl->GetObject(__classid(VTLog));

	// cancela eventual conexão ativa
	Desconecta();
	// fecha o arquivo de log
	if (plog)
	{
		plog->Close();
	}
	// Destroi objetos locais
	if (oracleSessionHist)
	{
		delete oracleSessionHist;
		oracleSessionHist = NULL;
	}
	if (oracleSessionRede)
	{
		delete oracleSessionRede;
		oracleSessionRede = NULL;
	}
	// if (accessConnGeral)   { delete accessConnGeral;   accessConnGeral   = NULL; }
	if (accessConnRede)
	{
		delete accessConnRede;
		accessConnRede = NULL;
	}
	if (accessConnHist)
	{
		delete accessConnHist;
		accessConnHist = NULL;
	}
	// destrói Apl
	if (apl)
	{
		delete apl;
		apl = NULL;
	}

}

// ---------------------------------------------------------------------------
bool __fastcall TBD::CommitHist(void)
{
	if (oracleSessionHist)
	{
		oracleSessionHist->Commit();
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::CommitRede(void)
{
	if (oracleSessionRede)
	{
		oracleSessionRede->Commit();
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::Conecta(int intTipoConexao)
{
	return (ConectaBaseRede(intTipoConexao));
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::ConectaBaseHist(int intTipoConexao)
{
	// variáveis locais
	bool bolret;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// verifica se foi definida a base de dados
	if ((intTipoConexao != SNPDBTYPE_ORACLE) && (intTipoConexao != SNPDBTYPE_ACCESS))
	{ // identifica base ativa
		intTipoConexao = geral->BaseDado;
	}
	switch (intTipoConexao)
	{
	case SNPDBTYPE_ORACLE:
		bolret = ConectaServidorHistRemoto(intTipoConexao);
		break;
	case SNPDBTYPE_SQLSERVER:
		bolret = ConectaServidorHistRemoto(intTipoConexao);
		break;
	case SNPDBTYPE_ACCESS:
		bolret = ConectaServidorHistLocal(intTipoConexao);
		break;
	default:
		bolret = false;
		break;
	}

	return (bolret);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::ConectaBaseRede(int intTipoConexao)
{
	// variáveis locais
	bool bolret;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// verifica se foi definida a base de dados
	if ((intTipoConexao != SNPDBTYPE_ORACLE) && (intTipoConexao != SNPDBTYPE_ACCESS))
	{ // identifica base ativa
		intTipoConexao = geral->BaseDado;
	}
	switch (intTipoConexao)
	{
	case SNPDBTYPE_ORACLE:
		bolret = ConectaServidorRedeRemoto(intTipoConexao);
		break;
	case SNPDBTYPE_SQLSERVER:
		bolret = ConectaServidorRedeRemoto(intTipoConexao);
		break;
	case SNPDBTYPE_ACCESS:
		bolret = ConectaServidorRedeLocal(intTipoConexao);
		break;
	default:
		bolret = false;
		break;
	}
	return (bolret);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::ConectaBaseHistAccess(AnsiString arq_hist)
{
	try
	{ // cancela eventual Conexao pre-existente
		if (!DesconectaHist())
			return (false);
		// verifica se existem os arquivos indicado
		if (!FileExists(arq_hist))
			return (false);
		// destrói eventual conexão pré-existente
		if (accessConnHist)
			delete accessConnHist;
		// cria Conexao c/ base Access
		if ((accessConnHist = IniciaConexaoAccess(arq_hist)) == NULL)
			return (false);
		// inicia Conexao c/ bases Access
		// if (! accessConnGeral->Init()) return(false);
		// if (! accessConnRede->Init())  return(false);
		// cria demais objetos de acesso à base de dados
		bd_hist = NewObjBDhistAccess(apl, accessConnHist);
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::ConectaBaseHistOracle(AnsiString TNS, AnsiString user_name,
	AnsiString password)
{
	try
	{ // cancela eventual Conexao pre-existente
		if (!DesconectaHist())
			return (false);
		// destrói eventual conexão pré-existente
		if (oracleSessionHist)
			delete oracleSessionHist;
		// cria Conexao c/ bases Oracle
		if ((oracleSessionHist = IniciaConexaoOracle(TNS, user_name, password)) == NULL)
			return (false);
		// cria demais objetos de acesso à base de dados
		bd_hist = NewObjBDhistOracle(apl, oracleSessionHist);
		apl->Add(bd_hist);
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::ConectaBaseHistSqlServer(AnsiString TNS, AnsiString user_name,
	AnsiString password)
{
	return (false);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16526463
//https://sinapsisenergia.teamwork.com/#tasks/14422298
//https://sinapsisenergia.teamwork.com/#tasks/14422299
//https://sinapsisenergia.teamwork.com/#tasks/14422301
bool __fastcall TBD::ConectaBaseRedeAccess(AnsiString arq_rede)
{
	try
	{ // cancela eventual Conexao pre-existente
		if (!DesconectaRede())
			return (false);
		// verifica se existem o arquivo indicado
		if (!FileExists(arq_rede))
			return (false);
		// destrói eventual conexão pré-existente
		if (accessConnRede)
			delete accessConnRede;
		// cria Conexao c/ bases Access
		if ((accessConnRede = IniciaConexaoAccess(arq_rede)) == NULL)
			return (false);
		// inicia Conexao c/ bases Access
		// cria demais objetos de acesso à base de dados
		bd_eqpto = NewObjBDeqptoAccess425(apl, accessConnRede);
		bd_geral = NewObjBDgeralAccess(apl, accessConnRede);
		//bd_figura = NewObjBDfigura(apl, accessConnRede);
		// Adicionado em 20120717
		apl->Add(bd_geral);
		apl->Add(bd_eqpto);
		// verifica versão da base para criar o bd_eqpto correto
		AnsiString versao = bd_eqpto->VersaoBase();
		//cria o BDfigura de acordo
		bd_figura = CriaBDFigura(versao, apl, accessConnRede);
		apl->Add(bd_figura);
		VTBDeqpto *bd_eqpto_old;
		if (versao.AnsiCompareIC(BASE_REV_ATUAL) != 0)
		{
			if (versao.AnsiCompareIC(BASE_REV_424) == 0)
			{
				bd_eqpto_old = NewObjBDeqptoAccess424(apl, accessConnRede);
			}
			else if (versao.AnsiCompareIC(BASE_REV_423) == 0)
			{
				bd_eqpto_old = NewObjBDeqptoAccess423(apl, accessConnRede);
			}
			else if (versao.AnsiCompareIC(BASE_REV_422) == 0)
			{
				bd_eqpto_old = NewObjBDeqptoAccess422(apl, accessConnRede);
			}
			else if (versao.AnsiCompareIC(BASE_REV_421) == 0)
			{
				bd_eqpto_old = NewObjBDeqptoAccess421(apl, accessConnRede);
			}
			else if (versao.AnsiCompareIC(BASE_REV_42x) == 0)
			{
				bd_eqpto_old = NewObjBDeqptoAccess420(apl, accessConnRede);
			}
			else if (versao.AnsiCompareIC(BASE_REV_41x) == 0)
			{
				bd_eqpto_old = NewObjBDeqptoAccess410(apl, accessConnRede);
			}
			else if (versao.AnsiCompareIC(BASE_REV_4x) == 0)
			{
				bd_eqpto_old = NewObjBDeqptoAccess40(apl, accessConnRede);
			}
			else if (versao.AnsiCompareIC(BASE_REV310) == 0)
			{
				bd_eqpto_old = NewObjBDeqptoAccess310(apl, accessConnRede);
			}
			else
				return (false);
			apl->Remove(bd_eqpto);
			delete bd_eqpto;
			apl->Add(bd_eqpto_old);
			bd_eqpto = bd_eqpto_old;
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16526463
bool __fastcall TBD::ConectaBaseRedeOracle(AnsiString TNS, AnsiString user_name,
	AnsiString password)
{
	try
	{ // cancela eventual Conexao pre-existente
		if (!DesconectaRede())
			return (false);
		// destrói eventual conexão pré-existente
		if (oracleSessionRede)
			delete oracleSessionRede;
		// cria Conexao c/ bases Oracle
		if ((oracleSessionRede = IniciaConexaoOracle(TNS, user_name, password)) == NULL)
			return (false);
		// cria demais objetos de acesso à base de dados
		bd_eqpto = NewObjBDeqptoOracle(apl, oracleSessionRede);
		bd_geral = NewObjBDgeralOracle(apl, oracleSessionRede);
		bd_figura = NewObjBDfiguraDummy(apl, NULL);
		apl->Add(bd_geral);
		apl->Add(bd_eqpto);
        apl->Add(bd_figura);
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::ConectaBaseRedeSqlServer(AnsiString TNS, AnsiString user_name,
	AnsiString password)
{
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::ConectaServidorHistLocal(int intTipoConexao)
{
	bool sucesso;

	// Indica o tipo de conexão
	base_remota = false;

	if (intTipoConexao == SNPDBTYPE_ACCESS)
	{
		// variáveis locais
		VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

		// MM: A idéia é fazer as duas conexões apontarem p/ a base de rede. Isso deve ser suficiente p/ continuar com os testes do projeto BDihm
		// sucesso = ConectaBaseHistAccess(path->ArqPerdaTecnica());
		sucesso = ConectaBaseHistAccess(path->ArqHistorico);
	}
	else
	{
		sucesso = false;
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::ConectaServidorHistRemoto(int intTipoConexao)
{
	// variáveis locais
	struct
	{
		AnsiString tipo, TNS, user_name, password;
	} servidor;
	bool sucesso;
	VTRegistro *registro;
	VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

	// Indica o tipo de conexão
	base_remota = true;

	// Verifica se o tipo passado não é absurdo
	// Note que o tipo indefinido foi permitido neste ponto
	if (intTipoConexao == SNPDBTYPE_ACCESS)
		return (false);

	try
	{ // cria objeto VTRegistro p/ ler registro do Windows
		if ((registro = DLL_NewObjRegistro(produto->Codigo)) == NULL)
			return (false);
		// verifica se o servidor está configurado
		if (registro->ExisteInfo("servidor\\Hist", "tipo", servidor.tipo) && registro->ExisteInfo
			("servidor\\Hist", "tns", servidor.TNS) && registro->ExisteInfo("servidor\\Hist",
			"user", servidor.user_name) && registro->ExisteInfo("servidor\\Hist", "pswd",
			servidor.password))
		{ // seleciona tipo de servidor
			if (servidor.tipo.AnsiCompareIC("oracle") == 0)
			{
				sucesso = ConectaBaseHistOracle(servidor.TNS, servidor.user_name,
				servidor.password);
			}
			else if (servidor.tipo.AnsiCompareIC("SQLserver") == 0)
			{
				sucesso = ConectaBaseHistSqlServer(servidor.TNS, servidor.user_name,
					servidor.password);
			}
		}
		else
		{
			sucesso = false;
		}
		// destrói objeto Registro
		delete registro;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::ConectaServidorRedeLocal(int intTipoConexao)
{
	bool sucesso;
	VTPath* path;

	// Indica o tipo de conexão
	base_remota = false;
	if ((intTipoConexao == SNPDBTYPE_ACCESS) || (intTipoConexao == SNPDBTYPE_INDEFINIDO))
	{
		// Descobre  o path do arquivo
		if ((path = (VTPath*)apl->GetObject(__classid(VTPath))) == NULL)
			return (false);
		// Conecta-se ao arquivo
		sucesso = ConectaBaseRedeAccess(path->ArqRede);
		// OOPS_Perda
		if (sucesso)
		{
			// OOPS. Por enquanto não está se conectando
			// sucesso = ConectaServidorHistLocal(intTipoConexao);
		}
	}
	else
	{
		sucesso = false;
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::ConectaServidorRedeRemoto(int intTipoConexao)
{
	// variáveis locais
	struct
	{
		AnsiString tipo, tnsname, username, password;
	} servidor;
	bool sucesso;
	AnsiString secao;
	VTRegistro *registro;
	VTProduto *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

	// Testa se o produto existe
	if (!produto)
		return (false);

	// Indica o tipo de conexão
	base_remota = true;

	// Verifica se o tipo passado não é absurdo
	// Note que o tipo indefinido foi permitido neste ponto
	if (intTipoConexao == SNPDBTYPE_ACCESS)
		return (false);

	try
	{ // cria objeto VTRegistro p/ ler registro do Windows
		if ((registro = DLL_NewObjRegistro(produto->Sinapsis)) == NULL)
			return (false);
		// verifica se o servidor está configurado
		secao = produto->WinRegister + "\\servidor\\rede";
		if (registro->ExisteInfo(secao, "tipo", servidor.tipo) && registro->ExisteInfo(secao,
			"tnsname", servidor.tnsname) && registro->ExisteInfo(secao, "username",
			servidor.username) && registro->ExisteInfo(secao, "password", servidor.password))
		{ // seleciona tipo de servidor
			if (servidor.tipo.AnsiCompareIC("oracle") == 0)
			{
				sucesso = ConectaBaseRedeOracle(servidor.tnsname, servidor.username,
					servidor.password);
			}
			else if (servidor.tipo.AnsiCompareIC("SQLserver") == 0)
			{
				sucesso = ConectaBaseRedeSqlServer(servidor.tnsname, servidor.username,
					servidor.password);
			}
		}
		else
		{
			sucesso = false;
		}
		// destrói objeto Registro
		delete registro;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}

	return (sucesso);
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16526463
VTBDfigura* __fastcall TBD::CriaBDFigura(AnsiString versaoBase,VTApl *apl_owner, VTSNPDBConn *bd_conn)
{
	VTBDfigura *bdFigura = NULL;

	// FKM 2018.04.05 gambiarra para eliminar o problema do X
	versaoBase = UpperCase(versaoBase);
	versaoBase = StringReplace(versaoBase, "X", "0", TReplaceFlags() << rfReplaceAll);
	if(versaoBase.AnsiCompareIC(BASE_REV_424) >= 0)
	{
        bdFigura = NewObjBDfigura(apl_owner,bd_conn);
	}
	else
	{
		bdFigura = NewObjBDfiguraDummy(apl_owner,bd_conn);
	}

	return bdFigura;
}
// ---------------------------------------------------------------------------
bool __fastcall TBD::Desconecta(void)
{
	// Não considera resultados intermediários.
	// Desconecta o que estiver conectado, simplesmente.
	DesconectaRede();
	DesconectaHist();
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::DesconectaHist(void)
{
	try
	{ // destrói objetos de acesso às bases de dados
		if (bd_hist)
		{
			apl->Remove(bd_hist);
			delete bd_hist;
			bd_hist = NULL;
		}
	}
	catch (Exception &e)
	{
		return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::DesconectaRede(void)
{
	try
	{ // destrói objetos de acesso às bases de dados
		if (bd_geral)
		{
			apl->Remove(bd_geral);
			delete bd_geral;
			bd_geral = NULL;
		}
		if (bd_eqpto)
		{
			apl->Remove(bd_eqpto);
			delete bd_eqpto;
			bd_eqpto = NULL;
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
VTSNPDBConn* __fastcall TBD::IniciaConexaoAccess(AnsiString arq_mdb)
{
	// variáveis locais
	AnsiString strLoginName = "";
	// AnsiString strLoginPwd     = "ultraplan2006";
	AnsiString strLoginPwd = "sinap2006_2013";
	int intDBType = SNPDBTYPE_ACCESS;
	bool bolMultiDataSet = true;

	return (NewObjSNPDBConn(apl, arq_mdb, strLoginName, strLoginPwd, intDBType, bolMultiDataSet));
}

// ---------------------------------------------------------------------------
TOracleSession* __fastcall TBD::IniciaConexaoOracle(AnsiString strTNS, AnsiString strLoginName,
	AnsiString strLoginPwd)
{
	// variáveis locais
	TOracleSession* ptrOracleSession;

	try
	{
		// If you want to retrieve UTF8 data as WE8MSWIN1252 you can set the
		// global NoUnicodeSupport Boolean variable in the Oracle unit to True.
		// !OOPS! Isso aqui deveria se tornar um parâmetro do Sinap para futuros problemas
		// Precisa ver se funciona corretamente na Sinapsis
		NoUnicodeSupport = true;

		ptrOracleSession = new TOracleSession(NULL);
		ptrOracleSession->LogonUsername = strLoginName; // "SINAPADM";
		ptrOracleSession->LogonPassword = strLoginPwd; // "sinapadmpwd";
		ptrOracleSession->LogonDatabase = strTNS; // Sinapsis: SNPORA_SINAPSIS, Local: SNPORA
		ptrOracleSession->LogOn();

		// !OOPS! retirar isso aqui
		// ptrOracleSession->AutoCommit = true;
	}
	catch (EOracleError &e)
	{
		if (e.ErrorCode == 0)
		{
			ShowMessage(
				"Erro na conexão com o Oracle. Verifique a instalação do driver e sua configuração.");
		}
		if (ptrOracleSession)
		{
			delete ptrOracleSession;
			ptrOracleSession = NULL;
		}
	}
	return (ptrOracleSession);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::PM_GetConectado(void)
{
	// verifica se está conectado com Oracle
	if (oracleSessionRede)
	{
		return (oracleSessionRede->Connected);
	}
	if (oracleSessionHist)
	{
		return (oracleSessionHist->Connected);
	}
	if (accessConnRede)
	{
		return (accessConnRede->getConnectedOK(false));
	}

	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::PM_GetBaseRemota(void)
{
	return (base_remota);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::PM_SetBaseRemota(bool bolFlagRemoto)
{
	// OOPS. Fiz com property para poder permitir a chamada de funções de conexão, se necessário
	base_remota = bolFlagRemoto;

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::RollbackHist(void)
{
	if (oracleSessionHist)
	{
		oracleSessionHist->Rollback();
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBD::RollbackRede(void)
{
	if (oracleSessionRede)
	{
		oracleSessionRede->Rollback();
	}

	return (true);
}

// ---------------------------------------------------------------------------
// eof
