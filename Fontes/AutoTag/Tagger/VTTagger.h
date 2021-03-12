// -----------------------------------------------------------------------------
#ifndef VTTaggerH
#define VTTaggerH

// arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>
//-----------------------------------------------------------------------------
class VTApl;
class VTAcao;
// -----------------------------------------------------------------------------
class VTTagger : public TObject
{
	public: 
		__fastcall VTTagger(void){};
		__fastcall ~VTTagger(void){};

	virtual bool __fastcall  InsereTags(VTAcao *acao, bool undo) = 0;
	virtual void    __fastcall  InicializaComentariosAutomaticos(void) = 0;
};
#endif
// ---------------------------------------------------------------------------
// eof
