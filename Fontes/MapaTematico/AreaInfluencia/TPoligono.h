//---------------------------------------------------------------------------
#ifndef TPoligonoH
#define TPoligonoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTPoligono.h"

//-----------------------------------------------------------------------------
class TPoligono : public VTPoligono
   {
   public:
           __fastcall  TPoligono(int size);
           __fastcall ~TPoligono(void);
      bool __fastcall  AreaInternaAreaRetangular(int x1, int y1, int x2, int y2);
      bool __fastcall  InserePonto(int x, int y);
      bool __fastcall  PontoInterno(int x, int y);

   private: //métodos

   private: //dados
      struct   {
               int x1, y1, x2, y2;
               }rect;
   };

//---------------------------------------------------------------------------
#endif
//eof
