//---------------------------------------------------------------------------
#ifndef TAtivoH
#define TAtivoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAtivo.h"

//---------------------------------------------------------------------------
class TAtivo : public VTAtivo
   {
   public:
                __fastcall  TAtivo(void);
                __fastcall ~TAtivo(void);
     AnsiString __fastcall TipoAcaoAsString(void);
     AnsiString __fastcall TipoAtivoAsString(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

