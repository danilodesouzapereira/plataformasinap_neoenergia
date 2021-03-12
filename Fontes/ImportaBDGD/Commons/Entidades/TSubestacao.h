// --------------------------------------------------------------------------------------------- bof
#ifndef TSubestacaoH
#define TSubestacaoH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class VTRede;
class TRedeMT;
// -------------------------------------------------------------------------------------------
class TSubestacao : public TObject
{
public: //Property
		__property int           Id         = {read=PD.id_subestacao,   write=PD.id_subestacao};
		__property AnsiString    Codigo     = {read=PD.codigo,          write=PD.codigo       };
		__property AnsiString    Nome       = {read=PD.nome,            write=PD.nome         };
		__property AnsiString    Descricao  = {read=PD.descricao,       write=PD.descricao    };
		__property VTRede*       Rede       = {read=PD.rede,            write=PD.rede    };

public:
	__fastcall TSubestacao(void);
	__fastcall ~TSubestacao(void);
	TRedeMT* __fastcall ExistePrimarioCodigo(AnsiString codigo);
	TRedeMT* __fastcall ExistePrimarioNome(AnsiString nome);
	bool __fastcall InserePrimario(TRedeMT *primario);
	TList* __fastcall LisPrimario(void);
	void   __fastcall SelecionaPrimarioCodigo(AnsiString codigo);
	void   __fastcall SelecionaPrimarioNome(AnsiString nome);
	void   __fastcall TodosPrimariosSelecionado(bool selecionado);
	protected:
	// dados
    TList * lisPRIMARIO;
	struct
	{
		int id_subestacao;
		AnsiString codigo;
		AnsiString nome;
		AnsiString descricao;
		VTRede *rede;
	}PD;
};

// função global
TSubestacao* __fastcall NewObjSubestacao(void);

#endif

// --------------------------------------------------------------------------------------------- eof
