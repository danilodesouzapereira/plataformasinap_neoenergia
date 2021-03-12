// -----------------------------------------------------------------------------
#ifndef VTTagFormatterH
#define VTTagFormatterH

// arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>
//-----------------------------------------------------------------------------
class VTApl;
class VTAcao;
class VTFiguraComentario;
class VTEqpto;
//texto auxiliares das tags
#define tf_AUTO_TAG_TEXT ""
// -----------------------------------------------------------------------------
class VTTagFormatter : public TObject
{
	public: // property
		__fastcall  VTTagFormatter(void){};
		__fastcall ~VTTagFormatter(void){};
		
	virtual void __fastcall FormataTagAberturaChave(VTFiguraComentario *comentario, VTEqpto *eqpto_associado) = 0;
	virtual void __fastcall FormataTagFechamentoChave(VTFiguraComentario *comentario, VTEqpto *eqpto_associado) = 0;
	virtual bool       __fastcall PossuiTextoAutomatico(VTFiguraComentario *comentario) = 0;
	virtual AnsiString __fastcall TextoTagAberturaChave(VTEqpto *eqpto_associado) = 0;
	virtual AnsiString __fastcall TextoTagFechamentoChave(VTEqpto *eqpto_associado) = 0;
};
// -----------------------------------------------------------------------------
// função global
VTTagFormatter* __fastcall NewObjTagFormatter(VTApl *apl);
VTTagFormatter* __fastcall NewObjTagFormatterCOPEL(VTApl *apl);
#endif
// ---------------------------------------------------------------------------
// eof
