//---------------------------------------------------------------------------
#ifndef TValEqptoH
#define TValEqptoH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTValEqpto.h"

//---------------------------------------------------------------------------
class VTApl;
class VTParametro;

//---------------------------------------------------------------------------
class TValEqpto : public VTValEqpto
	{
	public:
							__fastcall  TValEqpto(VTApl *apl);
							__fastcall ~TValEqpto(void);
		AnsiString 		__fastcall TraduzCabo(double cabo_d);
		int 				__fastcall TraduzFasesReg(double liga);
		int 				__fastcall TraduzLigacao(double liga);
		int 				__fastcall TraduzLigacaoReg(double liga);
		bool 				__fastcall ValidaArranjo(strARRANJO2 &str);
		bool 				__fastcall ValidaCabo(strCABO &str);
		bool 				__fastcall ValidaCapacitor(strCAPACITOR &str);
		bool   			__fastcall ValidaCargaA(strCARGA &str);
		bool   			__fastcall ValidaCargaB(strCARGA &str);
		bool 				__fastcall ValidaCargaEQV(strCARGA &str);
		bool 				__fastcall ValidaChave(strCHAVE &str);
		bool 				__fastcall ValidaGerador(strGERADOR &str);
		bool 				__fastcall ValidaRegulador(strREGULADOR &str);
		bool 				__fastcall ValidaSuporte(strSUPORTE &str);
		bool 				__fastcall ValidaSuprimento(strSUPRIMENTO &str);
		bool 				__fastcall ValidaTrafoSED(strTRAFO &str);
		bool 				__fastcall ValidaTrafoMTBT(strTRAFO &str);
		bool 				__fastcall ValidaTrechoMT(strTRECHO &str, strARRANJO1 &str_arr);
		bool 				__fastcall ValidaTrechoBT(strTRECHO &str, strARRANJO1 &str_arr);

	private: //objetos externos
		VTApl       *apl;

	private: //obj locais

   };
//---------------------------------------------------------------------------
#endif
