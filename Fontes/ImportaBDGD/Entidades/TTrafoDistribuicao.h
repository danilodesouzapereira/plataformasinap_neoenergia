// --------------------------------------------------------------------------------------------- bof
#ifndef TTrafoDistribuicaoH
#define TTrafoDistribuicaoH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
class TBarraBDGD;
class TFasesConexao;
class TTensao;
class TTipoUnidade;
class TPosse;
class TCapElo;
class TCorrenteNom;
class TAreaLocalizacao;
class TConfCircuito;
class TTipoTrafo;
class TLigacaoTrafo;


// ------------------------------------------------------------------------------------------- class
class TTrafoDistribuicao : public TObject
{
public: //Property
		__property int               Id                  = {read=PD.id_trafo_distribuicao, 	 write=PD.id_trafo_distribuicao };
		__property int               Id_Rede_MT  		     = {read=PD.id_rede_mt,				 write=PD.id_rede_mt             	};
		__property int               Id_Rede_BT  		     = {read=PD.id_rede_bt,				 write=PD.id_rede_bt             	};
		__property TBarraBDGD*           Barra_1        	 = {read=PD.barra_1,   			     write=PD.barra_1      			};
		__property TBarraBDGD*           Barra_2      		 = {read=PD.barra_2,     			 write=PD.barra_2      			};
		__property TBarraBDGD*           Barra_3       		 = {read=PD.barra_3,      			 write=PD.barra_3     			};
		__property TFasesConexao*    Fases_Con_Pri   	 = {read=PD.fases_conexao_pri,     	 write=PD.fases_conexao_pri     };
		__property TFasesConexao*    Fases_Con_Sec   	 = {read=PD.fases_conexao_sec,     	 write=PD.fases_conexao_sec     };
		__property TFasesConexao*    Fases_Con_Ter    	 = {read=PD.fases_conexao_ter,  	 write=PD.fases_conexao_ter     };
		__property TTensao*          Tensao_Pri       	 = {read=PD.tensao_pri,        		 write=PD.tensao_pri     		};
		__property TTensao*          Tensao_Sec       	 = {read=PD.tensao_sec,    			 write=PD.tensao_sec     		};
		__property TTensao*          Tensao_Ter       	 = {read=PD.tensao_ter,   			 write=PD.tensao_ter     		};
		__property TTipoUnidade*     Tipo_Unidade        = {read=PD.tipo_unidade,  	   		 write=PD.tipo_unidade          };
		__property TPosse*           Posse     			 = {read=PD.posse,  	   			 write=PD.posse                 };
		__property TCapElo*          Elo_Fusivel  	   	 = {read=PD.elo_fusivel,   			 write=PD.elo_fusivel           };
		__property TCorrenteNom*     Corrente_Nom  		 = {read=PD.corr_nom,       		 write=PD.corr_nom              };
		__property TAreaLocalizacao* Area_Loc     		 = {read=PD.area_loc,         		 write=PD.area_loc              };
		__property TConfCircuito*    Config_Circuito     = {read=PD.config_circuito,         write=PD.config_circuito       };
		__property TTipoTrafo*       Tipo_Trafo          = {read=PD.tipo_trafo,       		 write=PD.tipo_trafo            };
		__property TLigacaoTrafo*    Ligacao_Trafo       = {read=PD.ligacao_trafo,       	 write=PD.ligacao_trafo      	};
		__property AnsiString        Codigo              = {read=PD.codigo,       			 write=PD.codigo        		};
		__property int               Atribuicao_Perda    = {read=PD.atribuicao_perda,		 write=PD.atribuicao_perda      };
		__property double            Ten_Lin_Sec_kV      = {read=PD.tensao_lin_sec,     	 write=PD.tensao_lin_sec        };
		__property double            Tap                 = {read=PD.tap,     				 write=PD.tap                   };
		__property double            S_Nom_kVA           = {read=PD.pot_nom_aparente_kva,    write=PD.pot_nom_aparente_kva  };
		__property double            Perda_Fe_W          = {read=PD.perda_ferro_W,           write=PD.perda_ferro_W    		};
		__property double            Perda_Total_W       = {read=PD.perda_total_W,           write=PD.perda_total_W    		};
		__property bool              Banco               = {read=PD.banco,    				 write=PD.banco    			    };
		__property bool              Mono_Retorno_Terra  = {read=PD.mono_retorno_terra,    	 write=PD.mono_retorno_terra    };
		__property double            R    	 		     = {read=PD.r,     	  		         write=PD.r         		    };
		__property double            X_Pri_Sec           = {read=PD.x_pri_sec,     	         write=PD.x_pri_sec             };
		__property double            X_Pri_Ter    	 	 = {read=PD.x_pri_ter,     	  		 write=PD.x_pri_ter             };
		__property double            X_Sec_Ter           = {read=PD.x_sec_ter,     	         write=PD.x_sec_ter             };
		__property AnsiString        Descricao           = {read=PD.descricao,   			 write=PD.descricao     		};
public:
	__fastcall TTrafoDistribuicao(void);
	__fastcall ~TTrafoDistribuicao(void);

	protected:
	// dados
	struct
	{
		int id_trafo_distribuicao;
		int id_rede_mt;
		int id_rede_bt;
		TBarraBDGD* barra_1;
		TBarraBDGD* barra_2;
		TBarraBDGD* barra_3;
		TFasesConexao* fases_conexao_pri;
		TFasesConexao* fases_conexao_sec;
		TFasesConexao* fases_conexao_ter;
		TTensao* tensao_pri;
		TTensao* tensao_sec;
		TTensao* tensao_ter;
		TTipoUnidade* tipo_unidade;
		TPosse* posse;
        TCapElo* elo_fusivel;
		TCorrenteNom* corr_nom;
		TAreaLocalizacao* area_loc;
		TConfCircuito* config_circuito;
		TTipoTrafo* tipo_trafo;
		TLigacaoTrafo* ligacao_trafo;
		AnsiString codigo;
        int atribuicao_perda;
		double tensao_lin_sec;
		double tap;
		double pot_nom_aparente_kva;
		double perda_ferro_W;
		double perda_total_W;
		bool banco;
		bool mono_retorno_terra;
        double r;
		double x_pri_sec;
		double x_pri_ter;
		double x_sec_ter;
		AnsiString descricao;
	}PD;
};

// função global
TTrafoDistribuicao* __fastcall NewObjTrafoDistribuicao(void);

#endif

// --------------------------------------------------------------------------------------------- eof
