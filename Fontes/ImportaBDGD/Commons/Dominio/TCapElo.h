// --------------------------------------------------------------------------------------------- bof
#ifndef TCapEloH
#define TCapEloH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TCapElo : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_capacidade_elo_fusivel, write=PD.id_capacidade_elo_fusivel };
		__property AnsiString    Codigo     = {read=PD.codigo,                    write=PD.codigo                    };
		__property AnsiString    Descricao  = {read=PD.descricao,       		  write=PD.descricao                 };
public:
	__fastcall TCapElo(void);
	__fastcall ~TCapElo(void);

	protected:
	// dados
	struct
	{
		int id_capacidade_elo_fusivel;
		AnsiString codigo;
		AnsiString descricao;
	}PD;
};

// função global
TCapElo* __fastcall NewObjCapElo(void);

#endif

// --------------------------------------------------------------------------------------------- eof
