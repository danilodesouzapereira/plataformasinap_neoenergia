// --------------------------------------------------------------------------------------------- bof
#ifndef TTensaoH
#define TTensaoH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TTensao : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_tensao,         write=PD.id_tensao };
		__property double        Tensao_V   = {read=PD.tensao,    		  write=PD.tensao    };
		__property AnsiString    Descricao  = {read=PD.descricao,      	  write=PD.descricao };
public:
	__fastcall TTensao(void);
	__fastcall ~TTensao(void);

	protected:
	// dados
	struct
	{
		int id_tensao;
		double tensao;
		AnsiString descricao;
    }PD;
};

// função global
TTensao* __fastcall NewObjTensao(void);

#endif

// --------------------------------------------------------------------------------------------- eof
