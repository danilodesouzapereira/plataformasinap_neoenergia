// --------------------------------------------------------------------------------------------- bof
#ifndef TGeradorBDGDH
#define TGeradorBDGDH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
// -------------------------------------------------------------------------------------------
class TBarraBDGD;
class TFasesConexao;
class TTensao;

// ------------------------------------------------------------------------------------------- class
class TGeradorBDGD : public TObject
{
public: //Property
		__property int             Id                    = {read=PD.id_gerador,          write=PD.id_gerador            };
		__property int             Id_Rede               = {read=PD.id_rede,             write=PD.id_rede               };
		__property TBarraBDGD*         Barra                 = {read=PD.barra,               write=PD.barra                 };
		__property TFasesConexao*  Fases_Con             = {read=PD.fases_con,           write=PD.fases_con             };
		__property TTensao*        Tensao                = {read=PD.tensao,              write=PD.tensao                };
		__property AnsiString      Codigo                = {read=PD.codigo,              write=PD.codigo                };
		__property double          P_Inst_kW    	     = {read=PD.pot_instalada,       write=PD.pot_instalada         };
		__property double          P_Contra_Max_kW     	 = {read=PD.pot_contratada_max,  write=PD.pot_contratada_max    };
		__property double          R0    	             = {read=PD.r0,     	         write=PD.r0                    };
		__property double          X0     	             = {read=PD.x0,     	         write=PD.x0                    };
		__property double          R1     	             = {read=PD.r1,     	         write=PD.r1                    };
		__property double          X1     	             = {read=PD.x1,     	         write=PD.x1                    };
		__property AnsiString      Descricao             = {read=PD.descricao,           write=PD.descricao             };
public:
	__fastcall TGeradorBDGD(void);
	__fastcall ~TGeradorBDGD(void);

		protected:
	// dados
	struct
	{
		int id_gerador;
		int id_rede;
		TBarraBDGD* barra;
		TFasesConexao* fases_con;
		TTensao* tensao;
		AnsiString codigo;
		double pot_instalada;   //kw
		double pot_contratada_max;  //kW
		double r0;
		double x0;
		double r1;
		double x1;
		AnsiString descricao;
	}PD;

		   };

// função global
TGeradorBDGD* __fastcall NewObjGeradorBDGD(void);

#endif

// --------------------------------------------------------------------------------------------- eof
