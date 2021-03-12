// --------------------------------------------------------------------------------------------- bof
#ifndef TTipoPotReativaH
#define TTipoPotReativaH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TTipoPotReativa : public TObject
{
public: //Property
		__property int           Id        		    = {read=PD.id_tipo_potencia_reativa,  write=PD.id_tipo_potencia_reativa };
		__property double        Pot_Reativa_kVAr   = {read=PD.potencia_reativa,          write=PD.potencia_reativa    		};
		__property AnsiString    Descricao  		= {read=PD.descricao,       		  write=PD.descricao     		    };
public:
	__fastcall TTipoPotReativa(void);
	__fastcall ~TTipoPotReativa(void);

	protected:
	// dados
	struct
	{
		int id_tipo_potencia_reativa;
		double potencia_reativa;
		AnsiString descricao;
    }PD;
};

// função global
TTipoPotReativa* __fastcall NewObjTipoPotReativa(void);

#endif

// --------------------------------------------------------------------------------------------- eof
