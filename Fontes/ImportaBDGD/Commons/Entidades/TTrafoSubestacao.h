// --------------------------------------------------------------------------------------------- bof
#ifndef TTrafoSubestacaoH
#define TTrafoSubestacaoH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
class TFasesConexao;
class TTipoTrafo;
class TClasseTensao;
class TTipoPotAparente;
class TLigacaoTrafo;
class TTensao;

// ------------------------------------------------------------------------------------------- class
class TTrafoSubestacao : public TObject
{
public: //Property
		__property int               Id                = {read=PD.id_trafo_subestacao, 		     write=PD.id_trafo_subestacao 		  };
		__property int               Id_Sub            = {read=PD.id_subestacao, 		         write=PD.id_subestacao 		      };
		__property int               Id_Barramento_1   = {read=PD.id_barramento_1,				 write=PD.id_barramento_1             };
		__property int               Id_Barramento_2   = {read=PD.id_barramento_2,				 write=PD.id_barramento_2 			  };
		__property int               Id_Barramento_3   = {read=PD.id_barramento_3,				 write=PD.id_barramento_3 			  };
		__property int               Id_Barra_1        = {read=PD.id_barra_1,   			     write=PD.id_barra_1      			  };
		__property int               Id_Barra_2        = {read=PD.id_barra_2,     			     write=PD.id_barra_2      			  };
		__property int               Id_Barra_3        = {read=PD.id_barra_3,      				 write=PD.id_barra_3     			  };
		__property TFasesConexao*    Fases_Con_Pri     = {read=PD.fases_conexao_pri,     	     write=PD.fases_conexao_pri       	  };
		__property TFasesConexao*    Fases_Con_Sec     = {read=PD.fases_conexao_sec,     	     write=PD.fases_conexao_sec       	  };
		__property TFasesConexao*    Fases_Con_Ter     = {read=PD.fases_conexao_ter,  		     write=PD.fases_conexao_ter       	  };
		__property TTipoTrafo*       Tipo_Trafo        = {read=PD.tipo_trafo,       			 write=PD.tipo_trafo        		  };
		__property TClasseTensao*    Classe_Tensao     = {read=PD.classe_tensao,       			 write=PD.classe_tensao       		  };
		__property TTipoPotAparente* Tipo_Pot_Aparente = {read=PD.tipo_potencia_nominal,         write=PD.tipo_potencia_nominal       };
		__property TLigacaoTrafo*    Ligacao_Trafo     = {read=PD.ligacao_trafo,       			 write=PD.ligacao_trafo      		  };
		__property TTensao*          Tensao_Pri        = {read=PD.tensao_pri,        			 write=PD.tensao_pri     			  };
		__property TTensao*          Tensao_Sec        = {read=PD.tensao_sec,    			     write=PD.tensao_sec     			  };
		__property TTensao*          Tensao_Ter        = {read=PD.tensao_ter,   			     write=PD.tensao_ter     			  };
		__property AnsiString        Codigo            = {read=PD.codigo,       				 write=PD.codigo        			  };
		__property double            S_Nom             = {read=PD.potencia_nominal_aparente,     write=PD.potencia_nominal_aparente   };
		__property double            S_VentFor_1_MVA   = {read=PD.potencia_nominal_vent_forc_1,  write=PD.potencia_nominal_vent_forc_1};
		__property double            S_VentFor_2_MVA   = {read=PD.potencia_nominal_vent_forc_2,  write=PD.potencia_nominal_vent_forc_2};
		__property double            Perda_Fe_Perc     = {read=PD.perda_ferro_per,               write=PD.perda_ferro_per    		  };
		__property double            Perda_Total_Perc  = {read=PD.perda_total_per,               write=PD.perda_total_per    		  };
		__property AnsiString        Descricao         = {read=PD.descricao,   				     write=PD.descricao     			  };
public:
	__fastcall TTrafoSubestacao(void);
	__fastcall ~TTrafoSubestacao(void);

	protected:
	// dados
	struct
	{
		int id_trafo_subestacao;
		int id_subestacao;
		int id_barramento_1;
		int id_barramento_2;
		int id_barramento_3;
		int id_barra_1;
		int id_barra_2;
		int id_barra_3;
		TFasesConexao* fases_conexao_pri;
		TFasesConexao* fases_conexao_sec;
		TFasesConexao* fases_conexao_ter;
		TTipoTrafo* tipo_trafo;
		TClasseTensao* classe_tensao;
		TTipoPotAparente* tipo_potencia_nominal;
		TLigacaoTrafo* ligacao_trafo;
		TTensao* tensao_pri;
		TTensao* tensao_sec;
		TTensao* tensao_ter;
		AnsiString codigo; //
		double potencia_nominal_aparente;    // acho que esse é ID DA POTENCIA  - campo já está
		double potencia_nominal_vent_forc_1;
		double potencia_nominal_vent_forc_2;
		double perda_ferro_per;
		double perda_total_per;
		AnsiString descricao;
	}PD;
};

// função global
TTrafoSubestacao* __fastcall NewObjTrafoSubestacao(void);

#endif

// --------------------------------------------------------------------------------------------- eof
