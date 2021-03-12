// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <list>

// Project include
#include <DLL_Inc\LocalDB.h>
#include <Fontes\LocalDB\SQLite\VTSQLite.h>
#include <Fontes\LocalDB\SQLite\ESQLite.h>
#include <..\Exceptions\EImportaUgrid.h>
#include <BDGDDAO.h>

// --------------------------------------------------------------------------------------- namespace
using namespace std;
//using namespace sinapsis::sinap::BDGD;


// -------------------------------------------------------------------------------------------
VTBDGDDAO* NewObjBDGDDAO(UnicodeString filePathCOMMONS, UnicodeString filePathSED)
{
	VTBDGDDAO *dao = NULL;

	if (filePathSED.IsEmpty())
	{
		dao = new BDGDDAO(filePathCOMMONS);
	}
	else
	{
		dao = new BDGDDAO(filePathSED, filePathCOMMONS);
	}

	return (dao);
}
// ------------------------------------------------------------------------------------------- class
__fastcall BDGDDAO::BDGDDAO(UnicodeString filePathSED, UnicodeString filePathCOMMONS)
{
	AnsiString sql;

	sqLite = DLL_NewObjSQLiteDB();
	sqLite->LoadAttach(AnsiString(filePathSED), AnsiString(filePathCOMMONS), "COMMONS");

}

// -------------------------------------------------------------------------------------------
__fastcall BDGDDAO::BDGDDAO(UnicodeString filePath)
{
	AnsiString sql;

	sqLite = DLL_NewObjSQLiteDB();
	sqLite->Load(AnsiString(filePath));
}

// -------------------------------------------------------------------------------------------------
__fastcall BDGDDAO::~BDGDDAO(void)
{
	if (sqLite != NULL)
	{
		delete sqLite;
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall BDGDDAO::Close(void)
{
	sqLite->Close();                                                                                                    }

// -------------------------------------------------------------------------------------------------
int __fastcall BDGDDAO::ExecuteDML(UnicodeString sql) throw(EImportaUgridDAO)
{
	// return(sqLite->execDML(AnsiString(sql).c_str()));
	try
	{
		return (sqLite->ExecDML(AnsiString(sql).c_str()));
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na ExecuteDML.");
	}

}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSql(UnicodeString sql) throw(EImportaUgridDAO)
{
	// return(sqLite->getTable(AnsiString(sql).c_str()));
	try
	{
		return (sqLite->GetTable(AnsiString(sql).c_str()));
	}
	catch (ESQLite &e)
	{
		throw;
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlAreaLoc() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_AREA_LOCALIZACAO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query AREA_LOCALIZACAO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlArranjo() throw(EImportaUgridDAO)
{
	// Commons: Entidade
	try
	{
		return ExecuteSql("SELECT * FROM TB_ARRANJO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query ARRANJO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlBarra() throw(EImportaUgridDAO)
{
	// Commons: Entidade
	try
	{
		return ExecuteSql("SELECT * FROM TB_BARRA;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query BARRA.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlBarramento() throw(EImportaUgridDAO)
{
	// Commons: Entidade
	try
	{
		return ExecuteSql("SELECT * FROM TB_BARRAMENTO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query BARRAMENTO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlBitolaCabo() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_BITOLA_CABO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query BITOLA_CABO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlCapElo() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_CAPACIDADE_ELO_FUSIVEL;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query CAPACIDADE_ELO_FUSIVEL.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlClasseSubclasse() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_CLASSE_SUBCLASSE;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query CLASSE_SUBCLASSE.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlClasseTensao() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_CLASSE_TENSAO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query CLASSE_TENSAO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlConfCircuito() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_CONFIGURACAO_CIRCUITO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query CONFIGURACAO_CIRCUITO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlCorrenteNom() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_CORRENTE_NOMINAL;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query CORRENTE_NOMINAL.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlFasesConexao() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_FASES_CONEXAO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query FASES_CONEXAO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlFormacaoCabo() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_FORMACAO_CABO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query FORMACAO_CABO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlGeometriaCabo() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_GEOMETRIA_CABO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query GEOMETRIA_CABO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlGrupoTarifario() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_GRUPO_TARIFARIO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query GRUPO_TARIFARIO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlIsolacaoCabo() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_ISOLACAO_CABO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query ISOLACAO_CABO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlLigacaoTrafo() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_LIGACAO_TRAFO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query LIGACAO_TRAFO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlMaterialCabo() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_MATERIAL_CABO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query MATERIAL_CABO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlPosse() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_POSSE;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query POSSE.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlRedeMT() throw(EImportaUgridDAO)
{
	// Commons: Entidade
	try
	{
		return ExecuteSql("SELECT TB_REDE_MT.*, TB_BARRAMENTO.ID_BARRA AS "
						  "ID_BARRA_TRAFO from TB_REDE_MT left join TB_BARRAMENTO "
						  "ON TB_REDE_MT.ID_BARRAMENTO = TB_BARRAMENTO.ID_BARRAMENTO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query REDE_MT.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlSubestacao() throw(EImportaUgridDAO)
{
	// Commons: Entidade
	try
	{
		return ExecuteSql("SELECT * FROM TB_SUBESTACAO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query SUBESTACAO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlTensao() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_TENSAO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query TENSAO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlTipoPotAparente() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_TIPO_POTENCIA_APARENTE;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query TIPO_POTENCIA_APARENTE.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlTipoPotReativa() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_TIPO_POTENCIA_REATIVA;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query TIPO_POTENCIA_REATIVA.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlTipoRegulador() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_TIPO_REGULADOR;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query TIPO_REGULADOR.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlTipoTrafo() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_TIPO_TRAFO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query TIPO_TRAFO.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlTipoUnidade() throw(EImportaUgridDAO)
{
	// Commons: TABELA DE DOMÍNIO
	try
	{
		return ExecuteSql("SELECT * FROM TB_TIPO_UNIDADE;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query TIPO_UNIDADE.");
	}
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlTrafoSub() throw(EImportaUgridDAO)
{
	// Commons: Entidade
	try
	{
		return ExecuteSql("SELECT * FROM TB_TRAFO_SUBESTACAO;");
	}
	catch (ESQLite &e)
	{
		throw EImportaUgridDAO("Erro na query TRAFO_SUBESTACAO.");
	}
}

//// -------------------------------------------------------------------------------------------------
//VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlSub() throw(EImportaUgridDAO)
//{
//	try
//	{
//		return ExecuteSql
//			("select * from TB_SED inner join TB_REDE on TB_REDE.ID_REDE = "
//			" TB_SED.ID_SED ORDER BY NOME");
//	}
//	// catch(...)
//	catch (ESQLite &e)
//	{
//		throw EImportaUgridDAO("Erro na query de subestações. ");
//	}
//}
//
//// -------------------------------------------------------------------------------------------------
//VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlSub(int idSubestacao) throw(EImportaUgridDAO)
//{
//	try
//	{
//		return ExecuteSql
//		("select SED.*, REDE.*, AUX.UTMX_M AS UTMX_M, AUX.UTMY_M AS UTMY_M from commons.TB_SED SED "
//		 "inner join commons.TB_REDE REDE on REDE.ID_REDE = SED.ID_SED "
//		 "inner join "
//		 "("
//		 "select distinct commons.TB_TRAFO_SED.ID_SED, TB_BARRAMENTO.UTMX_M, TB_BARRAMENTO.UTMY_M from TB_BARRAMENTO "
//		 "inner join commons.TB_TRAFO_SED on commons.TB_TRAFO_SED.ID_INSTALACAO = TB_BARRAMENTO.ID_INSTALACAO "
//		 ") AUX "
//		 "ON aux.ID_SED = SED.ID_SED "
//		 "WHERE SED.ID_SED=" + IntToStr(idSubestacao));
//	}
//	// catch(...)
//	catch (ESQLite &e)
//	{
//		throw EImportaUgridDAO("Erro na query de subestações com where=" + IntToStr(idSubestacao)
//			+ ". ");
//	}
//}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlPriCapacitor(int idAlimentador)
{

	return ExecuteSql("select * from TB_CAPACITOR_MT where ID_REDE_MT = " + IntToStr(idAlimentador));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlPriCargaMT(int idAlimentador)
{
	return ExecuteSql("select * FROM TB_CARGA_MT LEFT JOIN "
					  "TB_DEMANDA_ATIVA_MAX_MT DEM ON TB_CARGA_MT.ID_CARGA_MT = DEM.ID_CARGA_MT "
					  "LEFT JOIN TB_ENERGIA_ATIVA_MT ENE ON TB_CARGA_MT.ID_CARGA_MT = ENE.ID_CARGA_MT "
					  "WHERE TB_CARGA_MT.ID_REDE_MT = " + IntToStr(idAlimentador));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlPriChave(int idAlimentador)
{
	return ExecuteSql("select * from TB_CHAVE_MT where ID_REDE_MT = " + IntToStr(idAlimentador));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlPriGerador(int idAlimentador)
{
	// está sendo lida
	return ExecuteSql("select * from TB_GERADOR_MT where ID_REDE_MT = " + IntToStr(idAlimentador));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlPriRegulador(int idAlimentador)
{
	return ExecuteSql("select * from TB_REGULADOR_TENSAO_MT where ID_REDE_MT = " + IntToStr(idAlimentador));
}

//// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlPriTrecho(int idAlimentador)
{
	return ExecuteSql("select * from TB_TRECHO_MT WHERE ID_REDE_MT = " + IntToStr(idAlimentador));
}
//
//// REDE SECUNDÁRIA
//// -------------------------------------------------------------------------------------------------
//VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlSecBarra(int idSecundario) throw(EImportaUgridDAO)
//{
//	try
//	{
//	return ExecuteSql("select distinct * from ( "
//			"select distinct tb_barra.*, tb_trecho.ID_REDE as ID_REDE_BT from tb_barra "
//			"inner join tb_trecho on "
//			"((tb_trecho.id_barra1 = tb_barra.ID_BARRA) or "
//			"(tb_trecho.id_barra2 = tb_barra.ID_BARRA)) "
//			"where tb_trecho.ID_REDE = " + IntToStr(idSecundario) +
//			" union "
//			"select distinct tb_barra.*, tb_link.ID_REDE as ID_REDE_BT from tb_barra "
//			"inner join tb_link on "
//			"((tb_link.id_barra1 = tb_barra.ID_BARRA) or "
//			"(tb_link.id_barra2 = tb_barra.ID_BARRA))"
//			"where tb_link.ID_REDE = " + IntToStr(idSecundario) +
//			" union "
//			"select distinct tb_barra.*, tb_trafo_mtbt.ID_REDE_BT as ID_REDE_BT from tb_barra "
//			"inner join tb_trafo_mtbt on "
//			"(tb_trafo_mtbt.id_barra_bt = tb_barra.ID_BARRA) "
//			"where tb_trafo_mtbt.ID_REDE_BT = " + IntToStr(idSecundario) +
//			" union "
////			"select distinct tb_barra.*, tb_capacitor.ID_REDE as ID_REDE_BT from tb_barra "
////			"inner join tb_capacitor on "
////			"(tb_capacitor.id_barra = tb_barra.ID_BARRA) "
////			"where tb_capacitor.ID_REDE = " + IntToStr(idSecundario) +
////			" union "
////			"select distinct tb_barra.*, tb_gerador.ID_REDE as ID_REDE_BT from tb_barra "
////			"inner join tb_gerador on "
////			"((tb_gerador.id_barra1 = tb_barra.ID_BARRA) or "
////			"(tb_gerador.id_barra2 = tb_barra.ID_BARRA)) "
////			"where tb_gerador.ID_REDE = " + IntToStr(idSecundario) +
////			" union "
////			"select distinct tb_barra.*, tb_carga_ip.ID_REDE_BT as ID_REDE_BT from tb_barra "
////			"inner join tb_carga_ip on "
////			"(tb_carga_ip.id_barra = tb_barra.ID_BARRA) "
////			"where tb_carga_ip.ID_REDE_BT = " + IntToStr(idSecundario) +
////			" union "
//			"select distinct tb_barra.*, tb_pcarga_bt.ID_REDE_BT as ID_REDE_BT from tb_barra "
//			"inner join tb_pcarga_bt on "
//			"(tb_pcarga_bt.id_barra = tb_barra.ID_BARRA) "
//			"where tb_pcarga_bt.ID_REDE_BT = " + IntToStr(idSecundario) +
//			")");
//	}
//// catch(...)
//	catch (ESQLite &e)
//	{
//		throw EImportaUgridDAO("Erro na query de barras do secundário");
//	}
//
//	}
//

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlSecCapacitor(int idSec)
{
	return ExecuteSql("select * from TB_CAPACITOR_BT where ID_REDE_BT = " + IntToStr(idSec));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlSecChave(int idSec)
{
	return ExecuteSql("select * from TB_CHAVE_BT where ID_REDE_BT = " + IntToStr(idSec));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlSecConsBT(int idSec)
{
	return ExecuteSql("select * FROM TB_CARGA_BT LEFT JOIN "
					  "TB_ENERGIA_ATIVA_MEDIDA_BT ENE ON TB_CARGA_BT.ID_CARGA_BT = ENE.ID_CARGA_BT "
					  "WHERE TB_CARGA_BT.ID_REDE_BT = " + IntToStr(idSec));

}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlSecConsIP(int idSec)
{
	return ExecuteSql("select * FROM TB_CARGA_IP "
		   "CARGA LEFT JOIN TB_ENERGIA_ATIVA_ESTIMADA_IP IP ON "
		   "CARGA.ID_CARGA_IP = IP.ID_CARGA_IP where CARGA.ID_REDE_BT = " + IntToStr(idSec));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlSecGeradorBT(int idSec)
{
	return ExecuteSql("select * from TB_GERADOR_BT WHERE ID_REDE_BT = " + IntToStr(idSec));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlSecRamal(int idSec)
{
	return ExecuteSql("select * from TB_RAMAL_LIGACAO WHERE ID_REDE_BT = " + IntToStr(idSec));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlSecRede(int idAlimentador)
{
	return ExecuteSql
	("select TB_REDE_BT.*, TB_TRAFO_DISTRIBUICAO.ID_TENSAO_SEC AS TEN_SEC_MTBT, "
	"TB_TRAFO_DISTRIBUICAO.ID_TENSAO_TER AS TEN_TER_MTBT "
	"from TB_REDE_BT inner join TB_TRAFO_DISTRIBUICAO on TB_REDE_BT.ID_REDE_BT = TB_TRAFO_DISTRIBUICAO.ID_REDE_BT "
	"where TB_REDE_BT.ID_REDE_MT = " + IntToStr(idAlimentador));
}

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlSecTrafo(int idAlimentador)
{
	return ExecuteSql
	("select * from TB_TRAFO_DISTRIBUICAO where ID_REDE_MT = " + IntToStr(idAlimentador));
}

//// -------------------------------------------------------------------------------------------------
//VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlSecUtcBT(int idSecundario)
//{
//	return ExecuteSql(
//	"select * from TB_TRAFO_MTBT "
//	"where ID_REDE_BT = " + IntToStr(idSecundario) + " ORDER BY SNOM_KVA DESC");
//}
//

// -------------------------------------------------------------------------------------------------
VTSQLiteTable* __fastcall BDGDDAO::ExecuteSqlSecTrecho(int idSecundario)
{
	return ExecuteSql("select * from TB_TRECHO_BT where ID_REDE_BT = " + IntToStr(idSecundario));
}



// --------------------------------------------------------------------------------------------- eof
