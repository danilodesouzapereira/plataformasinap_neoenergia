// ---------------------------------------------------------------------------
#ifndef TRelObraTOH
#define TRelObraTOH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
// ---------------------------------------------------------------------------
class VTAlternativa;
class VTObra;
class VTItemObraCOPEL;
class VTRede;
// ---------------------------------------------------------------------------
class TRelObraTO : public TObject
{
	public:
			__fastcall TRelObraTO(void);
			__fastcall ~TRelObraTO(void);
	void 	__fastcall Reinicia(void);

	public:
		VTAlternativa *alternativa;
		VTObra        *obra;
		VTRede        *subestacao;
		VTRede        *alimentador;
		AnsiString    nomeAlternativa;
		AnsiString    nomeObra;
		AnsiString    data;
		AnsiString    nomeSED;
		AnsiString    nomeAlimentador;
		AnsiString    acao;
		AnsiString    eqpto;
		AnsiString    cod_modulo;
		AnsiString    descricao_modulo;
		AnsiString    complemento;
		AnsiString    custo;
};

#endif
// ---------------------------------------------------------------------------
// eof
