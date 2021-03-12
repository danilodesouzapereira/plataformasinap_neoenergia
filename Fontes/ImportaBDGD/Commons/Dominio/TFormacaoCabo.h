// --------------------------------------------------------------------------------------------- bof
#ifndef TFormacaoCaboH
#define TFormacaoCaboH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TFormacaoCabo : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_formacao_cabo,        write=PD.id_formacao_cabo };
		__property AnsiString    Descricao  = {read=PD.descricao,               write=PD.descricao        };
public:
	__fastcall TFormacaoCabo(void);
	__fastcall ~TFormacaoCabo(void);

	protected:
	// dados
	struct
	{
		int id_formacao_cabo;
		AnsiString descricao;
	}PD;
};

// função global
TFormacaoCabo* __fastcall NewObjFormacaoCabo(void);

#endif

// --------------------------------------------------------------------------------------------- eof
