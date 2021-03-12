// --------------------------------------------------------------------------------------------- bof
#ifndef TArranjoBDGDH
#define TArranjoBDGDH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

class TGeometriaCabo;
class TFormacaoCabo;
class TBitolaCabo;
class TMaterialCabo;
class TIsolacaoCabo;
// ------------------------------------------------------------------------------------------- class
class TArranjoBDGD : public TObject
{
public: //Property
		__property int             Id                  = {read=PD.id_arranjo,      	     write=PD.id_arranjo           };
		__property AnsiString      Codigo     		   = {read=PD.codigo,                write=PD.codigo               };
		__property TGeometriaCabo* Geometria_Cabo      = {read=PD.geometria_cabo,        write=PD.geometria_cabo       };
		__property TFormacaoCabo*  Formacao_Cabo       = {read=PD.formacao_cabo,         write=PD.formacao_cabo        };
		__property TBitolaCabo*    Bitola_Cabo_1       = {read=PD.bitola_cabo_1,         write=PD.bitola_cabo_1        };
		__property TBitolaCabo*    Bitola_Cabo_2       = {read=PD.bitola_cabo_2,         write=PD.bitola_cabo_2        };
		__property TBitolaCabo*    Bitola_Cabo_3       = {read=PD.bitola_cabo_3,         write=PD.bitola_cabo_3        };
		__property TBitolaCabo*    Bitola_Cabo_N       = {read=PD.bitola_cabo_N,         write=PD.bitola_cabo_N        };
		__property TMaterialCabo*  Material_Cabo_1     = {read=PD.material_cabo_1,       write=PD.material_cabo_1      };
		__property TMaterialCabo*  Material_Cabo_2     = {read=PD.material_cabo_2,       write=PD.material_cabo_2      };
		__property TMaterialCabo*  Material_Cabo_3     = {read=PD.material_cabo_3,       write=PD.material_cabo_3      };
		__property TMaterialCabo*  Material_Cabo_N     = {read=PD.material_cabo_N,       write=PD.material_cabo_N      };
		__property TIsolacaoCabo*  Isolacao_Cabo_1     = {read=PD.isolacao_cabo_1,       write=PD.isolacao_cabo_1      };
		__property TIsolacaoCabo*  Isolacao_Cabo_2     = {read=PD.isolacao_cabo_2,       write=PD.isolacao_cabo_2      };
		__property TIsolacaoCabo*  Isolacao_Cabo_3     = {read=PD.isolacao_cabo_3,       write=PD.isolacao_cabo_3      };
		__property TIsolacaoCabo*  Isolacao_Cabo_N     = {read=PD.isolacao_cabo_N,       write=PD.isolacao_cabo_N      };
		__property int             Num_Conductor_Fase  = {read=PD.num_condutor_fase,     write=PD.num_condutor_fase    };
		__property double          R1     			   = {read=PD.r1,     			   	 write=PD.r1                   };
		__property double          X1     			   = {read=PD.x1,     			   	 write=PD.x1                   };
		__property double          Fator_Conversao     = {read=PD.fator_conversao,     	 write=PD.fator_conversao      };
		__property double          Corrente_Nom_A      = {read=PD.corrente_nom_condutor, write=PD.corrente_nom_condutor};
		__property double          Corrente_Max_A      = {read=PD.corrente_max_condutor, write=PD.corrente_max_condutor};
		__property AnsiString      Descricao           = {read=PD.descricao,             write=PD.descricao            };
public:
				__fastcall TArranjoBDGD(void);
				__fastcall ~TArranjoBDGD(void);
	AnsiString  __fastcall Nome(void);

	protected:
	// dados
	struct
	{
		int id_arranjo;
		AnsiString codigo;
		TGeometriaCabo* geometria_cabo;
		TFormacaoCabo* formacao_cabo;
		TBitolaCabo* bitola_cabo_1;
		TBitolaCabo* bitola_cabo_2;
		TBitolaCabo* bitola_cabo_3;
		TBitolaCabo* bitola_cabo_N;
		TMaterialCabo* material_cabo_1;
		TMaterialCabo* material_cabo_2;
		TMaterialCabo* material_cabo_3;
		TMaterialCabo* material_cabo_N;
		TIsolacaoCabo* isolacao_cabo_1;
		TIsolacaoCabo* isolacao_cabo_2;
		TIsolacaoCabo* isolacao_cabo_3;
		TIsolacaoCabo* isolacao_cabo_N;
		int num_condutor_fase;
		double r1;
		double x1;
		double fator_conversao;
		double corrente_nom_condutor;
		double corrente_max_condutor;
		AnsiString descricao;
	}PD;
};

// função global
TArranjoBDGD* __fastcall NewObjArranjo(void);

#endif

// --------------------------------------------------------------------------------------------- eof
