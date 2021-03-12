//---------------------------------------------------------------------------
#ifndef TArvoreH
#define TArvoreH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTArvore.h"

//---------------------------------------------------------------------------
class TArvore : public VTArvore
	{
   private: //objetos externos
      VTApl   *apl;

   private: //dados locais
      TList  *lisNODE;

   public:  //métodos
              __fastcall  TArvore(VTApl *apl);
              __fastcall ~TArvore(void);
      void    __fastcall  Clear(void);
      void    __fastcall  ClearObjects(void);
      void    __fastcall  DefineNodeId(void);
      VTNode* __fastcall  ExisteNode(AnsiString codigo);
      VTNode* __fastcall  ExisteNode(int node_id);
	  VTNode* __fastcall  ExisteNode(TObject *object);
	  bool    __fastcall  Inicia(VTBlocos *blocos);
	  bool    __fastcall  Inicia(VTRedes *redes, bool incluir_rede_isolada);
	  bool    __fastcall  Inicia(TList *lisMREDE);
      bool    __fastcall  Inicia(TList *lisLIG, VTBarra *barra_ini);
	  VTNode* __fastcall  InsereNode(VTNode *node_pai);
	  void    __fastcall  InsereNode(VTNode *node_pai, VTNode *node_filho);
	  void    __fastcall  LisAllNode(TList *lisEXT);
	  void    __fastcall  LisAllObject(TList *lisEXT);
	  void    __fastcall  LisNode(TList *lisEXT, int level);
	  TList*  __fastcall  LisNodeRaiz(void);
	  bool    __fastcall  RetiraNode(VTNode *node);
	  void    __fastcall  TreeViewInicia(TTreeView *TView, bool sort=true);

	private: //métodos
      void __fastcall DefineNodeId(TList *lisNODE, int &node_id);

	};

//---------------------------------------------------------------------------
#endif
//eof
