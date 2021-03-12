// --------------------------------------------------------------------------------------------- bof
#ifndef TPosseH
#define TPosseH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TPosse : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_posse,         write=PD.id_posse  };
		__property AnsiString    Codigo     = {read=PD.codigo,           write=PD.codigo    };
		__property AnsiString    Descricao  = {read=PD.descricao,        write=PD.descricao };
public:
	__fastcall TPosse(void);
	__fastcall ~TPosse(void);

	protected:
	// dados
	struct
	{
		int id_posse;
		AnsiString codigo;
		AnsiString descricao;
    }PD;
};

// função global
TPosse* __fastcall NewObjPosse(void);

#endif

// --------------------------------------------------------------------------------------------- eof
