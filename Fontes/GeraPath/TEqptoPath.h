//---------------------------------------------------------------------------
#ifndef TEqptoPathH
#define TEqptoPathH
//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTArvore;
class VTEqpto;

//---------------------------------------------------------------------------
class TEqptoPath : public TObject
   {
   public:
             __fastcall TEqptoPath(VTEqpto* eqpto);
             __fastcall ~TEqptoPath(void);
		  bool __fastcall AddCaminho(VTArvore* arv);
		  bool __fastcall AddCaminhoProt(VTArvore* arv);
		  bool __fastcall AddArvore(VTArvore* arv);
		  bool __fastcall AddEqProt(VTArvore* arv);
	 VTEqpto* __fastcall Eqpto(void);
	 TList*   __fastcall LisEqProt(void);
	 TList*   __fastcall LisArv(void);
	 TList*   __fastcall LisPath(bool flagNF);
    TList*   __fastcall LisPathProt(void);
		  bool __fastcall NA(void);

   private: //métodos

   private: //dados externos

   private: //dados locais
		VTEqpto *eqpto, *eqptoProt;
		TList* lisEqProt;
		TList* lisArv;   //lista de ramos ligados por eqpto
		TList* lisPath;   //lista de caminhos - árvores extremas
		TList* lisPathP;  //lista de caminhos desligados por protecao
		TList* lisPathNF; //lista de caminhos - árvores extremas
		TList* lisFontes; //lista de fontes atingidas
      bool   flag_NA;
   };

#endif
//---------------------------------------------------------------------------
//eof
