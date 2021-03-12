//---------------------------------------------------------------------------
#ifndef VTPoligonoH
#define VTPoligonoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTPoligono : public TObject
   {
   public:  //property
      __property TPoint* Point = {read=PD.point};
      __property int     Size  = {read=PD.size};

   public:
                   __fastcall  VTPoligono(void) {};
                   __fastcall ~VTPoligono(void) {};
      virtual bool __fastcall  AreaInternaAreaRetangular(int x1, int y1, int x2, int y2) = 0;
      virtual bool __fastcall  InserePonto(int x, int y) = 0;
      virtual bool __fastcall  PontoInterno(int x, int y) = 0;

   protected:  //dados acessados via property
      struct   {
               TPoint *point;
               int    size;
               int    next;
               }PD;
   };

//---------------------------------------------------------------------------
VTPoligono* __fastcall NewObjPoligono(int size);

//---------------------------------------------------------------------------
#endif
//eof
