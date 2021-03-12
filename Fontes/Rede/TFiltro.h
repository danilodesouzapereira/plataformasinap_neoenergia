//---------------------------------------------------------------------------
#ifndef TFiltroH
#define TFiltroH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTFiltro.h"

//---------------------------------------------------------------------------
class TFiltro : public VTFiltro
   {
   public:
                 __fastcall  TFiltro(void);
                 __fastcall ~TFiltro(void);
      VTFiltro*  __fastcall  Clone(void);
      void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
      int        __fastcall  Tipo(void);
		AnsiString __fastcall  TipoAsString(void);
		bool       __fastcall  Z0_pu(double Sbase, strIMP &z0_pu);
   };
   
//---------------------------------------------------------------------------
#endif
//eof

 