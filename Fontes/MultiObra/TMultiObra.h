//---------------------------------------------------------------------------
#ifndef TMultiObraH
#define TMultiObraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMultiObra.h"
//---------------------------------------------------------------------------
class VTNode;
//---------------------------------------------------------------------------
class TMultiObra : public VTMultiObra
{
	public:  //métodos
		__fastcall  TMultiObra(VTApl *apl);
		__fastcall ~TMultiObra(void);

	bool __fastcall AnalisaObra(VTObra *obra);
	bool __fastcall AtualizaEstadosNodes(void);
	bool __fastcall ContraiNode(VTNode *nodeDetalhe);
	bool __fastcall ExpandeNode(VTNode *nodeDetalhe);
	bool __fastcall HabilitaItemObra(VTItemObra *itemObra, bool habilita);
	bool __fastcall FinalizaObra(void);
    void __fastcall Reinicia(void);

	private:  //metodos
		bool __fastcall AtualizaEstadoNode(VTNode *nodeDetalhe);
		bool __fastcall AssociaItemObraAoNodesDet(void);
		bool __fastcall AssociaItemObraAoNodesDet(VTNode *nodeDetalhe);
		bool __fastcall CriaItemObraFaltantes(void);
		bool __fastcall CriaItemObraFaltantes(VTNode *nodeDetalhe);
		void __fastcall DestroiItemObra(VTNode *node);
		void __fastcall DestroiItemObraJusante(VTNode *node);
		void __fastcall DestroiItemObraMontante(VTNode *node);
        void __fastcall InicializaItemObra(void);
		bool __fastcall InsereItemObra(VTNode *node);
		bool __fastcall InsereItemObra(VTNode *node, VTItemObra *itemObra);
		bool __fastcall MontaArvore(VTObra *obra);
		bool __fastcall MesmasAcoes(VTNode *nodeDet, VTItemObra *itemObra);

		void __fastcall RemoveEDestroiAdeB(TList *lisItemA, TList *lisItemB);

	private: //dados proprios
		TList *listItemObraAux;

	private:  //dados externos
		VTApl *apl;
        VTObra *obra_ref;

};

//---------------------------------------------------------------------------
#endif
//eof
