// ---------------------------------------------------------------------------
#ifndef TExtraiCamposH
#define TExtraiCamposH

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

// ---------------------------------------------------------------------------
class TExtraiCampos
{
public:
	__fastcall TExtraiCampos(void);
	__fastcall ~TExtraiCampos(void);
	TStrings* __fastcall Executa(AnsiString txt);
	void __fastcall SeparadorInsere(char car);
	void __fastcall SeparadorInsere(AnsiString separadores);
	void __fastcall SeparadorReinicia(void);
	void __fastcall SetOpcaoFiltraComentario(bool enabled);
	void __fastcall SetOpcaoTrimCampos(bool enabled);

private: // métodos
	bool __fastcall IsSeparador(char car);

	// dados locais
	bool filtra_comentario;
	TStrings *campos;

	struct
	{
		bool filtra_comentario;
		bool trim_campo;
		AnsiString separadores;
	}

	opcao;
};

// ---------------------------------------------------------------------------
#endif
// eof
