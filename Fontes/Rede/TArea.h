//---------------------------------------------------------------------------
#ifndef TAreaH
#define TAreaH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTArea.h"

//---------------------------------------------------------------------------
class TArea : public VTArea
   {
   public: //métodos
             __fastcall  TArea(void);
             __fastcall  TArea(double xa, double ya, double xb, double yb);
             __fastcall ~TArea(void);
      bool   __fastcall  ContemPonto(double x, double y);
		void   __fastcall  CopiaAtributosDe(VTArea &ref);
      double __fastcall  Height(void);
      bool   __fastcall  Igual(VTArea *area);
      double __fastcall  Width(void);
   };

//---------------------------------------------------------------------------
#endif
//eof

