// -----------------------------------------------------------------------------
#ifndef VTAutoTagH
#define VTAutoTagH

// arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>
//-----------------------------------------------------------------------------
class VTApl;
class VTAcao;
//-----------------------------------------------------------------------------
struct autoTagCFG
{
    bool enabled;
	bool tag_chave_manobra;
};
// -----------------------------------------------------------------------------
class VTAutoTag : public TObject
{
	public: // property
		__property bool Habilitado 		= {read=cfg.enabled, write=cfg.enabled};
		__property bool ChaveManobrada 	= {read=cfg.tag_chave_manobra, write=PM_SetChaveManobra};

		__fastcall VTAutoTag(void){};
		__fastcall ~VTAutoTag(void){};
    virtual bool __fastcall Inicializa(void) = 0;
    virtual bool __fastcall InsereTagAcao(VTAcao *acao, bool undo = false) = 0;



	protected: // dados  acessados via property
		autoTagCFG cfg;

	protected: // metodos  acessados via property
     virtual void __fastcall PM_SetChaveManobra(bool enabled) = 0;

};

// -----------------------------------------------------------------------------
// função global
VTAutoTag* __fastcall NewObjAutoTag(VTApl *apl);

#endif
// ---------------------------------------------------------------------------
// eof
