//---------------------------------------------------------------------------
#ifndef TMontaArvoreH
#define TMontaArvoreH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTArvore;
class VTBarra;
class VTMRede;
class VTNode;
class VTApl;
class VTBloco;
class VTBlocos;
class VTRedes;

//---------------------------------------------------------------------------
class TMontaArvore
   {
   private: //objetos externos
		VTApl    *apl;
		VTRedes  *redes;
		VTBlocos *blocos;

	private: //dados locais
		VTArvore *arvore;
		TList    *lisBLO_ORD;
//		TList    *lisBLO_VIZ;
		TList    *lisMNET_ORD;
		TList    *lisMNET_ISO;
		TList    *lisTMP, *lisORD;

   public:  //métodos
			  __fastcall  TMontaArvore(VTApl *apl);
			  __fastcall ~TMontaArvore(void);
		bool __fastcall  Executa(VTArvore *arvore, VTBlocos *blocos);
		bool __fastcall  Executa(VTArvore *arvore, VTRedes *redes, bool incluir_rede_isolada);
		bool __fastcall  Executa(VTArvore *arvore, TList *lisMREDE);
		bool __fastcall  Executa(VTArvore *arvore, TList *lisLIG, VTBarra *barra_ini);
		void __fastcall  TreeViewInicia(VTArvore *arvore, TTreeView *TView, bool sort);

	private: //métodos
		void       __fastcall DefineCodigoNode(VTNode *node);
		bool       __fastcall IniciaLisMNet(void);
		void       __fastcall InsereNode(VTNode *node_pai, VTBloco *bloco_pai);
		void       __fastcall Ordena(TList *lisBLO_ORD);
		void       __fastcall OrdenaMRede(VTMRede *mrede, TList *lisORIG);
		TTreeNode* __fastcall TViewInsereNode(TTreeView *TView, VTNode *node, TTreeNode *treenode_pai=NULL);
	};

//---------------------------------------------------------------------------
#endif
//eof
