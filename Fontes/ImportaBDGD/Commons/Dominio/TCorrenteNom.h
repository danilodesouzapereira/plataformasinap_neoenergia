// --------------------------------------------------------------------------------------------- bof
#ifndef TCorrenteNomH
#define TCorrenteNomH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TCorrenteNom : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_corrente_nominal,     write=PD.id_corrente_nominal };
		__property double        Corrente_A = {read=PD.corrente,     			write=PD.corrente   		 };
		__property AnsiString    Descricao  = {read=PD.descricao,       		write=PD.descricao    	     };
public:
	__fastcall TCorrenteNom(void);
	__fastcall ~TCorrenteNom(void);

	protected:
	// dados
	struct
	{
		int id_corrente_nominal;
		double corrente;
		AnsiString descricao;
	}PD;
};

// função global
TCorrenteNom* __fastcall NewObjCorrenteNom(void);

#endif

// --------------------------------------------------------------------------------------------- eof
