// --------------------------------------------------------------------------------------------- bof
#ifndef TTipoPotAparenteH
#define TTipoPotAparenteH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TTipoPotAparente : public TObject
{
public: //Property
		__property int           Id        		    = {read=PD.id_tipo_pot_aparente,  write=PD.id_tipo_pot_aparente };
		__property double        Pot_Aparente_kVA   = {read=PD.potencia_aparente,     write=PD.potencia_aparente    };
		__property AnsiString    Descricao  		= {read=PD.descricao,             write=PD.descricao            };
public:
	__fastcall TTipoPotAparente(void);
	__fastcall ~TTipoPotAparente(void);

	protected:
	// dados
	struct
	{
		int id_tipo_pot_aparente;
		double potencia_aparente;
		AnsiString descricao;
    }PD;
};

// função global
TTipoPotAparente* __fastcall NewObjTipoPotAparente(void);

#endif

// --------------------------------------------------------------------------------------------- eof
