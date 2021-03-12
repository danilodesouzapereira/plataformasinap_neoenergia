// --------------------------------------------------------------------------------------------- bof
#ifndef TMaterialCaboH
#define TMaterialCaboH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TMaterialCabo : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_material_cabo,        write=PD.id_material_cabo };
		__property AnsiString    Descricao  = {read=PD.descricao,       		write=PD.descricao        };
public:
	__fastcall TMaterialCabo(void);
	__fastcall ~TMaterialCabo(void);

	protected:
	// dados
	struct
	{
		int id_material_cabo;
		AnsiString descricao;
    }PD;
};

// função global
TMaterialCabo* __fastcall NewObjMaterialCabo(void);

#endif

// --------------------------------------------------------------------------------------------- eof
