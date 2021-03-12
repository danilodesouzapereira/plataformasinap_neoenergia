// --------------------------------------------------------------------------------------------- bof
#ifndef TBitolaCaboH
#define TBitolaCaboH
// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TBitolaCabo : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_bitola_cabo,  write=PD.id_bitola_cabo };
		__property AnsiString    Descricao  = {read=PD.descricao,       write=PD.descricao      };
public:
	__fastcall TBitolaCabo(void);
	__fastcall ~TBitolaCabo(void);

	protected:
	// dados
	struct
	{
		int id_bitola_cabo;
		AnsiString descricao;
	}PD;
};

// função global
TBitolaCabo* __fastcall NewObjBitolaCabo(void);

#endif

// --------------------------------------------------------------------------------------------- eof
