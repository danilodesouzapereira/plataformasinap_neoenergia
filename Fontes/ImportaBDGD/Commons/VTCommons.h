// --------------------------------------------------------------------------------------------- bof
#ifndef VTCommonsH
#define VTCommonsH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
#include <map>

class VTApl;
class VTBDGDDAO;
class BDGDDAO;
class EImportaUgrid;
class TAreaLocalizacao;
class TArranjoBDGD;
class TBarraBDGD;
class TBarramento;
class TBitolaCabo;
class TCapElo;
class TClasseSubclasse;
class TClasseTensao;
class TConfCircuito;
class TCorrenteNom;
class TFasesConexao;
class TFormacaoCabo;
class TGeometriaCabo;
class TGrupoTarifario;
class TIsolacaoCabo;
class TLigacaoTrafo;
class TMaterialCabo;
class TPosse;
class TRedeMT;
class TSubestacao;
class TTensao;
class TTipoPotAparente;
class TTipoPotReativa;
class TTipoRegulador;
class TTipoTrafo;
class TTipoUnidade;
class TTrafoSubestacao;

// ------------------------------------------------------------------------------------------- class
class VTCommons : public TObject
   {
	public:	//property
	  __property bool BatchMode = {read=batch, write=batch};

	public:
									 __fastcall  VTCommons(void) {};
									 __fastcall ~VTCommons(void) {};
	  virtual bool          __fastcall  Clear(void) = 0;
	  virtual void          __fastcall  LeDAO(BDGDDAO* ext_dao) = 0;
	  virtual void          __fastcall  LimpaMapas(void) = 0;
	  virtual bool          __fastcall  ReadTabelasDominio(void) = 0;
	  // Leitura do UGRID
	  virtual bool          __fastcall  ReadAreaLoc(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadArranjo(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadBarra(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadBarramento(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadBitola(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadCapElo(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadClasseSubclasse(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadClasseTensao(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadConfCircuito(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadCorrenteNom(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadEntidadesEmpresa(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadFasesConexao(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadFormacaoCabo(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadGeometriaCabo(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadGrupoTarifario(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadIsolacaoCabo(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadLigacaoTrafo(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadMaterialCabo(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadPosse(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadRedeMT(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadSubestacao(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadTensao(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadTipoPotAparente(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadTipoPotReativa(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadTipoRegulador(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadTipoTrafo(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadTipoUnidade(void) throw(EImportaUgrid)= 0;
	  virtual bool          __fastcall  ReadTrafoSub(void) throw(EImportaUgrid)= 0;
	  // Return dos maps
	  virtual TAreaLocalizacao* __fastcall GetAreaLoc(int id_area)= 0;
	  virtual TArranjoBDGD* __fastcall GetArranjo(int id_arranjo) = 0;
	  virtual TBarraBDGD* __fastcall GetBarra(int id_barra)= 0;
	  virtual TBarramento* __fastcall GetBarramento(int id_barramento)= 0;
	  virtual TBitolaCabo* __fastcall GetBitolaCabo(int id_bitola)= 0;
	  virtual TCapElo* __fastcall GetCapElo(int id_capElo)= 0;
	  virtual TClasseSubclasse* __fastcall GetClasseSubclasse(int id_cl_subcl)= 0;
	  virtual TClasseTensao* __fastcall GetClasseTensao(int id_cl_tensao)= 0;
	  virtual TConfCircuito* __fastcall GetConfCircuito(int id_conf_circuito)= 0;
	  virtual TCorrenteNom* __fastcall GetCorrenteNom(int id_corrente_nom)= 0;
	  virtual TFasesConexao* __fastcall GetFasesConexao(int id_fases_con)= 0;
	  virtual TFormacaoCabo* __fastcall GetFormacaoCabo(int id_formacao_cabo)= 0;
	  virtual TGeometriaCabo* __fastcall GetGeometriaCabo(int id_geom_cabo)= 0;
	  virtual TGrupoTarifario* __fastcall GetGrupoTarifario(int id_grupo_tarif)= 0;
	  virtual TIsolacaoCabo* __fastcall GetIsolacaoCabo(int id_isolacao_cabo)= 0;
	  virtual TLigacaoTrafo* __fastcall GetLigacaoTrafo(int id_ligacao_trafo)= 0;
	  virtual TMaterialCabo* __fastcall GetMaterialCabo(int id_material_cabo)= 0;
	  virtual TPosse* __fastcall GetPosse(int id_posse)= 0;
	  virtual TRedeMT* __fastcall GetRedeMT(int id_rede_mt)= 0;
	  virtual TSubestacao* __fastcall GetSubestacao(int id_subestacao)=0;
	  virtual TTensao* __fastcall GetTensao(int id_tensao)= 0;
	  virtual TTipoPotAparente* __fastcall GetTipoPotAparente(int id_tipoS)= 0;
	  virtual TTipoPotReativa* __fastcall GetTipoPotReativa(int id_tipoQ)= 0;
	  virtual TTipoRegulador* __fastcall GetTipoRegulador(int id_tipo_regulador)= 0;
	  virtual TTipoTrafo* __fastcall GetTipoTrafo(int id_tipo_trafo)= 0;
	  virtual TTipoUnidade* __fastcall GetTipoUnidade(int id_tipo_unidade)= 0;
	  virtual TTrafoSubestacao* __fastcall GetTrafoSubestacao(int id_trafo_subestacao)=0;
		  // Return dos mapas inteiros
	  virtual std::map<int,TArranjoBDGD*> __fastcall GetmapArranjo(void) =0;
	  virtual std::map<int,TBarraBDGD*> __fastcall GetmapBarra(void)=0;
	  virtual std::map<int,TBarramento*> __fastcall GetmapBarramento(void) =0;
	  virtual std::map<int,TRedeMT*> __fastcall GetmapRedeMT(void) =0;
	  virtual std::map<int,TSubestacao*> __fastcall GetmapSubestacao(void) =0;
	  virtual std::map<int,TTipoUnidade*> __fastcall GetmapTipoUnidade(void) =0;
	  virtual std::map<int,TTrafoSubestacao*> __fastcall GetmapTrafoSub(void) =0;


	protected: // dados - property
	  bool batch;

	protected: //métodos acessados via property
	  virtual bool __fastcall PM_GetBatch(void) = 0;
	  virtual void __fastcall PM_SetBatch(bool batch) = 0;
	};

//-----------------------------------------------------------------------------
// função global para criar objeto VTCommons
//-----------------------------------------------------------------------------
VTCommons* NewObjCommons(VTApl *apl, VTBDGDDAO *dao);
#endif
//-----------------------------------------------------------------------------
// eof
