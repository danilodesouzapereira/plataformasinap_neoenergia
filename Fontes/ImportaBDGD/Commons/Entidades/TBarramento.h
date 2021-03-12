// --------------------------------------------------------------------------------------------- bof
#ifndef TBarramentoH
#define TBarramentoH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
class TTensao;

// ------------------------------------------------------------------------------------------- class
class TBarramento : public TObject
{
public: //Property
		__property int           Id        = {read=PD.id_barramento, write=PD.id_barramento };
		__property int           Id_Sub    = {read=PD.id_subestacao, write=PD.id_subestacao };
		__property int           Id_Barra  = {read=PD.id_barra,      write=PD.id_barra      };
		__property TTensao*      Tensao    = {read=PD.tensao,        write=PD.tensao        };
		__property AnsiString    Codigo    = {read=PD.codigo,        write=PD.codigo        };
		__property AnsiString    Descricao = {read=PD.descricao,     write=PD.descricao     };
public:
	__fastcall TBarramento(void);
	__fastcall ~TBarramento(void);

	protected:
	// dados
	struct
	{
		int id_barramento;
		int id_subestacao;
		int id_barra;
		TTensao* tensao;
		AnsiString codigo;
		AnsiString descricao;
    }PD;
};

// função global
TBarramento* __fastcall NewObjBarramento(void);

#endif

// --------------------------------------------------------------------------------------------- eof
