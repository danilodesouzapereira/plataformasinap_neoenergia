//---------------------------------------------------------------------------
#ifndef VTGeraCorteH
#define VTGeraCorteH

//---------------------------------------------------------------------------
class VTApl;
//---------------------------------------------------------------------------
class VTGeraCorte: public TObject
   {
	public:
								__fastcall VTGeraCorte(void) {};
		virtual           __fastcall ~VTGeraCorte(void) {};
		virtual  bool     __fastcall ExecutaCorteA(TList* lisArvs, int icont) = 0;
		virtual  void     __fastcall InsereLigaExcl(TList* lis) = 0;
		virtual  TList*   __fastcall LisCortes(void) = 0;
		virtual  TList*   __fastcall LisExcl(void) = 0;
		//virtual  bool     __fastcall AgregaCortes(TList *lisArvs) = 0;
		//virtual  bool     __fastcall ExecutaCorte(VTArvores* arvs, int icont) = 0;
		//virtual  bool     __fastcall ExecutaCorteParcial(VTArvores* arvs) = 0;
		//virtual  bool     __fastcall ExecutaCorteArvores(TList* lisArvs, VTEqpto* eqpto, int icont) = 0;
		//virtual  bool     __fastcall ExecutaCorte1(VTArvores* arvs, bool flagNF) = 0;
		//virtual  bool     __fastcall ExecutaCorte2(VTArvores* arvs, bool flagNF) = 0;
		//virtual  bool     __fastcall ExecutaCorte3(VTArvores* arvs, bool flagNF) = 0;

	public:
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTConfiab
//---------------------------------------------------------------------------
VTGeraCorte* __fastcall NewObjGeraCorte(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 