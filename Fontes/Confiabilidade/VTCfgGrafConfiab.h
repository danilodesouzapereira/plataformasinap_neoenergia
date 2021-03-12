//---------------------------------------------------------------------------
#ifndef VTCfgGrafConfiabH
#define VTCfgGrafConfiabH
//---------------------------------------------------------------------------
//#define clYELLOW 0x00FFFF00
//#define clGREEN  0x0000FF00
//#define clRED    0x000000FF
//#define clWHITE  0x00FFFFFF
//---------------------------------------------------------------------------
enum eView { eDEC = 0, eFEC, eEND, ePOT, eTOPO};
//---------------------------------------------------------------------------
struct strLimite
	{
	double limite1, limite2;
	};
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTCfgGrafConfiab : public TObject
	{
	public:
		//contrutor da classe
		__fastcall VTCfgGrafConfiab(void) {};
		__fastcall ~VTCfgGrafConfiab(void) {};

		//interface
		virtual TColor __fastcall Cor(double dec, double fec, double end, double pot) = 0;

		//atributos publicos
		int index;
		strLimite dec, fec, end, pot;

	  };
//---------------------------------------------------------------------------
//função global
//---------------------------------------------------------------------------
VTCfgGrafConfiab *NewObjCfgGrafConfiab(void);
//---------------------------------------------------------------------------
#endif
