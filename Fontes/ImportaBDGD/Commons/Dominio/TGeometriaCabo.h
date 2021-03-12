// --------------------------------------------------------------------------------------------- bof
#ifndef TGeometriaCaboH
#define TGeometriaCaboH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TGeometriaCabo : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_geometria_cabo,       write=PD.id_geometria_cabo };
		__property AnsiString    Descricao  = {read=PD.descricao,       		write=PD.descricao         };
public:
	__fastcall TGeometriaCabo(void);
	__fastcall ~TGeometriaCabo(void);

	protected:
	// dados
	struct
	{
		int id_geometria_cabo;
		AnsiString descricao;
	}PD;
};

// função global
TGeometriaCabo* __fastcall NewObjGeometriaCabo(void);

#endif

// --------------------------------------------------------------------------------------------- eof
