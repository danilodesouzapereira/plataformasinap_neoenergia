// --------------------------------------------------------------------------------------------- bof
#ifndef TTipoReguladorH
#define TTipoReguladorH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TTipoRegulador : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_tipo_regulador,    write=PD.id_tipo_regulador };
		__property AnsiString    Codigo     = {read=PD.codigo,     			 write=PD.codigo    		};
		__property AnsiString    Descricao  = {read=PD.descricao,      		 write=PD.descricao         };
public:
	__fastcall TTipoRegulador(void);
	__fastcall ~TTipoRegulador(void);

	protected:
	// dados
	struct
	{
		int id_tipo_regulador;
		AnsiString codigo;
		AnsiString descricao;
    }PD;
};

// função global
TTipoRegulador* __fastcall NewObjTipoRegulador(void);

#endif

// --------------------------------------------------------------------------------------------- eof
