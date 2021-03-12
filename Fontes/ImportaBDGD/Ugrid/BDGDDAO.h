// --------------------------------------------------------------------------------------------- bof
#ifndef BDGDDAOH
#define BDGDDAOH

// ---------------------------------------------------------------------------------------- includes
#include <list>
#include <Classes.hpp>

// Project include
#include "VTBDGDDAO.h"
#include <Fontes\LocalDB\SQLite\VTSQLite.h>
class EImportaUgridDAO;


using namespace std;

// ------------------------------------------------------------------------------------------- class
class BDGDDAO : public VTBDGDDAO
{

private:
	VTSQLiteDB *sqLite;

	VTSQLiteTable* __fastcall ExecuteSql(UnicodeString sql) throw (EImportaUgridDAO);
	int 		__fastcall ExecuteDML(UnicodeString sql) throw (EImportaUgridDAO);
public:
	__fastcall BDGDDAO(UnicodeString filePathSED,UnicodeString filePathCOMMONS);
	__fastcall BDGDDAO(UnicodeString filePath);
	__fastcall ~BDGDDAO(void);
	void __fastcall Close(void);
	// COMMONS: Dominio
	VTSQLiteTable* __fastcall ExecuteSqlAreaLoc(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlBitolaCabo(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlCapElo(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlClasseSubclasse(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlClasseTensao(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlConfCircuito(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlCorrenteNom(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlFasesConexao(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlFormacaoCabo(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlGeometriaCabo(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlGrupoTarifario(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlIsolacaoCabo(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlLigacaoTrafo(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlMaterialCabo(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlPosse(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlTensao(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlTipoPotAparente(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlTipoPotReativa(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlTipoRegulador(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlTipoTrafo(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlTipoUnidade(void) throw (EImportaUgridDAO);
	// COMMONS: Entidades
	VTSQLiteTable* __fastcall ExecuteSqlArranjo(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlBarra(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlBarramento(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlRedeMT(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlSubestacao(void) throw (EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlTrafoSub(void) throw (EImportaUgridDAO);
	// PRIMÁRIO
	VTSQLiteTable* __fastcall ExecuteSqlPriCapacitor(int idAlimentador);
	VTSQLiteTable* __fastcall ExecuteSqlPriChave(int idAlimentador);
	VTSQLiteTable* __fastcall ExecuteSqlPriCargaMT(int idAlimentador);
	VTSQLiteTable* __fastcall ExecuteSqlPriGerador(int idAlimentador);
	VTSQLiteTable* __fastcall ExecuteSqlPriRegulador(int idAlimentador);
	VTSQLiteTable* __fastcall ExecuteSqlPriTrecho(int idAlimentador);
//    // SECUNDÁRIO
//	VTSQLiteTable* __fastcall ExecuteSqlSecBarra(int idSec) throw(EImportaUgridDAO);
	VTSQLiteTable* __fastcall ExecuteSqlSecCapacitor(int idSec);
	VTSQLiteTable* __fastcall ExecuteSqlSecChave(int idSec);
	VTSQLiteTable* __fastcall ExecuteSqlSecConsBT(int idSec);
	VTSQLiteTable* __fastcall ExecuteSqlSecConsIP(int idSec);
	VTSQLiteTable* __fastcall ExecuteSqlSecGeradorBT(int idSec);
	VTSQLiteTable* __fastcall ExecuteSqlSecRamal(int idSec);
	VTSQLiteTable* __fastcall ExecuteSqlSecRede(int idAlimentador);
	VTSQLiteTable* __fastcall ExecuteSqlSecTrafo(int idAlimentador);
	VTSQLiteTable* __fastcall ExecuteSqlSecTrecho(int idSec);

};

#endif

// --------------------------------------------------------------------------------------------- eof
