// --------------------------------------------------------------------------------------------- bof
#ifndef TRedeMTH
#define TRedeMTH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

class TSubestacao;
class TTensao;
class TRedeBT;

// ------------------------------------------------------------------------------------------- class
class TRedeMT : public TObject
{
public: //Property
		__property int           Id             = {read=PD.id_rede_mt,          write=PD.id_rede_mt         };
		__property int           Id_Sub         = {read=PD.id_subestacao,       write=PD.id_subestacao      };
		__property TSubestacao*  Subestacao     = {read=PD.subestacao,          write=PD.subestacao         };
		__property int           Id_Barramento  = {read=PD.id_barramento,       write=PD.id_barramento      };
		__property int           Id_Trafo_Sub   = {read=PD.id_trafo_subestacao, write=PD.id_trafo_subestacao};
		__property int           Id_Barra_Ini   = {read=PD.id_barra_inicial,    write=PD.id_barra_inicial   };
		__property int			 Id_Barra_Trafo = {read=PD.id_barra_trafo,      write=PD.id_barra_trafo     };
		__property bool          Selecionado    = {read=PD.selecionado,         write=PD.selecionado        };
		__property TTensao*      Tensao_Nom     = {read=PD.tensao_nominal,      write=PD.tensao_nominal     };
		__property AnsiString    Codigo         = {read=PD.codigo,              write=PD.codigo             };
		__property AnsiString    Nome    	    = {read=PD.nome,                write=PD.nome               };
		__property double        Tensao_Oper_pu = {read=PD.tensao_operacao,     write=PD.tensao_operacao    };
		__property AnsiString    Descricao      = {read=PD.descricao,           write=PD.descricao          };
public:
	__fastcall TRedeMT(void);
	__fastcall ~TRedeMT(void);
	bool __fastcall InsereSecundario(TRedeBT *secundario);
	TList* __fastcall LisSecundario(void);

	protected:
	// dados
	TList * lisSECUNDARIO;
	// dados
	struct
	{
		int id_rede_mt;
		int id_subestacao;
		TSubestacao *subestacao;
		int id_barramento;
		int id_trafo_subestacao;
		int id_barra_inicial;
		int id_barra_trafo;
		bool selecionado;
		TTensao* tensao_nominal;
		AnsiString codigo;
		AnsiString nome;
		double tensao_operacao;
		AnsiString descricao;
    }PD;
};

// função global
TRedeMT* __fastcall NewObjRedeMT(void);

#endif

// --------------------------------------------------------------------------------------------- eof
