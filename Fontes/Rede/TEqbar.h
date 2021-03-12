// ---------------------------------------------------------------------------
#ifndef TEqbarH
#define TEqbarH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTEqbar.h"

// ---------------------------------------------------------------------------
class TEqbar : public VTEqbar
{
public:
	__fastcall TEqbar(void);
	virtual __fastcall ~TEqbar(void);
	void __fastcall CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2);
	void __fastcall CoordenadasEsquematico(int &xm, int &ym, int &x1, int &y1, int &x2, int &y2,
		int &x3, int &y3);
	bool __fastcall CoordenadasUtm_cm(int &x, int &y);
	bool __fastcall CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2);
	bool __fastcall CoordenadasUtm_cm(int &xm, int &ym, int &x1, int &y1, int &x2, int &y2, int &x3,
		int &y3);
	bool __fastcall CoordenadasUtm_m(int &x, int &y);
	bool __fastcall CoordenadasUtm_m(int &x1, int &y1, int &x2, int &y2);
	bool __fastcall CoordenadasUtm_m(int &xm, int &ym, int &x1, int &y1, int &x2, int &y2, int &x3,
		int &y3);
	void __fastcall CopiaAtributosDe(VTEqpto &ref);
	void __fastcall DefineObjBarra(VTBarra *pbar);
	void __fastcall DefinePosicao(int x, int y);
	bool __fastcall TipoEqbar(void);

private: // métodos
	void __fastcall DefineDisbarMinima(void);
};

// ---------------------------------------------------------------------------
#endif
// eof
