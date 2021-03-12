//---------------------------------------------------------------------------
#ifndef TTema1H
#define TTema1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TTema.h"

//---------------------------------------------------------------------------
class TTema1 : public TTema
   {
   public:  //m�todos
           __fastcall  TTema1(VTApl *apl);
           __fastcall ~TTema1(void);

   protected://m�todos a serem implementados nas classes derivadas
		void __fastcall CalculaValorProprioCelula(void);
		void __fastcall CalculaValorTotalCelula(void);
		void __fastcall DefineCorCelula(void);
		void __fastcall NormalizaValorTotalCelula(void);

	private:  //m�todos
		TColor __fastcall Degrade(double valor);
		TColor __fastcall DegradeCinza(double valor);
   };

//---------------------------------------------------------------------------
#endif
//eof
