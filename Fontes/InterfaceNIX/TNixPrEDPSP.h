//---------------------------------------------------------------------------
#ifndef TNixPrEDPSPH
#define TNixPrEDPSPH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>
#include "TNixPr.h"
//---------------------------------------------------------------------------
class VTChave;
//---------------------------------------------------------------------------
class TNixPrEDPSP : public TNixPr
   {
   public:
                 __fastcall  TNixPrEDPSP(VTChave *chave, int index, bool pr_com_corrente);
				 __fastcall ~TNixPrEDPSP(void);

   private: //métodos
	  AnsiString      __fastcall TraduzCodigo(VTChave *chave, int index);
   };

//---------------------------------------------------------------------------
#endif
//eof

