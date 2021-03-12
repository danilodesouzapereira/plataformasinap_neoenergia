//---------------------------------------------------------------------------
#ifndef TGeraCorteH
#define TGeraCorteH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include "VTGeraCorte.h"

//---------------------------------------------------------------------------
class VTArvore;
class VTArvores;
class VTEqpto;
class VTLigacao;
//---------------------------------------------------------------------------
class TGeraCorte: public VTGeraCorte
   {
   public:
               __fastcall TGeraCorte(VTApl* apl);
               __fastcall ~TGeraCorte(void);
	 bool       __fastcall ExecutaCorteA(TList* lisArvs, int icont);
	 void       __fastcall InsereLigaExcl(TList* lis);
	 TList*     __fastcall LisCortes(void);
	 TList*     __fastcall LisExcl(void);
	 //bool       __fastcall ExecutaCorteParcial(VTArvores* arvs);
	 //bool       __fastcall AgregaCortes(TList *lisArvs);
	 //bool       __fastcall ExecutaCorte(VTArvores* arvs, int icont);
	 //bool       __fastcall ExecutaCorteArvores(TList* lisArvs, VTEqpto* eqpto, int icont);
	 //bool       __fastcall ExecutaCorte1(VTArvores* arvs, bool flagNF);
	 //bool       __fastcall ExecutaCorte2(VTArvores* arvs, bool flagNF);
	 //bool       __fastcall ExecutaCorte3(VTArvores* arvs, bool flagNF);

	private: //métodos
	 bool       __fastcall AvaliaCorte1(VTEqpto* eqpto_sai, VTEqpto* eqpto, VTArvores* arvs);
	 bool       __fastcall AvaliaLiga(VTLigacao* pliga);
	 bool       __fastcall ExecutaCorteA(VTArvores* arvs, int icont);
	 bool       __fastcall ExecutaFluxo(void);
    bool       __fastcall RealocaCortes(TList* lis1, TList* lis2, TList* lis3, VTArvores* arvs);
    bool       __fastcall VerChvSoc(VTEqpto* eqpto);
    bool       __fastcall VerCompLis(TList* lis1, TList* lis2);
    bool       __fastcall VerEqptoCorte(TList* lisCorte, VTEqpto* eqpto1, VTEqpto* eqpto2, VTEqpto* eqpto3);
	 bool       __fastcall VerLisSoc(TList* lisSoc, VTArvores* arvs);

   private: //dados externos
        VTApl* apl;

   private: //dados locais
		TList         *lisEQS;
		TList         *lisC1, *lisC2, *lisC3;//, *lisCortes;
		TList         *lisCortes;
		TList         *lisExcl;
	};
#endif
//---------------------------------------------------------------------------
//eof

