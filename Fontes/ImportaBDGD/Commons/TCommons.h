// --------------------------------------------------------------------------------------------- bof
#ifndef TCommonsH
#define TCommonsH

// ---------------------------------------------------------------------------------------- includes
#include <stdexcept>
#include <list>
#include <Classes.hpp>
#include <System.Classes.hpp>

// Project include
//#include <Fontes\LocalDB\SQLite\ESQLite.h>
#include "VTCommons.h"
#include <map>

// ------------------------------------------------------------------------------------ declarations
//class VTExtrator;
class BDGDDAO;
class EImportaUgrid;
class VTLog;
class VTProgresso;

// ------------------------------------------------------------------------------------------- class
class TCommons : public VTCommons
{
public:
		 __fastcall TCommons(VTApl *apl, BDGDDAO *dao);
		 __fastcall ~TCommons(void);
	bool __fastcall Clear(void);
	void __fastcall LeDAO(BDGDDAO *ext_dao);
	void __fastcall LimpaMapas(void);
	bool __fastcall ReadTabelasDominio(void);

	// Leitura do UGRID
	   bool          __fastcall  ReadAreaLoc(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadArranjo(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadBarra(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadBarramento(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadBitola(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadCapElo(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadClasseSubclasse(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadClasseTensao(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadConfCircuito(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadCorrenteNom(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadEntidadesEmpresa(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadFasesConexao(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadFormacaoCabo(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadGeometriaCabo(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadGrupoTarifario(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadIsolacaoCabo(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadLigacaoTrafo(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadMaterialCabo(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadPosse(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadRedeMT(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadSubestacao(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadTensao(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadTipoPotAparente(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadTipoPotReativa(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadTipoRegulador(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadTipoTrafo(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadTipoUnidade(void) throw(EImportaUgrid);
	   bool          __fastcall  ReadTrafoSub(void) throw(EImportaUgrid);

	// Return das maps inteiros


	// Return dos elementos dos maps  - Tabelas de dominio
	bool __fastcall PM_GetBatch(void);
	void __fastcall PM_SetBatch(bool batch);
	TAreaLocalizacao* __fastcall GetAreaLoc(int id_area);
	TArranjoBDGD* __fastcall GetArranjo(int id_arranjo);
	TBarraBDGD* __fastcall GetBarra(int id_barra);
	TBarramento* __fastcall GetBarramento(int id_barramento);
	TBitolaCabo* __fastcall GetBitolaCabo(int id_bitola);
	TCapElo* __fastcall GetCapElo(int id_capElo);
	TClasseSubclasse* __fastcall GetClasseSubclasse(int id_cl_subcl);
	TClasseTensao* __fastcall GetClasseTensao(int id_cl_tensao);
	TConfCircuito* __fastcall GetConfCircuito(int id_conf_circuito);
	TCorrenteNom* __fastcall GetCorrenteNom(int id_corrente_nom);
	TFasesConexao* __fastcall GetFasesConexao(int id_fases_con);
	TFormacaoCabo* __fastcall GetFormacaoCabo(int id_formacao_cabo);
	TGeometriaCabo* __fastcall GetGeometriaCabo(int id_geom_cabo);
	TGrupoTarifario* __fastcall GetGrupoTarifario(int id_grupo_tarif);
	TIsolacaoCabo* __fastcall GetIsolacaoCabo(int id_isolacao_cabo);
	TLigacaoTrafo* __fastcall GetLigacaoTrafo(int id_ligacao_trafo);
	TMaterialCabo* __fastcall GetMaterialCabo(int id_material_cabo);
	TPosse* __fastcall GetPosse(int id_posse);
	TRedeMT* __fastcall GetRedeMT(int id_rede_mt);
	TSubestacao* __fastcall GetSubestacao(int id_subestacao);
	TTensao* __fastcall GetTensao(int id_tensao);
	TTipoPotAparente* __fastcall GetTipoPotAparente(int id_tipoS);
	TTipoPotReativa* __fastcall GetTipoPotReativa(int id_tipoQ);
	TTipoRegulador* __fastcall GetTipoRegulador(int id_tipo_regulador);
	TTipoTrafo* __fastcall GetTipoTrafo(int id_tipo_trafo);
	TTipoUnidade* __fastcall GetTipoUnidade(int id_tipo_unidade);
	TTrafoSubestacao* __fastcall GetTrafoSubestacao(int id_trafo_subestacao);

	std::map<int,TArranjoBDGD*> __fastcall GetmapArranjo(void);
	std::map<int,TBarraBDGD*> __fastcall GetmapBarra(void);
	std::map<int,TBarramento*> __fastcall GetmapBarramento(void);
	std::map<int,TRedeMT*> __fastcall GetmapRedeMT(void);
	std::map<int,TSubestacao*> __fastcall GetmapSubestacao(void);
	std::map<int,TTrafoSubestacao*> __fastcall GetmapTrafoSub(void);
	std::map<int,TTipoUnidade*> __fastcall GetmapTipoUnidade(void);
	//Mapas publicos
	std::map<int,TArranjoBDGD*>mapArranjo;
	std::map<int,TBarraBDGD*>mapBarra;
	std::map<int,TBarramento*>mapBarramento;
    std::map<int,TRedeMT*>mapRedeMT;
	std::map<int,TSubestacao*>mapSubestacao;
	std::map<int,TTrafoSubestacao*>mapTrafoSubestacao;

private:

	// objetos externos
	VTApl *apl;
	BDGDDAO *dao;
	VTProgresso *progresso;

	// Listas locais
	TList *lisDOMINIO;
	TList *lisENTIDADES;

	// objetos locais
	std::map<int,TAreaLocalizacao*>mapAreaLoc;

	std::map<int,TBitolaCabo*>mapBitola;
	std::map<int,TCapElo*>mapCapElo;
	std::map<int,TClasseSubclasse*>mapClasseSubclasse;
	std::map<int,TClasseTensao*>mapClasseTensao;
	std::map<int,TConfCircuito*>mapConfCircuito;
	std::map<int,TCorrenteNom*>mapCorrenteNom;
	std::map<int,TFasesConexao*>mapFasesConexao;
	std::map<int,TFormacaoCabo*>mapFormacaoCabo;
	std::map<int,TGeometriaCabo*>mapGeometriaCabo;
	std::map<int,TGrupoTarifario*>mapGrupoTarifario;
	std::map<int,TIsolacaoCabo*>mapIsolacaoCabo;
	std::map<int,TLigacaoTrafo*>mapLigacaoTrafo;
	std::map<int,TMaterialCabo*>mapMaterialCabo;
	std::map<int,TPosse*>mapPosse;
	std::map<int,TTensao*>mapTensao;
	std::map<int,TTipoPotAparente*>mapTipoPotAparente;
	std::map<int,TTipoPotReativa*>mapTipoPotReativa;
	std::map<int,TTipoRegulador*>mapTipoRegulador;
	std::map<int,TTipoTrafo*>mapTipoTrafo;
	std::map<int,TTipoUnidade*>mapTipoUnidade;

};
#endif

// --------------------------------------------------------------------------------------------- eof
