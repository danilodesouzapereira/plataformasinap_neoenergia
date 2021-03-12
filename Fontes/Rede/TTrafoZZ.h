//---------------------------------------------------------------------------
#ifndef TTrafoZZH
#define TTrafoZZH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTTrafoZZ.h"

//---------------------------------------------------------------------------
class TTrafoZZ : public VTTrafoZZ
   {
   public:
                 __fastcall  TTrafoZZ(void);
                 __fastcall ~TTrafoZZ(void);
      VTTrafoZZ* __fastcall  Clone(void);
      void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
      int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);
		bool       __fastcall  Z0_pu(double Sbase, strIMP &z0_pu);
	};
   
//---------------------------------------------------------------------------
#endif
//eof

 