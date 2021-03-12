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
              __fastcall  TPoligono(void);
              __fastcall ~TPoligono(void);
      VTArea* __fastcall  AreaUtm_cm(void);
      bool    __fastcall  AreaInternaAreaRetangular(int x1, int y1, int x2, int y2);
      void    __fastcall  Clear(void);
      bool    __fastcall  ContemPonto(int x, int y);
      bool    __fastcall  InserePonto(int x, int y);
      TList*  __fastcall  LisPoint(void);
      bool    __fastcall  PontoInternoPoligono(int x, int y);
      bool    __fastcall  PontoInternoAreaRetangula(int x, int y);

   protected:  //métodos acessados via Property
      TPoint* __fastcall PM_GetPoint(int index);
      int     __fastcall PM_GetSize(void);

   private: //métodos

   private: //dados
      TList *lisPOINT;
      VTArea *area_utm;
      /*
      struct   {
               int x1, y1, x2, y2;
               }rect;
      */
   };

//---------------------------------------------------------------------------
#endif
//eof
