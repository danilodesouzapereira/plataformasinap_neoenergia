// --------------------------------------------------------------------------------------------- bof
#ifndef TReguladorMTH
#define TReguladorMTH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// --------------------------------------------------------------------------------------- CLASS
class TBarraBDGD;
class TFasesConexao;
class TTipoUnidade;
class TTipoRegulador;
class TTipoPotAparente;
class TCorrenteNom;


// -------------------------------------------------------------------------------------------
class TReguladorMT : public TObject
{
public: //Property
		__property int          	  Id               = {read=PD.id_regulador_MT,     write=PD.id_regulador_MT   };
		__property int        	      Id_Rede          = {read=PD.id_rede,   	   	   write=PD.id_rede           };
		__property TBarraBDGD*   	      Barra_1          = {read=PD.barra1,   	  	   write=PD.barra1            };
		__property TBarraBDGD*            Barra_2          = {read=PD.barra2,        	   write=PD.barra2            };
		__property TFasesConexao*     Fases_Con        = {read=PD.fases_con,     	   write=PD.fases_con         };
		__property TTipoUnidade*      Tipo_Unidade     = {read=PD.tipo_unidade,  	   write=PD.tipo_unidade      };
		__property TTipoRegulador*    Tipo_Regulador   = {read=PD.tipo_regulador,      write=PD.tipo_regulador    };
		__property TTipoPotAparente*  Tipo_S_Nom       = {read=PD.tipo_S,         	   write=PD.tipo_S      	  };
		__property TCorrenteNom*      Corrente_Nom 	   = {read=PD.corr_nom,       	   write=PD.corr_nom     	  };
		__property AnsiString   	  Codigo           = {read=PD.codigo,         	   write=PD.codigo            };
		__property bool   			  Banco            = {read=PD.banco,         	   write=PD.banco             };
		__property int          	  Qtd_Unid_Mono    = {read=PD.qtd_unidades_mono,   write=PD.qtd_unidades_mono };
		__property double             Pot_Nom    	   = {read=PD.pot_nominal,     	   write=PD.pot_nominal       };
		__property double             Tensao_Regulacao = {read=PD.tensao_regulacao,    write=PD.tensao_regulacao  };
		__property double             R    	 		   = {read=PD.r,     	  		   write=PD.r         		  };
		__property double             X_Pri_Sec        = {read=PD.x_pri_sec,     	   write=PD.x_pri_sec         };
		__property double             Perda_Fe     	   = {read=PD.perda_fe,     	   write=PD.perda_fe          };
		__property double             Perda_Total      = {read=PD.perda_total,     	   write=PD.perda_total       };
		__property int          	  RTP              = {read=PD.rtp,                 write=PD.rtp               };
		__property AnsiString   	  Descricao        = {read=PD.descricao,           write=PD.descricao         };
public:
	__fastcall TReguladorMT(void);
	__fastcall ~TReguladorMT(void);

	protected:
	// dados
	struct
	{
		int id_regulador_MT;
		int id_rede;
		TBarraBDGD* barra1;
		TBarraBDGD* barra2;
		TFasesConexao* fases_con;
		TTipoUnidade* tipo_unidade;
		TTipoRegulador* tipo_regulador ;
		TTipoPotAparente* tipo_S;
		TCorrenteNom* corr_nom;
		AnsiString codigo;
		bool banco;
		int qtd_unidades_mono;
		double pot_nominal;
		double tensao_regulacao;
		double r;
		double x_pri_sec;
		double perda_fe;
		double perda_total;
		int rtp;
		AnsiString descricao;
	}PD;
};

// função global
TReguladorMT* __fastcall NewObjReguladorMT(void);

#endif

// --------------------------------------------------------------------------------------------- eof
