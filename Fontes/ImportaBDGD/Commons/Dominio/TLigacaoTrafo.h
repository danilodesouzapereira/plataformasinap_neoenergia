// --------------------------------------------------------------------------------------------- bof
#ifndef TLigacaoTrafoH
#define TLigacaoTrafoH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TLigacaoTrafo : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_ligacao_trafo,        write=PD.id_ligacao_trafo };
		__property AnsiString    Descricao  = {read=PD.descricao,       		write=PD.descricao        };
public:
	__fastcall TLigacaoTrafo(void);
	__fastcall ~TLigacaoTrafo(void);

	protected:
	// dados
	struct
	{
		int id_ligacao_trafo;
		AnsiString descricao;
	}PD;
};

// função global
TLigacaoTrafo* __fastcall NewObjLigacaoTrafo(void);

#endif

// --------------------------------------------------------------------------------------------- eof
