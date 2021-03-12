// --------------------------------------------------------------------------------------------- bof
#ifndef TIsolacaoCaboH
#define TIsolacaoCaboH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TIsolacaoCabo : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_isolacao_cabo,        write=PD.id_isolacao_cabo };
		__property AnsiString    Descricao  = {read=PD.descricao,       		write=PD.descricao        };
public:
	__fastcall TIsolacaoCabo(void);
	__fastcall ~TIsolacaoCabo(void);

	protected:
	// dados
	struct
	{
		int id_isolacao_cabo;
		AnsiString descricao;
	}PD;
};

// função global
TIsolacaoCabo* __fastcall NewObjIsolacaoCabo(void);

#endif

// --------------------------------------------------------------------------------------------- eof
