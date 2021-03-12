//---------------------------------------------------------------------------
#ifndef TDadoH
#define TDadoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTDado.h"

//---------------------------------------------------------------------------
class TDado : public VTDado
   {
   public:
                 __fastcall TDado(AnsiString descricao);
                 __fastcall ~TDado(void);
      AnsiString __fastcall DescUnid(void);
      AnsiString __fastcall DescUnidValor(void);
      AnsiString __fastcall DescValorUnid(void);
      AnsiString __fastcall ValorUnid(void);
   };
   
//---------------------------------------------------------------------------
#endif
//eof
