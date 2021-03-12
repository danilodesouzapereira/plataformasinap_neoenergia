//---------------------------------------------------------------------------
#ifndef TMontaArvoreH
#define TMontaArvoreH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMontaArvore.h"

//---------------------------------------------------------------------------
class VTArvore;
class VTNode;


//---------------------------------------------------------------------------
class TMontaArvore : public VTMontaArvore
   {
   public:
			  __fastcall  TMontaArvore(VTApl *apl);
			  __fastcall ~TMontaArvore(void);
		bool __fastcall  InsereNode(strNODE &str);

	private:	//métodos
      VTNode* __fastcall ExisteNode(int node_id);

	private:	//objetos externos
		VTApl    *apl;
		VTArvore *arvore;

   private: //dados locais
      TList *lisNODE;
	};

#endif
//---------------------------------------------------------------------------
//eof

