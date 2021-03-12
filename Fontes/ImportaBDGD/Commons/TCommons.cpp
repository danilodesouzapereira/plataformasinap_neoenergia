// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <algorithm>
#include <inifiles.hpp>
#include <map>

// DLL include
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Progresso.h>

// Plataforma include
#include <Fontes\Apl\VTApl.h>
#include <Fontes\BD\Rede\BD_Inc.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\ImportaRedeGis\VTImportaRedeGis.h>
#include <Fontes\LocalDB\SQLite\ESQLite.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Progresso\VTProgresso.h>
#include <BDGDDAO.h>
#include <EImportaUgrid.h>

// Project include - <Commons\Dominio>
#include <Dominio\TAreaLocalizacao.h>
#include <Dominio\TBitolaCabo.h>
#include <Dominio\TCapElo.h>
#include <Dominio\TClasseSubclasse.h>
#include <Dominio\TClasseTensao.h>
#include <Dominio\TConfCircuito.h>
#include <Dominio\TCorrenteNom.h>
#include <Dominio\TFasesConexao.h>
#include <Dominio\TFormacaoCabo.h>
#include <Dominio\TGeometriaCabo.h>
#include <Dominio\TGrupoTarifario.h>
#include <Dominio\TIsolacaoCabo.h>
#include <Dominio\TLigacaoTrafo.h>
#include <Dominio\TMaterialCabo.h>
#include <Dominio\TPosse.h>
#include <Dominio\TTensao.h>
#include <Dominio\TTipoPotAparente.h>
#include <Dominio\TTipoPotReativa.h>
#include <Dominio\TTipoRegulador.h>
#include <Dominio\TTipoTrafo.h>
#include <Dominio\TTipoUnidade.h>
// Project include - <Commons\Entidades>
#include <Entidades\TArranjoBDGD.h>
#include <Entidades\TBarraBDGD.h>
#include <Entidades\TBarramento.h>
#include <Entidades\TRedeMT.h>
#include <Entidades\TSubestacao.h>
#include <Entidades\TTrafoSubestacao.h>

#include <TCommons.h>

// -------------------------------------------------------------------------------------------------
VTCommons* NewObjCommons(VTApl *apl, VTBDGDDAO *dao)
{
	return (new TCommons(apl, (BDGDDAO*)dao));
}

// -------------------------------------------------------------------------------------------------
__fastcall TCommons::TCommons(VTApl *apl, BDGDDAO *dao)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	this->dao = dao;
	progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	// Cria listas
	lisDOMINIO = new TList();
	lisENTIDADES = new TList();
}

// -------------------------------------------------------------------------------------------------
__fastcall TCommons::~TCommons(void)
{
	// destrói lista e seus objetos
	if (lisDOMINIO)
	{
		LimpaTList(lisDOMINIO);
		delete lisDOMINIO;
		lisDOMINIO = NULL;
	}
	if (lisENTIDADES)
	{
		LimpaTList(lisENTIDADES);
		delete lisENTIDADES;
		lisENTIDADES = NULL;
	}
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::Clear(void)
{
	LimpaMapas();
	LimpaTList(lisENTIDADES);
	LimpaTList(lisDOMINIO);
	return (true);
}

// -----------------------------------------------------------------------------
TAreaLocalizacao* __fastcall TCommons::GetAreaLoc(int id_area)
{
	// Variaveis Locais
	TAreaLocalizacao *area_loc = NULL;
	//
	if (mapAreaLoc.find(id_area) != mapAreaLoc.end())
	{
		area_loc = mapAreaLoc.find(id_area)->second;
	}
	return (area_loc);
}

// -----------------------------------------------------------------------------
TArranjoBDGD* __fastcall TCommons::GetArranjo(int id_arranjo)
{
	// Variaveis Locais
	TArranjoBDGD *arranjo = NULL;
	//
	if (mapArranjo.find(id_arranjo) != mapArranjo.end())
	{
		arranjo = mapArranjo.find(id_arranjo)->second;
	}
	return (arranjo);
}

// -----------------------------------------------------------------------------
TBarraBDGD* __fastcall TCommons::GetBarra(int id_barra)
{
	// Variaveis Locais
	TBarraBDGD *barra = NULL;
	//
	if (mapBarra.find(id_barra) != mapBarra.end())
	{
		barra = mapBarra.find(id_barra)->second;
	}
	return (barra);
}

// -----------------------------------------------------------------------------
TBarramento* __fastcall TCommons::GetBarramento(int id_barramento)
{
	// Variaveis Locais
	TBarramento *barramento = NULL;
	//
	if (mapBarramento.find(id_barramento) != mapBarramento.end())
	{
		barramento = mapBarramento.find(id_barramento)->second;
	}
	return (barramento);
}

// -----------------------------------------------------------------------------
TBitolaCabo* __fastcall TCommons::GetBitolaCabo(int id_bitola)
{
	// Variaveis Locais
	TBitolaCabo *bitola = NULL;
	//
	if (mapBitola.find(id_bitola) != mapBitola.end())
	{
		bitola = mapBitola.find(id_bitola)->second;
	}
	return (bitola);
}

// -----------------------------------------------------------------------------
TCapElo* __fastcall TCommons::GetCapElo(int id_capElo)
{
	// Variaveis Locais
	TCapElo *capElo = NULL;
	//
	if (mapCapElo.find(id_capElo) != mapCapElo.end())
	{
		capElo = mapCapElo.find(id_capElo)->second;
	}
	return (capElo);
}

// -----------------------------------------------------------------------------
TClasseSubclasse* __fastcall TCommons::GetClasseSubclasse(int id_cl_subcl)
{
	// Variaveis Locais
	TClasseSubclasse *subclasse = NULL;
	//
	if (mapClasseSubclasse.find(id_cl_subcl) != mapClasseSubclasse.end())
	{
		subclasse = mapClasseSubclasse.find(id_cl_subcl)->second;
	}
	return (subclasse);
}

// -----------------------------------------------------------------------------
TClasseTensao* __fastcall TCommons::GetClasseTensao(int id_cl_tensao)
{
	// Variaveis Locais
	TClasseTensao *classe_tensao = NULL;

	if (mapClasseTensao.find(id_cl_tensao) != mapClasseTensao.end())
	{
		classe_tensao = mapClasseTensao.find(id_cl_tensao)->second;
	}
	return (classe_tensao);
}

// -----------------------------------------------------------------------------
TConfCircuito* __fastcall TCommons::GetConfCircuito(int id_conf_circuito)
{
	// Variaveis Locais
	TConfCircuito *conf_circuito = NULL;

	if (mapConfCircuito.find(id_conf_circuito) != mapConfCircuito.end())
	{
		conf_circuito = mapConfCircuito.find(id_conf_circuito)->second;
	}
	return (conf_circuito);
}

// -----------------------------------------------------------------------------
TCorrenteNom* __fastcall TCommons::GetCorrenteNom(int id_corrente_nom)
{
	// Variaveis Locais
	TCorrenteNom *corrente_nom = NULL;

	if (mapCorrenteNom.find(id_corrente_nom) != mapCorrenteNom.end())
	{
		corrente_nom = mapCorrenteNom.find(id_corrente_nom)->second;
	}
	return (corrente_nom);
}

// -----------------------------------------------------------------------------
TFasesConexao* __fastcall TCommons::GetFasesConexao(int id_fases_con)
{
	// Variaveis Locais
	TFasesConexao *fase_conexao = NULL;
	//
	if (mapFasesConexao.find(id_fases_con) != mapFasesConexao.end())
	{
		fase_conexao = mapFasesConexao.find(id_fases_con)->second;
	}
	return (fase_conexao);
}

// -----------------------------------------------------------------------------
TFormacaoCabo* __fastcall TCommons::GetFormacaoCabo(int id_formacao_cabo)
{
	// Variaveis Locais
	TFormacaoCabo *formacao_cabo = NULL;
	//
	if (mapFormacaoCabo.find(id_formacao_cabo) != mapFormacaoCabo.end())
	{
		formacao_cabo = mapFormacaoCabo.find(id_formacao_cabo)->second;
	}
	return (formacao_cabo);
}

// -----------------------------------------------------------------------------
TGeometriaCabo* __fastcall TCommons::GetGeometriaCabo(int id_geom_cabo)
{
	// Variaveis Locais
	TGeometriaCabo *geo_cabo = NULL;
	//
	if (mapGeometriaCabo.find(id_geom_cabo) != mapGeometriaCabo.end())
	{
		geo_cabo = mapGeometriaCabo.find(id_geom_cabo)->second;
	}
	return (geo_cabo);
}

// -----------------------------------------------------------------------------
TGrupoTarifario* __fastcall TCommons::GetGrupoTarifario(int id_grupo_tarif)
{
	// Variaveis Locais
	TGrupoTarifario *grupo_tarifario = NULL;
	//
	if (mapGrupoTarifario.find(id_grupo_tarif) != mapGrupoTarifario.end())
	{
		grupo_tarifario = mapGrupoTarifario.find(id_grupo_tarif)->second;
	}
	return (grupo_tarifario);
}

// -----------------------------------------------------------------------------
TIsolacaoCabo* __fastcall TCommons::GetIsolacaoCabo(int id_isolacao_cabo)
{
	// Variaveis Locais
	TIsolacaoCabo *iso_cabo = NULL;
	//
	if (mapIsolacaoCabo.find(id_isolacao_cabo) != mapIsolacaoCabo.end())
	{
		iso_cabo = mapIsolacaoCabo.find(id_isolacao_cabo)->second;
	}
	return (iso_cabo);
}

// -----------------------------------------------------------------------------
TLigacaoTrafo* __fastcall TCommons::GetLigacaoTrafo(int id_ligacao_trafo)
{
	// Variaveis Locais
	TLigacaoTrafo *lig_trafo = NULL;
	//
	if (mapLigacaoTrafo.find(id_ligacao_trafo) != mapLigacaoTrafo.end())
	{
		lig_trafo = mapLigacaoTrafo.find(id_ligacao_trafo)->second;
	}
	return (lig_trafo);
}

// -----------------------------------------------------------------------------
TMaterialCabo* __fastcall TCommons::GetMaterialCabo(int id_material_cabo)
{
	// Variaveis Locais
	TMaterialCabo *material_cabo = NULL;
	//
	if (mapMaterialCabo.find(id_material_cabo) != mapMaterialCabo.end())
	{
		material_cabo = mapMaterialCabo.find(id_material_cabo)->second;
	}
	return (material_cabo);
}

// -----------------------------------------------------------------------------
TPosse* __fastcall TCommons::GetPosse(int id_posse)
{
	// Variaveis Locais
	TPosse *posse = NULL;
	//
	if (mapPosse.find(id_posse) != mapPosse.end())
	{
		posse = mapPosse.find(id_posse)->second;
	}
	return (posse);
}

// -----------------------------------------------------------------------------
TRedeMT* __fastcall TCommons::GetRedeMT(int id_rede_mt)
{
	// Variaveis Locais
	TRedeMT *rede_mt = NULL;
	//
	if (mapRedeMT.find(id_rede_mt) != mapRedeMT.end())
	{
		rede_mt = mapRedeMT.find(id_rede_mt)->second;
	}
	return (rede_mt);
}

// -----------------------------------------------------------------------------
TSubestacao* __fastcall TCommons::GetSubestacao(int id_subestacao)
{
	// Variaveis Locais
	TSubestacao *subestacao = NULL;
	//
	if (mapSubestacao.find(id_subestacao) != mapSubestacao.end())
	{
		subestacao = mapSubestacao.find(id_subestacao)->second;
	}
	return (subestacao);
}

// -----------------------------------------------------------------------------
TTensao* __fastcall TCommons::GetTensao(int id_tensao)
{
	// Variaveis Locais
	TTensao *tensao = NULL;
	//
	if (mapTensao.find(id_tensao) != mapTensao.end())
	{
		tensao = mapTensao.find(id_tensao)->second;
	}
	return (tensao);
}

// -----------------------------------------------------------------------------
TTipoPotAparente* __fastcall TCommons::GetTipoPotAparente(int id_tipoS)
{
	// Variaveis Locais
	TTipoPotAparente *tipoS = NULL;
	//
	if (mapTipoPotAparente.find(id_tipoS) != mapTipoPotAparente.end())
	{
		tipoS = mapTipoPotAparente.find(id_tipoS)->second;
	}
	return (tipoS);
}

// -----------------------------------------------------------------------------
TTipoPotReativa* __fastcall TCommons::GetTipoPotReativa(int id_tipoQ)
{
	// Variaveis Locais
	TTipoPotReativa *tipoQ = NULL;
	//
	if (mapTipoPotReativa.find(id_tipoQ) != mapTipoPotReativa.end())
	{
		tipoQ = mapTipoPotReativa.find(id_tipoQ)->second;
	}
	return (tipoQ);
}

// -----------------------------------------------------------------------------
TTipoRegulador* __fastcall TCommons::GetTipoRegulador(int id_tipo_regulador)
{
	// Variaveis Locais
	TTipoRegulador *tipo_reg = NULL;
	//
	if (mapTipoRegulador.find(id_tipo_regulador) != mapTipoRegulador.end())
	{
		tipo_reg = mapTipoRegulador.find(id_tipo_regulador)->second;
	}
	return (tipo_reg);
}

// -----------------------------------------------------------------------------
TTipoTrafo* __fastcall TCommons::GetTipoTrafo(int id_tipo_trafo)
{
	// Variaveis Locais
	TTipoTrafo *tipo_trafo = NULL;
	//
	if (mapTipoTrafo.find(id_tipo_trafo) != mapTipoTrafo.end())
	{
		tipo_trafo = mapTipoTrafo.find(id_tipo_trafo)->second;
	}
	return (tipo_trafo);
}

// -----------------------------------------------------------------------------
TTipoUnidade* __fastcall TCommons::GetTipoUnidade(int id_tipo_unidade)
{
	// Variaveis Locais
	TTipoUnidade *tipo_uni = NULL;
	//
	if (mapTipoUnidade.find(id_tipo_unidade) != mapTipoUnidade.end())
	{
		tipo_uni = mapTipoUnidade.find(id_tipo_unidade)->second;
	}
	return (tipo_uni);
}

// -----------------------------------------------------------------------------
TTrafoSubestacao* __fastcall TCommons::GetTrafoSubestacao(int id_trafo_subestacao)
{
	// Variaveis Locais
	TTrafoSubestacao *trafo_subestacao = NULL;
	//
	if (mapTrafoSubestacao.find(id_trafo_subestacao) != mapTrafoSubestacao.end())
	{
		trafo_subestacao = mapTrafoSubestacao.find(id_trafo_subestacao)->second;
	}
	return (trafo_subestacao);
}

// -------------------------------------------------------------------------------------------------
std::map<int, TArranjoBDGD*>__fastcall TCommons::GetmapArranjo(void)
{
	return (mapArranjo);
}

// -------------------------------------------------------------------------------------------------
std::map<int, TBarraBDGD*>__fastcall TCommons::GetmapBarra(void)
{
	return (mapBarra);
}

// -------------------------------------------------------------------------------------------------
std::map<int, TBarramento*>__fastcall TCommons::GetmapBarramento(void)
{
	return (mapBarramento);
}

// -------------------------------------------------------------------------------------------------
std::map<int, TRedeMT*>__fastcall TCommons::GetmapRedeMT(void)
{
	return (mapRedeMT);
}

// -------------------------------------------------------------------------------------------------
std::map<int, TSubestacao*>__fastcall TCommons::GetmapSubestacao(void)
{
	return (mapSubestacao);
}

// -------------------------------------------------------------------------------------------------
std::map<int, TTrafoSubestacao*>__fastcall TCommons::GetmapTrafoSub(void)
{
	//
	return (mapTrafoSubestacao);
}

// -------------------------------------------------------------------------------------------------
std::map<int, TTipoUnidade*>__fastcall TCommons::GetmapTipoUnidade(void)
{
	//
	return (mapTipoUnidade);
}

// -------------------------------------------------------------------------------------------------
void __fastcall TCommons::LeDAO(BDGDDAO *ext_dao)
{
	//
	this->dao = ext_dao;
}

// -------------------------------------------------------------------------------------------------
void __fastcall TCommons::LimpaMapas(void)
{
	//
	mapAreaLoc.clear();
	mapArranjo.clear();
	mapBarra.clear();
	mapBarramento.clear();
	mapBitola.clear();
	mapCapElo.clear();
	mapClasseSubclasse.clear();
	mapClasseTensao.clear();
	mapConfCircuito.clear();
	mapCorrenteNom.clear();
	mapFasesConexao.clear();
	mapFormacaoCabo.clear();
	mapGeometriaCabo.clear();
	mapGrupoTarifario.clear();
	mapIsolacaoCabo.clear();
	mapLigacaoTrafo.clear();
	mapMaterialCabo.clear();
	mapPosse.clear();
	mapRedeMT.clear();
	mapSubestacao.clear();
	mapTensao.clear();
	mapTipoPotAparente.clear();
	mapTipoPotReativa.clear();
	mapTipoRegulador.clear();
	mapTipoTrafo.clear();
	mapTipoUnidade.clear();
	mapTrafoSubestacao.clear();
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::PM_GetBatch(void)
{
	return true;
}

// -------------------------------------------------------------------------------------------------
void __fastcall TCommons::PM_SetBatch(bool batch)

{
	//
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadAreaLoc(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TAreaLocalizacao *area_loc;
	VTSQLiteTable *table;
	int id_area_loc;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: AreaLocalizacao");
	}
	try
	{
		table = dao->ExecuteSqlAreaLoc();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_area_loc = table->GetIntField("ID_AREA_LOCALIZACAO");
				if (GetAreaLoc(id_area_loc) == NULL)
				{
					area_loc = NewObjAreaLocalizacao();
					area_loc->Id = id_area_loc;
					area_loc->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
					area_loc->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
					mapAreaLoc[area_loc->Id] = area_loc;
					lisDOMINIO->Add(area_loc);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: AreaLocalizacao.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: AreaLocalizacao.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadArranjo(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TArranjoBDGD *arranjo;
	VTSQLiteTable *table;
	int id_arranjo;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: ArranjoBDGD");
	}
	try
	{
		table = dao->ExecuteSqlArranjo();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_arranjo = table->GetIntField("ID_ARRANJO");
				if (GetArranjo(id_arranjo) == NULL)
				{
					arranjo = NewObjArranjo();
					arranjo->Id = id_arranjo;
					arranjo->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
					arranjo->Geometria_Cabo =
						GetGeometriaCabo(table->GetIntField("ID_GEOMETRIA_CABO"));
					arranjo->Formacao_Cabo =
						GetFormacaoCabo(table->GetIntField("ID_FORMACAO_CABO"));
					arranjo->Bitola_Cabo_1 = GetBitolaCabo(table->GetIntField("ID_BITOLA_CABO_1"));
					arranjo->Bitola_Cabo_2 = GetBitolaCabo(table->GetIntField("ID_BITOLA_CABO_2"));
					arranjo->Bitola_Cabo_3 = GetBitolaCabo(table->GetIntField("ID_BITOLA_CABO_3"));
					arranjo->Bitola_Cabo_N = GetBitolaCabo(table->GetIntField("ID_BITOLA_CABO_N"));
					arranjo->Material_Cabo_1 =
						GetMaterialCabo(table->GetIntField("ID_MATERIAL_CABO_1"));
					arranjo->Material_Cabo_2 =
						GetMaterialCabo(table->GetIntField("ID_MATERIAL_CABO_2"));
					arranjo->Material_Cabo_3 =
						GetMaterialCabo(table->GetIntField("ID_MATERIAL_CABO_3"));
					arranjo->Material_Cabo_N =
						GetMaterialCabo(table->GetIntField("ID_MATERIAL_CABO_N"));
					arranjo->Isolacao_Cabo_1 =
						GetIsolacaoCabo(table->GetIntField("ID_ISOLACAO_CABO_1"));
					arranjo->Isolacao_Cabo_2 =
						GetIsolacaoCabo(table->GetIntField("ID_ISOLACAO_CABO_2"));
					arranjo->Isolacao_Cabo_3 =
						GetIsolacaoCabo(table->GetIntField("ID_ISOLACAO_CABO_3"));
					arranjo->Isolacao_Cabo_N =
						GetIsolacaoCabo(table->GetIntField("ID_ISOLACAO_CABO_N"));
					arranjo->Num_Conductor_Fase = table->GetIntField("NUM_CONDUTOR_FASE");
					arranjo->R1 = table->GetFloatField("R1");
					arranjo->X1 = table->GetFloatField("X1");
					arranjo->Fator_Conversao = table->GetFloatField("FATOR_CONVERSAO");
					arranjo->Corrente_Nom_A = table->GetFloatField("CORRENTE_NOM_CONDUTOR");
					arranjo->Corrente_Max_A = table->GetFloatField("CORRENTE_MAX_CONDUTOR");
					arranjo->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
					mapArranjo[arranjo->Id] = arranjo;
					lisENTIDADES->Add(arranjo);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: ArranjoBDGD.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: ArranjoBDGD.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadBarra(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TBarraBDGD *barra;
	VTSQLiteTable *table;
	int id_barra;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: Barra");
	}
	try
	{
		table = dao->ExecuteSqlBarra();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_barra = table->GetIntField("ID_BARRA");
				if (GetBarra(id_barra) == NULL)
				{
					barra = NewObjBarraBDGD();
					barra->Id = id_barra;
					barra->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
					barra->Latitude = table->GetFloatField("LATITUDE");
					barra->Longitude = table->GetFloatField("LONGITUDE");
					mapBarra[barra->Id] = barra;
					lisENTIDADES->Add(barra);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: Barra.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: Barra.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadBarramento(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TBarramento *barramento;
	VTSQLiteTable *table;
	int id_barramento;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: Barramento");
	}
	try
	{
		table = dao->ExecuteSqlBarramento();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_barramento = table->GetIntField("ID_BARRAMENTO");
				if (GetBarramento(id_barramento) == NULL)
				{
					barramento = NewObjBarramento();
					barramento->Id = id_barramento;
					barramento->Id_Sub = table->GetIntField("ID_SUBESTACAO");
					barramento->Id_Barra = table->GetIntField("ID_BARRA");
					barramento->Tensao = GetTensao(table->GetIntField("ID_TENSAO"));
					barramento->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
					barramento->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
					mapBarramento[barramento->Id] = barramento;
					lisENTIDADES->Add(barramento);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: Barramento.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: Barramento.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadBitola(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TBitolaCabo *bitola;
	VTSQLiteTable *table;
	int id_bitola;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: BitolaCabo");
	}
	try
	{
		table = dao->ExecuteSqlBitolaCabo();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_bitola = table->GetIntField("ID_BITOLA_CABO");
				if (GetBitolaCabo(id_bitola) == NULL)
				{
					bitola = NewObjBitolaCabo();
					bitola->Id = id_bitola;
					bitola->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
					mapBitola[bitola->Id] = bitola;
					lisDOMINIO->Add(bitola);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: BitolaCabo.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: BitolaCabo.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadCapElo(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TCapElo *capElo;
	VTSQLiteTable *table;
	int id_capElo;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: CapElo");
	}
	try
	{
		table = dao->ExecuteSqlCapElo();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_capElo = table->GetIntField("ID_CAPACIDADE_ELO_FUSIVEL");
				if (GetCapElo(id_capElo) == NULL)
				{
					capElo = NewObjCapElo();
					capElo->Id = id_capElo;
					capElo->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
					capElo->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
					mapCapElo[capElo->Id] = capElo;
					lisDOMINIO->Add(capElo);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: CapElo.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: CapElo.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadClasseSubclasse(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TClasseSubclasse *classe_subclasse;
	VTSQLiteTable *table;
	int id_classe_subclasse;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: ClasseSubclasse");
	}
	try
	{
		table = dao->ExecuteSqlClasseSubclasse();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_classe_subclasse = table->GetIntField("ID_CLASSE_SUBCLASSE");
				if (GetClasseSubclasse(id_classe_subclasse) == NULL)
				{
					classe_subclasse = NewObjClasseSubclasse();
					classe_subclasse->Id = id_classe_subclasse;
					classe_subclasse->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
					classe_subclasse->Descricao =
						System::UTF8String(table->GetStringField("DESCRICAO"));
					mapClasseSubclasse[classe_subclasse->Id] = classe_subclasse;
					lisDOMINIO->Add(classe_subclasse);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: ClasseSubclasse.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: ClasseSubclasse.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadClasseTensao(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TClasseTensao *classe_tensao;
	VTSQLiteTable *table;
	int id_classeTensao;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: ClasseTensao");
	}
	try
	{
		table = dao->ExecuteSqlClasseTensao();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_classeTensao = table->GetIntField("ID_CLASSE_TENSAO");
				if (GetClasseTensao(id_classeTensao) == NULL)
				{
					classe_tensao = NewObjClasseTensao();
					classe_tensao->Id = id_classeTensao;
					classe_tensao->Tensao_V = table->GetIntField("TENSAO");
					classe_tensao->Descricao =
						System::UTF8String(table->GetStringField("DESCRICAO"));
					mapClasseTensao[classe_tensao->Id] = classe_tensao;
					lisDOMINIO->Add(classe_tensao);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: ClasseTensao.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: ClasseTensao.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadConfCircuito(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TConfCircuito *conf_circuito;
	VTSQLiteTable *table;
	int id_conf_cto;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: ConfCircuito");
	}
	try
	{
		table = dao->ExecuteSqlConfCircuito();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_conf_cto = table->GetIntField("ID_CONFIGURACAO_CIRCUITO");
				if (GetConfCircuito(id_conf_cto) == NULL)
				{
					conf_circuito = NewObjConfCircuito();
					conf_circuito->Id = id_conf_cto;
					conf_circuito->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
					conf_circuito->Descricao =
						System::UTF8String(table->GetStringField("DESCRICAO"));
					mapConfCircuito[conf_circuito->Id] = conf_circuito;
					lisDOMINIO->Add(conf_circuito);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: ConfCircuito.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: ConfCircuito.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadCorrenteNom(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TCorrenteNom *corr_nom;
	VTSQLiteTable *table;
	int id_corr_nom;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: CorrenteNom");
	}
	try
	{
		table = dao->ExecuteSqlCorrenteNom();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_corr_nom = table->GetIntField("ID_CORRENTE_NOMINAL");
				if (GetCorrenteNom(id_corr_nom) == NULL)
				{
					corr_nom = NewObjCorrenteNom();
					corr_nom->Id = id_corr_nom;
					corr_nom->Corrente_A = table->GetFloatField("CORRENTE");
					corr_nom->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
					mapCorrenteNom[corr_nom->Id] = corr_nom;
					lisDOMINIO->Add(corr_nom);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: CorrenteNom.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: CorrenteNom.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadEntidadesEmpresa(void) throw(EImportaUgrid)
{
	// Variáveis locais
	bool sucesso = true;

	try
	{
		if (!ReadArranjo())
			return (false);
		if (!ReadBarra())
			return (false);
		if (!ReadBarramento())
			return (false);
		if (!ReadSubestacao())
			return (false);
		if (!ReadTrafoSub())
			return (false);
		if (!ReadRedeMT())
			return (false);
	}
	catch (Exception &e)
	{
		sucesso = false;
		throw EImportaUgrid(e.Message);
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadFasesConexao(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TFasesConexao *fases_con;
	VTSQLiteTable *table;
	int id_fases_con;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: FasesConexao");
	}
	try
	{
		table = dao->ExecuteSqlFasesConexao();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_fases_con = table->GetIntField("ID_FASES_CONEXAO");
				if (GetFasesConexao(id_fases_con) == NULL)
				{
					fases_con = NewObjFasesConexao();
					fases_con->Id = id_fases_con;
					fases_con->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
					fases_con->Qtd_Fios = table->GetIntField("QTD_FIOS");
					fases_con->Fases = table->GetIntField("FASES");
					fases_con->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
					mapFasesConexao[fases_con->Id] = fases_con;
					lisDOMINIO->Add(fases_con);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: FasesConexao.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: FasesConexao.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadFormacaoCabo(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TFormacaoCabo *formacao_cabo;
	VTSQLiteTable *table;
	int id_formacao_cabo;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: FormacaoCabo");
	}
	try
	{
		table = dao->ExecuteSqlFormacaoCabo();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_formacao_cabo = table->GetIntField("ID_FORMACAO_CABO");
				if (GetFormacaoCabo(id_formacao_cabo) == NULL)
				{
					formacao_cabo = NewObjFormacaoCabo();
					formacao_cabo->Id = id_formacao_cabo;
					formacao_cabo->Descricao =
						System::UTF8String(table->GetStringField("DESCRICAO"));
					mapFormacaoCabo[formacao_cabo->Id] = formacao_cabo;
					lisDOMINIO->Add(formacao_cabo);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			throw EImportaUgrid("Coluna inexistente em Commons: FormacaoCabo.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			throw EImportaUgrid("Erro inesperado em Commons: FormacaoCabo.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadGeometriaCabo(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TGeometriaCabo *geometria_cabo;
	VTSQLiteTable *table;
	int id_geometria_cabo;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: GeometriaCabo");
	}
	try
	{
		table = dao->ExecuteSqlGeometriaCabo();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_geometria_cabo = table->GetIntField("ID_GEOMETRIA_CABO");
				if (GetGeometriaCabo(id_geometria_cabo) == NULL)
				{
					geometria_cabo = NewObjGeometriaCabo();
					geometria_cabo->Id = id_geometria_cabo;
					geometria_cabo->Descricao =
						System::UTF8String(table->GetStringField("DESCRICAO"));
					mapGeometriaCabo[geometria_cabo->Id] = geometria_cabo;
					lisDOMINIO->Add(geometria_cabo);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			throw EImportaUgrid("Coluna inexistente em Commons: GeometriaCabo.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: GeometriaCabo.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadGrupoTarifario(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TGrupoTarifario *grupo_tarifario;
	VTSQLiteTable *table;
	int id_grupo_tarifario;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: GrupoTarifario");
	}
	try
	{
		table = dao->ExecuteSqlGrupoTarifario();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_grupo_tarifario = table->GetIntField("ID_GRUPO_TARIFARIO");
				if (GetGrupoTarifario(id_grupo_tarifario) == NULL)
				{
					grupo_tarifario = NewObjGrupoTarifario();
					grupo_tarifario->Id = id_grupo_tarifario;
					grupo_tarifario->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
					grupo_tarifario->Descricao =
						System::UTF8String(table->GetStringField("DESCRICAO"));
					mapGrupoTarifario[grupo_tarifario->Id] = grupo_tarifario;
					lisDOMINIO->Add(grupo_tarifario);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: GrupoTarifario.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: GrupoTarifario.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadIsolacaoCabo(void)  throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TIsolacaoCabo *isolacao_cabo;
	VTSQLiteTable *table;
	int id_isolacao_cabo;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: IsolacaoCabo");
	}
	try
	{
		table = dao->ExecuteSqlIsolacaoCabo();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_isolacao_cabo = table->GetIntField("ID_ISOLACAO_CABO");
				if (GetIsolacaoCabo(id_isolacao_cabo) == NULL)
				{
					isolacao_cabo = NewObjIsolacaoCabo();
					isolacao_cabo->Id = id_isolacao_cabo;
					isolacao_cabo->Descricao =
						System::UTF8String(table->GetStringField("DESCRICAO"));
					mapIsolacaoCabo[isolacao_cabo->Id] = isolacao_cabo;
					lisDOMINIO->Add(isolacao_cabo);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: IsolacaoCabo.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: IsolacaoCabo.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadLigacaoTrafo(void)  throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TLigacaoTrafo *ligacao_trafo;
	VTSQLiteTable *table;
	int id_ligacao_trafo;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: LigacaoTrafo");
	}
	try
	{
		table = dao->ExecuteSqlLigacaoTrafo();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_ligacao_trafo = table->GetIntField("ID_LIGACAO_TRAFO");
				if (GetLigacaoTrafo(id_ligacao_trafo) == NULL)
				{
					ligacao_trafo = NewObjLigacaoTrafo();
					ligacao_trafo->Id = id_ligacao_trafo;
					ligacao_trafo->Descricao =
						System::UTF8String(table->GetStringField("DESCRICAO"));
					mapLigacaoTrafo[ligacao_trafo->Id] = ligacao_trafo;
					lisDOMINIO->Add(ligacao_trafo);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: LigacaoTrafo.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: LigacaoTrafo.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadMaterialCabo(void)  throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TMaterialCabo *material_cabo;
	VTSQLiteTable *table;
	int id_material_cabo;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: MaterialCabo");
	}
	try
	{
		table = dao->ExecuteSqlMaterialCabo();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_material_cabo = table->GetIntField("ID_MATERIAL_CABO");
				if (GetMaterialCabo(id_material_cabo) == NULL)
				{
					material_cabo = NewObjMaterialCabo();
					material_cabo->Id = id_material_cabo;
					material_cabo->Descricao =
						System::UTF8String(table->GetStringField("DESCRICAO"));
					mapMaterialCabo[material_cabo->Id] = material_cabo;
					lisDOMINIO->Add(material_cabo);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: MaterialCabo.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: MaterialCabo.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadPosse(void)  throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TPosse *posse;
	VTSQLiteTable *table;
	int id_posse;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: Posse");
	}
	try
	{
		table = dao->ExecuteSqlPosse();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_posse = table->GetIntField("ID_POSSE");
				if (GetPosse(id_posse) == NULL)
				{
					posse = NewObjPosse();
					posse->Id = id_posse;
					posse->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
					posse->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
					mapPosse[posse->Id] = posse;
					lisDOMINIO->Add(posse);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: Posse.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: Posse.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadRedeMT(void)  throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TRedeMT *rede_mt;
	TSubestacao *subestacao;
	VTSQLiteTable *table;
	int id_rede_mt;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: RedeMT");
	}
	try
	{
		table = dao->ExecuteSqlRedeMT();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_rede_mt = table->GetIntField("ID_REDE_MT");
				if (GetRedeMT(id_rede_mt) == NULL)
				{
					rede_mt = NewObjRedeMT();
					rede_mt->Id = id_rede_mt;
					rede_mt->Id_Sub = table->GetIntField("ID_SUBESTACAO");
					rede_mt->Id_Barramento = table->GetIntField("ID_BARRAMENTO");
					rede_mt->Id_Trafo_Sub = table->GetIntField("ID_TRAFO_SUBESTACAO");
					rede_mt->Id_Barra_Ini = table->GetIntField("ID_BARRA_INICIAL");
					rede_mt->Tensao_Nom = GetTensao(table->GetIntField("ID_TENSAO_NOMINAL"));
					rede_mt->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
					rede_mt->Nome = System::UTF8String(table->GetStringField("NOME"));
					rede_mt->Tensao_Oper_pu = table->GetFloatField("TENSAO_OPERACAO");
					rede_mt->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
					rede_mt->Id_Barra_Trafo = table->GetIntField("ID_BARRA_TRAFO");
					mapRedeMT[rede_mt->Id] = rede_mt;
					lisENTIDADES->Add(rede_mt);
					subestacao = GetSubestacao(rede_mt->Id_Sub);
					if (subestacao != NULL)
					{
						subestacao->InserePrimario(rede_mt);
					}
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: RedeMT.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: RedeMT.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadSubestacao(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TSubestacao *sub;
	VTSQLiteTable *table;
	int id_sub;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: Subestacao");
	}
	try
	{
		table = dao->ExecuteSqlSubestacao();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_sub = table->GetIntField("ID_SUBESTACAO");
				if (GetSubestacao(id_sub) == NULL)
				{
					sub = NewObjSubestacao();
					sub->Id = id_sub;
					sub->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
					sub->Nome = System::UTF8String(table->GetStringField("NOME"));
					sub->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
					mapSubestacao[sub->Id] = sub;
					lisENTIDADES->Add(sub);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			throw EImportaUgrid("Coluna inexistente em Commons: Subestacao.");
			sucesso = false;
		}
		catch (Exception &e)
		{ // erro inesperado
			throw EImportaUgrid("Erro inesperado em Commons: Subestacao.");
			sucesso = false;
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadTabelasDominio(void)
{
	// A leitura será feita na ordem alfabética das tabelas de domínio
	// Variáveis locais
	bool sucesso = true;

	try
	{
		if (!ReadAreaLoc())
			return (false);
		if (!ReadBitola())
			return (false);
		if (!ReadCapElo())
			return (false);
		if (!ReadClasseSubclasse())
			return (false);
		if (!ReadClasseTensao())
			return (false);
		if (!ReadConfCircuito())
			return (false);
		if (!ReadCorrenteNom())
			return (false);
		if (!ReadFasesConexao())
			return (false);
		if (!ReadFormacaoCabo())
			return (false);
		if (!ReadGeometriaCabo())
			return (false);
		if (!ReadGrupoTarifario())
			return (false);
		if (!ReadIsolacaoCabo())
			return (false);
		if (!ReadLigacaoTrafo())
			return (false);
		if (!ReadMaterialCabo())
			return (false);
		if (!ReadPosse())
			return (false);
		if (!ReadTensao())
			return (false);
		if (!ReadTipoPotAparente())
			return (false);
		if (!ReadTipoPotReativa())
			return (false);
		if (!ReadTipoRegulador())
			return (false);
		if (!ReadTipoTrafo())
			return (false);
		if (!ReadTipoUnidade())
			return (false);
	}
	catch (Exception &e)
	{
		throw EImportaUgrid(e.Message);
		sucesso = false;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadTensao(void)   throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TTensao *tensao;
	VTSQLiteTable *table;
	int id_tensao;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: Tensao");
	}
	try
	{
		table = dao->ExecuteSqlTensao();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_tensao = table->GetIntField("ID_TENSAO");
				if (GetTensao(id_tensao) == NULL)
				{
					tensao = NewObjTensao();
					tensao->Id = id_tensao;
					tensao->Tensao_V = table->GetFloatField("TENSAO");
					tensao->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
					mapTensao[tensao->Id] = tensao;
					lisDOMINIO->Add(tensao);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: Tensao.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: Tensao.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadTipoPotAparente(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TTipoPotAparente *tipo_pot_aparente;
	VTSQLiteTable *table;
	int id_tipo_pot_aparente;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: TipoPotAparente");
	}
	try
	{
		table = dao->ExecuteSqlTipoPotAparente();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_tipo_pot_aparente = table->GetIntField("ID_TIPO_POTENCIA_APARENTE");
				if (GetTipoPotAparente(id_tipo_pot_aparente) == NULL)
				{
					tipo_pot_aparente = NewObjTipoPotAparente();
					tipo_pot_aparente->Id = id_tipo_pot_aparente;
					tipo_pot_aparente->Pot_Aparente_kVA = table->GetFloatField("POTENCIA_APARENTE");
					tipo_pot_aparente->Descricao =
						System::UTF8String(table->GetStringField("DESCRICAO"));
					mapTipoPotAparente[tipo_pot_aparente->Id] = tipo_pot_aparente;
					lisDOMINIO->Add(tipo_pot_aparente);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: TipoPotAparente.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: TipoPotAparente.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadTipoPotReativa(void)  throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TTipoPotReativa *tipo_pot_reativa;
	VTSQLiteTable *table;
	int id_tipo_pot_reativa;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: TipoPotReativa");
	}
	try
	{
		table = dao->ExecuteSqlTipoPotReativa();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_tipo_pot_reativa = table->GetIntField("ID_TIPO_POTENCIA_REATIVA");
				if (GetTipoPotReativa(id_tipo_pot_reativa) == NULL)
				{
					tipo_pot_reativa = NewObjTipoPotReativa();
					tipo_pot_reativa->Id = id_tipo_pot_reativa;
					tipo_pot_reativa->Pot_Reativa_kVAr = table->GetFloatField("POTENCIA_REATIVA");
					tipo_pot_reativa->Descricao =
						System::UTF8String(table->GetStringField("DESCRICAO"));
					mapTipoPotReativa[tipo_pot_reativa->Id] = tipo_pot_reativa;
					lisDOMINIO->Add(tipo_pot_reativa);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: TipoPotReativa.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: TipoPotReativa.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadTipoRegulador(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TTipoRegulador *tipo_regulador;
	VTSQLiteTable *table;
	int id_tipo_regulador;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: TipoRegulador");
	}
	try
	{
		table = dao->ExecuteSqlTipoRegulador();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_tipo_regulador = table->GetIntField("ID_TIPO_REGULADOR");
				if (GetTipoRegulador(id_tipo_regulador) == NULL)
				{
					tipo_regulador = NewObjTipoRegulador();
					tipo_regulador->Id = id_tipo_regulador;
					tipo_regulador->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
					tipo_regulador->Descricao =
						System::UTF8String(table->GetStringField("DESCRICAO"));
					mapTipoRegulador[tipo_regulador->Id] = tipo_regulador;
					lisDOMINIO->Add(tipo_regulador);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: TipoRegulador.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: TipoRegulador.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadTipoTrafo(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TTipoTrafo *tipo_trafo;
	VTSQLiteTable *table;
	int id_tipo_trafo;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: TipoTrafo");
	}
	try
	{
		table = dao->ExecuteSqlTipoTrafo();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_tipo_trafo = table->GetIntField("ID_TIPO_TRAFO");
				if (GetTipoTrafo(id_tipo_trafo) == NULL)
				{
					tipo_trafo = NewObjTipoTrafo();
					tipo_trafo->Id = id_tipo_trafo;
					tipo_trafo->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
					tipo_trafo->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
					mapTipoTrafo[tipo_trafo->Id] = tipo_trafo;
					lisDOMINIO->Add(tipo_trafo);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: TipoTrafo.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: TipoTrafo.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadTipoUnidade(void)  throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TTipoUnidade *tipo_unidade;
	AnsiString descr = "";
	VTSQLiteTable *table;
	int id_tipo_unidade;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: TipoUnidade");
	}
	try
	{
		table = dao->ExecuteSqlTipoUnidade();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_tipo_unidade = table->GetIntField("ID_TIPO_UNIDADE");
				if (GetTipoUnidade(id_tipo_unidade) == NULL)
				{
					tipo_unidade = NewObjTipoUnidade();
					tipo_unidade->Id = id_tipo_unidade;
					tipo_unidade->Unidade = System::UTF8String(table->GetStringField("UNIDADE"));
					descr = System::UTF8String(table->GetStringField("DESCRICAO"));
					tipo_unidade->Descricao = descr.SubString(1,27);
					mapTipoUnidade[tipo_unidade->Id] = tipo_unidade;
					lisDOMINIO->Add(tipo_unidade);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: TipoUnidade.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: TipoUnidade.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TCommons::ReadTrafoSub(void) throw(EImportaUgrid)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString cmd_sql, strlog;
	TTrafoSubestacao *trafo_sub;
	VTSQLiteTable *table;
	int id_trafo_sub;

	// atualiza progresso
	if (!BatchMode)
	{
		if (progresso == NULL)
			progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
		if (progresso)
			progresso->Add("Leitura de Commons: TrafoSubestacao");
	}
	try
	{
		table = dao->ExecuteSqlTrafoSub();
	}
	catch (EImportaUgrid &e)
	{
		sucesso = false;
	}
	if (sucesso)
	{
		try
		{
			for (int row = 0; row < table->NumRows(); row++)
			{
				table->SetRow(row);
				id_trafo_sub = table->GetIntField("ID_TRAFO_SUBESTACAO");
				if (GetTrafoSubestacao(id_trafo_sub) == NULL)
				{
					trafo_sub = NewObjTrafoSubestacao();
					trafo_sub->Id = id_trafo_sub;
					trafo_sub->Id_Sub = table->GetIntField("ID_SUBESTACAO");
					trafo_sub->Id_Barramento_1 = table->GetIntField("ID_BARRAMENTO_1");
					trafo_sub->Id_Barramento_2 = table->GetIntField("ID_BARRAMENTO_2");
					trafo_sub->Id_Barramento_3 = table->GetIntField("ID_BARRAMENTO_3");
					trafo_sub->Id_Barra_1 = table->GetIntField("ID_BARRA_1");
					trafo_sub->Id_Barra_2 = table->GetIntField("ID_BARRA_2");
					trafo_sub->Id_Barra_3 = table->GetIntField("ID_BARRA_3");
					trafo_sub->Fases_Con_Pri =
						GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO_PRI"));
					trafo_sub->Fases_Con_Sec =
						GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO_SEC"));
					trafo_sub->Fases_Con_Ter =
						GetFasesConexao(table->GetIntField("ID_FASES_CONEXAO_TER"));
					trafo_sub->Tipo_Trafo = GetTipoTrafo(table->GetIntField("ID_TIPO_TRAFO"));
					trafo_sub->Classe_Tensao =
						GetClasseTensao(table->GetIntField("ID_CLASSE_TENSAO"));
					trafo_sub->Tipo_Pot_Aparente =
						GetTipoPotAparente(table->GetIntField("ID_TIPO_POTENCIA_NOMINAL"));
					trafo_sub->Ligacao_Trafo =
						GetLigacaoTrafo(table->GetIntField("ID_LIGACAO_TRAFO"));
					trafo_sub->Tensao_Pri = GetTensao(table->GetIntField("ID_TENSAO_PRI"));
					trafo_sub->Tensao_Sec = GetTensao(table->GetIntField("ID_TENSAO_SEC"));
					trafo_sub->Tensao_Ter = GetTensao(table->GetIntField("ID_TENSAO_TER"));
					trafo_sub->Codigo = System::UTF8String(table->GetStringField("CODIGO"));
					trafo_sub->S_Nom = table->GetFloatField("POTENCIA_NOMINAL_APARENTE");
					trafo_sub->S_VentFor_1_MVA =
						table->GetFloatField("POTENCIA_NOMINAL_VENT_FORC_1");
					trafo_sub->S_VentFor_2_MVA =
						table->GetFloatField("POTENCIA_NOMINAL_VENT_FORC_2");
					trafo_sub->Perda_Fe_Perc = table->GetFloatField("PERDA_FERRO_PER");
					trafo_sub->Perda_Total_Perc = table->GetFloatField("PERDA_TOTAL_PER");
					trafo_sub->Descricao = System::UTF8String(table->GetStringField("DESCRICAO"));
					mapTrafoSubestacao[trafo_sub->Id] = trafo_sub;
					lisENTIDADES->Add(trafo_sub);
				}
			}
		}
		catch (ESQLite &e)
		{ // provavelmente uma coluna que nao existe
			sucesso = false;
			throw EImportaUgrid("Coluna inexistente em Commons: TrafoSubestacao.");
		}
		catch (Exception &e)
		{ // erro inesperado
			sucesso = false;
			throw EImportaUgrid("Erro inesperado em Commons: TrafoSubestacao.");
		}
	}
	if (table != NULL)
	{
		delete table;
	}
	return (sucesso);
}

// --------------------------------------------------------------------------------------------- eof
