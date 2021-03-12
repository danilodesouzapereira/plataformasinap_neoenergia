// -----------------------------------------------------------------------------
#ifndef VTMultiObraH
#define VTMultiObraH

// arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>
//-----------------------------------------------------------------------------
class VTApl;
class VTObra;
class VTArvore;
class VTNode;
class VTItemObra;
// -----------------------------------------------------------------------------
class VTMultiObra : public TObject
{
	public: // property
		__property VTArvore*  Arvore 		= {read=arvoreDetalhes};
//		__property TList* 	  LisItemObra 	= {read=lisItemObra};
		__property bool       TodosItemsConfigurados = {read=todos_configurados};
		__property bool       UpToDate = {read=uptodate, write=uptodate};
		// __property TList* LisAcao    		= {read=lisAcao};
		// __property TList* LisIpeAtributo 	= {read=lisIpeAtributo};

		__fastcall VTMultiObra(void){};
		__fastcall ~VTMultiObra(void){};

	virtual bool __fastcall AnalisaObra(VTObra *obra) = 0;
	virtual bool __fastcall AtualizaEstadosNodes(void) = 0;
	virtual bool __fastcall ContraiNode(VTNode *nodeDetalhe) = 0;
	virtual bool __fastcall ExpandeNode(VTNode *nodeDetalhe) = 0;
	virtual bool __fastcall FinalizaObra(void) = 0;
	virtual bool __fastcall HabilitaItemObra(VTItemObra *itemObra, bool habilita) = 0;
	virtual void __fastcall Reinicia(void)=0;

	protected: // dados  acessados via property
		VTArvore *arvoreDetalhes;
//		TList *lisItemObra;
		bool   todos_configurados;
        bool   uptodate;
};

// -----------------------------------------------------------------------------
// função global
VTMultiObra* __fastcall NewObjMultiObra(VTApl *apl);

#endif
// ---------------------------------------------------------------------------
// eof
