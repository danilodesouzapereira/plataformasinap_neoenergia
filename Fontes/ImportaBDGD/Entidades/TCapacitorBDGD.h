// --------------------------------------------------------------------------------------------- bof
#ifndef TCapacitorBDGDH
#define TCapacitorBDGDH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// --------------------------------------------------------------------------------------- CLASS
class TBarraBDGD;
class TFasesConexao;
class TTipoUnidade;
class TTipoPotReativa;
class TAreaLocalizacao;

// -------------------------------------------------------------------------------------------
class TCapacitorBDGD : public TObject
{
public: //Property
		__property int          	  Id            = {read=PD.id_capacitor,   write=PD.id_capacitor };
		__property int        	      Id_Rede       = {read=PD.id_rede,   	   write=PD.id_rede      };
		__property TBarraBDGD*   	  Barra_1       = {read=PD.barra1,   	   write=PD.barra1      };
		__property TBarraBDGD*        Barra_2       = {read=PD.barra2,         write=PD.barra2      };
		__property TFasesConexao*     Fases_Con     = {read=PD.fases_con,      write=PD.fases_con    };
		__property TTipoUnidade*      Tipo_Unidade  = {read=PD.tipo_unidade,   write=PD.tipo_unidade };
		__property TTipoPotReativa*   Tipo_Q     	= {read=PD.tipo_Q,         write=PD.tipo_Q       };
		__property TAreaLocalizacao*  Area_Loc      = {read=PD.area_loc,       write=PD.area_loc     };
		__property AnsiString   	  Codigo        = {read=PD.codigo,         write=PD.codigo       };
		__property bool   			  Banco         = {read=PD.banco,          write=PD.banco        };
		__property AnsiString   	  Descricao     = {read=PD.descricao,      write=PD.descricao    };
public:
	__fastcall TCapacitorBDGD(void);
	__fastcall ~TCapacitorBDGD(void);

	protected:
	// dados
	struct
	{
		int id_capacitor;
		int id_rede;
		TBarraBDGD* barra1;
		TBarraBDGD* barra2;
		TFasesConexao* fases_con;
		TTipoUnidade* tipo_unidade;
		TTipoPotReativa* tipo_Q;
		TAreaLocalizacao* area_loc;
		AnsiString codigo;
		bool banco;
		AnsiString descricao;
	}PD;
};

// função global
TCapacitorBDGD* __fastcall NewObjCapacitorBDGD(void);

#endif

// --------------------------------------------------------------------------------------------- eof
