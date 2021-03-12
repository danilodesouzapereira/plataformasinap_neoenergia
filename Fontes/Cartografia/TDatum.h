//---------------------------------------------------------------------------
#ifndef TDatumH
#define TDatumH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//#include <string.h>

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15000938
class TDatum : public TObject
	{
	public:
			    __fastcall TDatum(void);
		        __fastcall ~TDatum(void);
		double  __fastcall Achat(int eId);
		double  __fastcall Dx(int eId);
		double  __fastcall Dy(int eId);
		double  __fastcall Dz(int eId);
        double  __fastcall SemiEixo(int eId);
	};

//---------------------------------------------------------------------------
#endif
//eof

