// --------------------------------------------------------------------------------------------- bof
#ifndef TTipoUnidadeH
#define TTipoUnidadeH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TTipoUnidade : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_tipo_unidade,         write=PD.id_tipo_unidade };
		__property AnsiString    Unidade     = {read=PD.unidade,     			write=PD.unidade         };
		__property AnsiString    Descricao  = {read=PD.descricao,     		    write=PD.descricao       };
public:
	__fastcall TTipoUnidade(void);
	__fastcall ~TTipoUnidade(void);

	protected:
	// dados
	struct
	{
		int id_tipo_unidade;
		AnsiString unidade;
		AnsiString descricao;
    }PD;
};

// função global
TTipoUnidade* __fastcall NewObjTipoUnidade(void);

#endif

// --------------------------------------------------------------------------------------------- eof
