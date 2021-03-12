//---------------------------------------------------------------------------
#ifndef TDtrf_H
#define TDtrf_H

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class TDbar;

//---------------------------------------------------------------------------
class TDtrf : TObject
   {
   public:
           __fastcall TDtrf(void);
           __fastcall ~TDtrf(void);

   public:
      int        id;
      AnsiString codigo;
      double     snom;
      strIMP     z1;
      strIMP     z0;
      double     tap_pu;
      struct     {
                 int    ligacao;
                 double vnom;
                 }pri, sec;
      TDbar     *dbar1, *dbar2, *dbar_md;
   };

#endif
//-----------------------------------------------------------------------------
// eof
