// -----------------------------------------------------------------------------
#ifndef VTMultiObraBOH
#define VTMultiObraBOH

// arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>
// -----------------------------------------------------------------------------
class VTApl;
class VTMultiObra;
class VTObra;
// -----------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17073894
class VTMultiObraBO : public TObject
{
	public: // property
		// __property bool 	Configurado		= {read=configurado, write=configurado};
		// __property TList* LisAcao    		= {read=lisAcao};
		// __property TList* LisIpeAtributo 	= {read=lisIpeAtributo};

		__fastcall VTMultiObraBO(void){};
		__fastcall ~VTMultiObraBO(void){};

	virtual VTMultiObra* __fastcall AnalisaAcoesObra(VTObra *obra) = 0;
	virtual void         __fastcall DestroiMultiObra(VTObra *obra)= 0;
	virtual void         __fastcall ReiniciaMultiObra(VTObra *obra)= 0;
	virtual VTMultiObra* __fastcall ExisteMultiObra(VTObra *obra)= 0;
	//com a passagem do ObraCopel e Regiao pro Obra é necessário conferir obras finalizadas
	virtual bool         __fastcall IsObraFinalizada(VTObra *obra) = 0;

	protected: // dados externos
		VTApl *apl;

		// dados internos
		TList *lisMultiObra;

};

// -----------------------------------------------------------------------------
// função global
VTMultiObraBO* __fastcall NewObjMultiObraBO(VTApl *apl_owner);

#endif
// ---------------------------------------------------------------------------
// eof
