//---------------------------------------------------------------------------
#ifndef VTArvoresH
#define VTArvoresH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTArvore;
class VTEqpto;

//---------------------------------------------------------------------------
class VTArvores : public TObject
	{
	public:
								__fastcall VTArvores(void) {};
		virtual           __fastcall ~VTArvores(void) {};
		virtual bool      __fastcall AddCaminho(VTArvore* arv) = 0;
		virtual bool      __fastcall AtribCaminho(VTArvore* arv, VTArvore* arvEqpto) = 0;
		virtual bool      __fastcall AtribCaminhoBarra(VTArvore* arv, VTEqpto *eqpto) = 0;
		virtual bool      __fastcall AtribCaminhoProt(VTArvore* arv, VTArvore* arvEqpto) = 0;
		//virtual bool      __fastcall AtribCaminhoProt(VTArvore* arv, VTEqpto* eqpto) = 0;
		virtual VTEqpto*  __fastcall EqRaiz(void) = 0;
		virtual VTArvore* __fastcall Arv(void) = 0;
		virtual TList*    __fastcall LisArvProt(void) = 0;
		virtual TList*    __fastcall LisCortesP(void) = 0;
		virtual TList*    __fastcall LisEqpto(void) = 0;
		virtual TList*    __fastcall LisEqptoPath(void) = 0;
		virtual TList*    __fastcall LisPath(bool flagNF, VTEqpto *eqpto=NULL) = 0;
		virtual TList*    __fastcall LisPathProt(VTEqpto *eqpto) = 0;
		virtual TList*    __fastcall LisEqProt(VTEqpto *eqpto) = 0;
		virtual bool      __fastcall NA(VTEqpto* eqpto) = 0;
		//virtual TList*    __fastcall LisCortes(int icorte) = 0;
		//virtual bool      __fastcall AtribArvore(VTArvore* arv, VTEqpto *eqpto) = 0;

	public:
      struct {
             double dic, fic;
             double tx_ff, r_ff;
             double tx_fm, r_fm;
             double pmed;
				 int    ncons;
				 }res_conf;
		VTArvores* arvs_ori;
	};

//função global p/ criar objeto
VTArvores* __fastcall NewObjArvores(VTEqpto* eqpto);

#endif
//---------------------------------------------------------------------------
//eof

