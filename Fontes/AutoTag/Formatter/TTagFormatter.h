//---------------------------------------------------------------------------

#ifndef TTagFormatterH
#define TTagFormatterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\Estrutura.h"
#include "VTTagFormatter.h"
//---------------------------------------------------------------------------
class VTApl;
//---------------------------------------------------------------------------
class TTagFormatter : public VTTagFormatter
{
	public:  //métodos
				__fastcall  TTagFormatter(VTApl *apl);
				__fastcall ~TTagFormatter(void);
	virtual	void 	__fastcall  FormataTagAberturaChave(VTFiguraComentario *comentario, VTEqpto *eqpto_associado);
	virtual	void 	__fastcall  FormataTagFechamentoChave(VTFiguraComentario *comentario, VTEqpto *eqpto_associado);
			bool       __fastcall PossuiTextoAutomatico(VTFiguraComentario *comentario);
	virtual AnsiString __fastcall TextoTagAberturaChave(VTEqpto *eqpto_associado);
	virtual AnsiString __fastcall TextoTagFechamentoChave(VTEqpto *eqpto_associado);

	protected:
		bool __fastcall DefineCoordenadas(VTFiguraComentario *comentario, VTEqpto *eqpto_associado);
		bool __fastcall GetCentro(VTEqpto *eqpto, strUTM &utm, strCOORD &esq);
		bool __fastcall GetCoordsComentario(VTEqpto *eqpto, strUTM &utm, strCOORD &esq);


	protected:  //dados externos
		VTApl *apl_;

};
//---------------------------------------------------------------------------
#endif
