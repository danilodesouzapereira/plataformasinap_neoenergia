//---------------------------------------------------------------------------
#ifndef TConjuntoAneelH
#define TConjuntoAneelH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TConjuntoAneel : public TObject
	{
	public:
		__fastcall TConjuntoAneel(AnsiString CodSE);
		__fastcall ~TConjuntoAneel(void);

	public:
		int        Codigo;
		AnsiString Nome;
		double     DEC;
		double     FEC;
		AnsiString CodSE;
	};
	
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
//eof