// --------------------------------------------------------------------------------------------- bof
#ifndef TClasseSubclasseH
#define TClasseSubclasseH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TClasseSubclasse : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_classe_subclasse,     write=PD.id_classe_subclasse };
		__property AnsiString    Codigo     = {read=PD.codigo,                  write=PD.codigo              };
		__property AnsiString    Descricao  = {read=PD.descricao,      			write=PD.descricao           };
public:
	__fastcall TClasseSubclasse(void);
	__fastcall ~TClasseSubclasse(void);

	protected:
	// dados
	struct
	{
		int id_classe_subclasse;
		AnsiString codigo;
		AnsiString descricao;
    }PD;
};

// função global
TClasseSubclasse* __fastcall NewObjClasseSubclasse(void);

#endif

// --------------------------------------------------------------------------------------------- eof
