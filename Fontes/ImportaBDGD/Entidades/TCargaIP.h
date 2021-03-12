// --------------------------------------------------------------------------------------------- bof
#ifndef TCargaIPH
#define TCargaIPH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// --------------------------------------------------------------------------------------- CLASS
class TBarraBDGD;
class TClasseSubclasse;
class TFasesConexao;
class TTensao;
class TGrupoTarifario;
class TAreaLocalizacao;


// -------------------------------------------------------------------------------------------
class TCargaIP : public TObject
{
public: //Property
		__property int          	  Id               = {read=PD.id_cargaIP,        write=PD.id_cargaIP       };
		__property int        	      Id_Rede_MT       = {read=PD.id_rede_MT,   	 write=PD.id_rede_MT       };
		__property int        	      Id_Rede_BT       = {read=PD.id_rede_BT,   	 write=PD.id_rede_BT       };
		__property TBarraBDGD*   	      Barra            = {read=PD.barra,   	   		 write=PD.barra            };
		__property TClasseSubclasse*  Classe_Subclasse = {read=PD.classe_subclasse,  write=PD.classe_subclasse };
		__property TFasesConexao*     Fases_Con        = {read=PD.fases_con,         write=PD.fases_con        };
		__property TTensao*           Tensao           = {read=PD.tensao,            write=PD.tensao           };
		__property TGrupoTarifario*   Grupo_Tarifario  = {read=PD.grupo_tarif,       write=PD.grupo_tarif      };
		__property TAreaLocalizacao*  Area_Loc         = {read=PD.area_loc,          write=PD.area_loc         };
		__property AnsiString   	  Codigo           = {read=PD.codigo,            write=PD.codigo           };
		__property double   		  Carga_Instal_kW  = {read=PD.carga_inst_kW,     write=PD.carga_inst_kW    };
		__property AnsiString   	  Descricao        = {read=PD.descricao,         write=PD.descricao        };
		__property double             Ene_kWh[int periodo]     = {read=PM_GetEne_kWh,        write=PM_SetEne_kWh       };
public:
	__fastcall TCargaIP(void);
	__fastcall ~TCargaIP(void);

  private: //dados locais
	  float  *ene_ativa_kwh;

	protected:
	// metodos acessados via property
	double          __fastcall PM_GetEne_kWh(int periodo) throw(Exception);
	//
	void            __fastcall PM_SetEne_kWh(int periodo, double valor) throw(Exception);
	// dados
	struct
	{
		int id_cargaIP;
		int id_rede_MT;
		int id_rede_BT;
		TBarraBDGD* barra;
		TClasseSubclasse* classe_subclasse;
		TFasesConexao* fases_con;
		TTensao* tensao;
		TGrupoTarifario* grupo_tarif;
		TAreaLocalizacao* area_loc;
		AnsiString codigo;
		double carga_inst_kW;
		AnsiString descricao;
	}PD;
};

// função global
TCargaIP* __fastcall NewObjCargaIP(void);

#endif

// --------------------------------------------------------------------------------------------- eof
