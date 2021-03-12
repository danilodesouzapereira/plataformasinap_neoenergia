// --------------------------------------------------------------------------------------------- bof
#ifndef TFasesConexaoH
#define TFasesConexaoH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TFasesConexao : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_fases_conexao,        write=PD.id_fases_conexao };
		__property AnsiString    Codigo     = {read=PD.codigo,    			    write=PD.codigo   		  };
		__property int           Qtd_Fios   = {read=PD.qtd_fios,         		write=PD.qtd_fios		  };
		__property int           Fases      = {read=PD.fases,        			write=PD.fases		      };
		__property AnsiString    Descricao  = {read=PD.descricao,       		write=PD.descricao        };
public:
	__fastcall TFasesConexao(void);
	__fastcall ~TFasesConexao(void);

	protected:
	// dados
	struct
	{
		int id_fases_conexao;
		AnsiString codigo;
		int qtd_fios;
		int fases;
		AnsiString descricao;
    }PD;
};

// função global
TFasesConexao* __fastcall NewObjFasesConexao(void);

#endif

// --------------------------------------------------------------------------------------------- eof
