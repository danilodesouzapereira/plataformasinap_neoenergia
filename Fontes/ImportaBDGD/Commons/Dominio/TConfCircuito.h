// --------------------------------------------------------------------------------------------- bof
#ifndef TConfCircuitoH
#define TConfCircuitoH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TConfCircuito : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_configuracao_circuito,   write=PD.id_configuracao_circuito };
		__property AnsiString    Codigo     = {read=PD.codigo,     				   write=PD.codigo    				 };
		__property AnsiString    Descricao  = {read=PD.descricao,      			   write=PD.descricao   		     };
public:
	__fastcall TConfCircuito(void);
	__fastcall ~TConfCircuito(void);

	protected:
	// dados
	struct
	{
		int id_configuracao_circuito;
		AnsiString codigo;
		AnsiString descricao;
    }PD;
};

// função global
TConfCircuito* __fastcall NewObjConfCircuito(void);

#endif

// --------------------------------------------------------------------------------------------- eof
