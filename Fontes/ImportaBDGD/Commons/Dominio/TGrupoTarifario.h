// --------------------------------------------------------------------------------------------- bof
#ifndef TGrupoTarifarioH
#define TGrupoTarifarioH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TGrupoTarifario : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_grupo_tarifario,      write=PD.id_grupo_tarifario };
		__property AnsiString    Codigo     = {read=PD.codigo,     				write=PD.codigo    			};
		__property AnsiString    Descricao  = {read=PD.descricao,       		write=PD.descricao     	    };
public:
	__fastcall TGrupoTarifario(void);
	__fastcall ~TGrupoTarifario(void);

	protected:
	// dados
	struct
	{
		int id_grupo_tarifario;
		AnsiString codigo;
		AnsiString descricao;
    }PD;
};

// função global
TGrupoTarifario* __fastcall NewObjGrupoTarifario(void);

#endif

// --------------------------------------------------------------------------------------------- eof
