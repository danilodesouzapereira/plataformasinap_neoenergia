//---------------------------------------------------------------------------
#ifndef TTema1H
#define TTema1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TTema.h"

//---------------------------------------------------------------------------
class TTema1 : public TTema
   {
   public:  //métodos
           __fastcall  TTema1(VTApl *apl);
           __fastcall ~TTema1(void);

   protected://métodos a serem implementados nas classes derivadas
		void __fastcall CalculaValorProprioCelula(void);
		void __fastcall CalculaValorTotalCelula(void);
		void __fastcall DefineCorCelula(void);
		void __fastcall NormalizaValorTotalCelula(void);

	private:  //métodos
		TColor __fastcall Degrade(double valor);
		TColor __fastcall DegradeCinza(double valor);
   };

//---------------------------------------------------------------------------
#endif
//eof
