//---------------------------------------------------------------------------
#ifndef TCoordenadaH
#define TCoordenadaH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTCoordenada.h"

//---------------------------------------------------------------------------
class TCoordenada : public VTCoordenada
   {
   public:
      __fastcall TCoordenada(void);
      __fastcall ~TCoordenada(void);

	protected: //m�todos acessados via property
		void __fastcall PM_SetArea(TArea *area);
      void __fastcall PM_SetPoint(TPoint *point);
      void __fastcall PM_SetLine(TLine *line);
      void __fastcall PM_SetPolyline(TPolyline *polyline);
   };

#endif
//-----------------------------------------------------------------------------
// eof
