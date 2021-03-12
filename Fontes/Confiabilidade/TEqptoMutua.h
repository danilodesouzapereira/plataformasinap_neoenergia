//---------------------------------------------------------------------------
#ifndef TEqptoMutuaH
#define TEqptoMutuaH
//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <..\Rede\VTEqpto.h>

//---------------------------------------------------------------------------
class TEqptoMutua : public VTEqpto
   {
   public:
					  __fastcall  TEqptoMutua(void);
					  __fastcall ~TEqptoMutua(void);
		//void       __fastcall  AreaEsquematico(int &x1, int &y1, int &x2, int &y2);
		VTEqpto*   __fastcall  Clone(void) {return(NULL);};
		void       __fastcall  CopiaAtributosDe(VTEqpto &ref) {return;};
		void       __fastcall  CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2) {return;};
		void       __fastcall  CoordenadasEsquematico(int &xm, int &ym, int &x1, int &y1,
																	int &x2, int &y2, int &x3, int &y3) {return;};
		bool       __fastcall  CoordenadasUtm_cm(int &x, int &y) {return(true);};
		bool       __fastcall  CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2) {return(true);};
		bool       __fastcall  CoordenadasUtm_cm(int &xm, int &ym, int &x1, int &y1,
															 int &x2, int &y2, int &x3, int &y3) {return(true);};
		bool       __fastcall  CoordenadasUtm_m(int &x, int &y) {return(true);};
		bool       __fastcall  CoordenadasUtm_m(int &x1, int &y1, int &x2, int &y2) {return(true);};
      bool       __fastcall  CoordenadasUtm_m(int &xm, int &ym, int &x1, int &y1,
															int &x2, int &y2, int &x3, int &y3) {return(true);};
		int        __fastcall  Tipo(void) {return(-1);};
      AnsiString __fastcall  TipoAsString(void) {return("");};

	protected: //métodos acessados via property

	protected: //dados acessados via property

	public:  //dados locais
     VTEqpto *eqpto1;
     VTEqpto *eqpto2;
   };

//---------------------------------------------------------------------------
#endif
//eof
