// --------------------------------------------------------------------------------------------- bof
#ifndef TClasseTensaoH
#define TClasseTensaoH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TClasseTensao : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_classe_tensao,    write=PD.id_classe_tensao };
		__property int           Tensao_V   = {read=PD.tensao,     			write=PD.tensao   		  };
		__property AnsiString    Descricao  = {read=PD.descricao,       	write=PD.descricao     	  };
public:
	__fastcall TClasseTensao(void);
	__fastcall ~TClasseTensao(void);

	protected:
	// dados
	struct
	{
		int id_classe_tensao;
		int tensao;
		AnsiString descricao;
    }PD;
};

// função global
TClasseTensao* __fastcall NewObjClasseTensao(void);

#endif

// --------------------------------------------------------------------------------------------- eof
