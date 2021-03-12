//---------------------------------------------------------------------------
#ifndef TIconeH
#define TIconeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTIcone.h"

//---------------------------------------------------------------------------
class TIcone : public VTIcone
   {
   public:
           __fastcall  TIcone(void);
           __fastcall ~TIcone(void);
      void __fastcall  InsereArco(int x1, int y1, int x2, int y2,
                                   int x3, int y3, int x4, int y4, int pen_width, TColor cor);
      void __fastcall  InsereElipse(int x1, int y1, int x2, int y2, int pen_width, TColor cor);
      void __fastcall  InsereLinha(int x1, int y1, int x2, int y2, int pen_width, TColor cor);
   };

#endif
//---------------------------------------------------------------------------
//eof

