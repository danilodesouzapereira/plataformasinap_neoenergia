// ---------------------------------------------------------------------------
#ifndef TRelObraH
#define TRelObraH
// ---------------------------------------------------------------------------
#include <map>
#include <Classes.hpp>
// ---------------------------------------------------------------------------
class VTArvore;
class VTAlternativa;
class VTApl;
class VTBarra;
class VTEstudo;
class VTItemObraCOPEL;
class VTObra;
class VTRede;
class TRelObraTO;
class VTEqpto;

// ---------------------------------------------------------------------------
class TRelObra : public TObject
{
	public:
					__fastcall TRelObra(VTEstudo *estudo, VTApl* apl_owner);
					__fastcall ~TRelObra(void);
		// void 	__fastcall Reinicia(void);
		void 		__fastcall IniciaAlternativas(void);
		TRelObraTO* __fastcall PreencheRelObraTO(VTAlternativa *alt, VTObra *obra,
			VTItemObraCOPEL *item);

	private:
        bool        __fastcall AlimentadorPai(VTRede *rede_sub, VTRede *rede_filha);
		AnsiString 	__fastcall DeterminaAcao(VTItemObraCOPEL *item);
		AnsiString 	__fastcall DeterminaCodigoPadrao(VTItemObraCOPEL *item);
		AnsiString 	__fastcall DeterminaDescricaoPadrao(VTItemObraCOPEL *item);
		VTEqpto* 	__fastcall DeterminaEqpto(VTItemObraCOPEL *item);
		AnsiString 	__fastcall DeterminaCusto(VTItemObraCOPEL *item);
		VTRede* 	__fastcall DeterminaRede(VTEqpto *eqpto);
		VTRede* 	__fastcall DeterminaRedeDaBarra(VTBarra *barra);
		VTRede* 	__fastcall DeterminaRedeDoEqpto(VTEqpto *eqpto);
		VTRede* 	__fastcall DeterminaSubestacao(VTRede* alimentador);
		AnsiString 	__fastcall FormataValorMonetario(double valor);

	private:
		// objetos externos
		VTEstudo *estudo;
		VTAlternativa *alternativa;
		VTApl *apl;

	private:  // objetos internos
		TList *lisRelObraTO;
		VTArvore *arvore_redes;
        TStringList *campos;
		std::map<VTItemObraCOPEL*, TRelObraTO*>mapRelObra;
};

#endif
// ---------------------------------------------------------------------------
// eof
