//---------------------------------------------------------------------------

#ifndef TTagFormatterCOPELH
#define TTagFormatterCOPELH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\Estrutura.h"
#include "TTagFormatter.h"
//---------------------------------------------------------------------------
//texto auxiliares das tags
#define tf_COPEL_ABRE_CHAVE "ABRIR chave: "
#define tf_COPEL_FECHA_CHAVE "FECHAR chave: "
//---------------------------------------------------------------------------
class VTApl;
class VTEstiloComentario;
//---------------------------------------------------------------------------
class TTagFormatterCOPEL : public TTagFormatter
{
	public:  //métodos
				__fastcall  TTagFormatterCOPEL(VTApl *apl);
				__fastcall ~TTagFormatterCOPEL(void);
		void 	__fastcall  FormataTagAberturaChave(VTFiguraComentario *comentario, VTEqpto *eqpto_associado);
		void 	__fastcall  FormataTagFechamentoChave(VTFiguraComentario *comentario, VTEqpto *eqpto_associado);
//		bool       __fastcall PossuiTextoAutomatico(VTFiguraComentario *comentario);
		AnsiString __fastcall TextoTagAberturaChave(VTEqpto *eqpto_associado);
		AnsiString __fastcall TextoTagFechamentoChave(VTEqpto *eqpto_associado);
	private:
		void    __fastcall FormataEstiloDefaultAbrir(VTEstiloComentario *estiloC);
		void    __fastcall FormataEstiloDefaultFechar(VTEstiloComentario *estiloC);

};
//---------------------------------------------------------------------------
#endif
