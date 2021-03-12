// --------------------------------------------------------------------------------------------- bof
#ifndef TAreaLocalizacaoH
#define TAreaLocalizacaoH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TAreaLocalizacao : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_area_localizacao,     write=PD.id_area_localizacao };
		__property AnsiString    Codigo     = {read=PD.codigo,                  write=PD.codigo              };
		__property AnsiString    Descricao  = {read=PD.descricao,               write=PD.descricao           };
public:
	__fastcall TAreaLocalizacao(void);
	__fastcall ~TAreaLocalizacao(void);

	protected:
	// dados
	struct
	{
		int id_area_localizacao;
		AnsiString codigo;
		AnsiString descricao;
	}PD;
};

// função global
TAreaLocalizacao* __fastcall NewObjAreaLocalizacao(void);

#endif

// --------------------------------------------------------------------------------------------- eof
