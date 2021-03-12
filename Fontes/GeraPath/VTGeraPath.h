//---------------------------------------------------------------------------
#ifndef VTGeraPathH
#define VTGeraPathH

//---------------------------------------------------------------------------
class VTApl;
class VTArvores;
class VTEqpto;
class VTLigacao;

//---------------------------------------------------------------------------
class VTGeraPath: public TObject
	{
	public:

								 __fastcall VTGeraPath(void) {};
		virtual            __fastcall ~VTGeraPath(void) {};
		virtual void       __fastcall AddCarga(VTEqpto* eqpto) = 0;
		virtual void       __fastcall AddFonte(VTEqpto* eqpto) = 0;
		virtual void       __fastcall AddLigaExc(VTLigacao* pliga, bool flag_12) = 0;
		virtual void       __fastcall DefLiga(TList* lisLiga) = 0;
		virtual bool       __fastcall GeraArvore(TList* lisArv, TList* lisSUP, bool flag_inv = false) = 0;
		virtual VTArvores* __fastcall GeraArvore(VTEqpto* eqpto, bool flag_inv = false) = 0;
		virtual VTArvores* __fastcall GeraCopia(VTArvores* arvs, VTEqpto* eqpto) = 0;
		virtual bool       __fastcall LigaIsolada(VTLigacao* pliga) = 0;
		virtual bool       __fastcall LisFontesConect(VTEqpto* eqpto, TList* lisConect) = 0;
		virtual TList*     __fastcall LisTipoProt(void) = 0;
		virtual bool       __fastcall VerConect(VTEqpto* eqpto) = 0;
		//virtual bool       __fastcall GeraArvore1(TList* lisArv, TList* lisSUP, bool flag_inv = false) = 0;

	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTConfiab
//---------------------------------------------------------------------------
VTGeraPath* __fastcall NewObjGeraPath(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


 