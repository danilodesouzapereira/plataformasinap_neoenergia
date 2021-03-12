//---------------------------------------------------------------------------
#ifndef TCfgGrafConfiabH
#define TCfgGrafConfiabH
//---------------------------------------------------------------------------
#include <VTCfgGrafConfiab.h>
//---------------------------------------------------------------------------
class TCfgGrafConfiab : public VTCfgGrafConfiab
	{
	public:
		//contrutor da classe
		__fastcall TCfgGrafConfiab(void);
		__fastcall ~TCfgGrafConfiab(void);

		//interface
		TColor __fastcall Cor(double dec, double fec, double end, double pot);

	private:
   	//métodos internos
		TColor __fastcall CorDEC(double dec);
		TColor __fastcall CorFEC(double fec);
		TColor __fastcall CorEND(double end);
		TColor __fastcall CorPOT(double pot);
   };
//---------------------------------------------------------------------------
#endif
