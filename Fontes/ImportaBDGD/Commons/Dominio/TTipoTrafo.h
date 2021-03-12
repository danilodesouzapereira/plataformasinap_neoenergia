// --------------------------------------------------------------------------------------------- bof
#ifndef TTipoTrafoH
#define TTipoTrafoH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TTipoTrafo : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_tipo_trafo,         write=PD.id_tipo_trafo };
		__property AnsiString    Codigo     = {read=PD.codigo,     			  write=PD.codigo   	 };
		__property AnsiString    Descricao  = {read=PD.descricao,       	  write=PD.descricao     };
public:
	__fastcall TTipoTrafo(void);
	__fastcall ~TTipoTrafo(void);

	protected:
	// dados
	struct
	{
		int id_tipo_trafo;
		AnsiString codigo;
		AnsiString descricao;
	}PD;
};

// função global
TTipoTrafo* __fastcall NewObjTipoTrafo(void);

#endif

// --------------------------------------------------------------------------------------------- eof
