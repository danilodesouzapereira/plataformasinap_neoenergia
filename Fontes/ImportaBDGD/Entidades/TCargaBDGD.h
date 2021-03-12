// --------------------------------------------------------------------------------------------- bof
#ifndef TCargaBDGDH
#define TCargaBDGDH

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
class TCargaBDGD : public TObject
{
public: //Property
		__property int          	  Id                       = {read=PD.id_carga,          write=PD.id_carga         };
		__property int        	      Id_Rede                  = {read=PD.id_rede,   	     write=PD.id_rede          };
		__property TBarraBDGD*   	  Barra                    = {read=PD.barra,   	   		 write=PD.barra            };
		__property TClasseSubclasse*  Classe_Subclasse         = {read=PD.classe_subclasse,  write=PD.classe_subclasse };
		__property TFasesConexao*     Fases_Con                = {read=PD.fases_con,         write=PD.fases_con        };
		__property TTensao*           Tensao                   = {read=PD.tensao,            write=PD.tensao           };
		__property TGrupoTarifario*   Grupo_Tarifario          = {read=PD.grupo_tarif,       write=PD.grupo_tarif      };
		__property TAreaLocalizacao*  Area_Loc                 = {read=PD.area_loc,          write=PD.area_loc         };
		__property AnsiString   	  Codigo                   = {read=PD.codigo,            write=PD.codigo           };
		__property double   		  Carga_Instal_kW          = {read=PD.carga_inst_kW,     write=PD.carga_inst_kW    };
		__property AnsiString   	  Descricao                = {read=PD.descricao,         write=PD.descricao        };
		__property double             Dem_Max_kW[int periodo]  = {read=PM_GetDem_Max_kW,     write=PM_SetDem_Max_kW    };
		__property double             Ene_kWh[int periodo]     = {read=PM_GetEne_kWh,        write=PM_SetEne_kWh       };
		__property int                Resultado_Inspecao       = {read=PD.id_inspecao,       write=PD.id_inspecao      };
public:
	__fastcall TCargaBDGD(void);
	__fastcall ~TCargaBDGD(void);

  private: //dados locais
	  float  *dem_ativa_max_kw;
	  float  *ene_ativa_kwh;

	protected:
	// metodos acessados via property
	double          __fastcall PM_GetDem_Max_kW(int periodo);
	double          __fastcall PM_GetEne_kWh(int periodo);
	//
	void            __fastcall PM_SetDem_Max_kW(int periodo, double valor);
	void            __fastcall PM_SetEne_kWh(int periodo, double valor);
	// dados
	struct
	{
		int id_carga;
		int id_rede;
		TBarraBDGD* barra;
		TClasseSubclasse* classe_subclasse;
		TFasesConexao* fases_con;
		TTensao* tensao;
		TGrupoTarifario* grupo_tarif;
		TAreaLocalizacao* area_loc;
		AnsiString codigo;
		double carga_inst_kW;
		AnsiString descricao;
		int id_inspecao;
	}PD;
};

// função global
TCargaBDGD* __fastcall NewObjCargaBDGD(void);

#endif

// --------------------------------------------------------------------------------------------- eof
