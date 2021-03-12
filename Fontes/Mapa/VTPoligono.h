//---------------------------------------------------------------------------
#ifndef VTPoligonoH
#define VTPoligonoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTArea;

//-----------------------------------------------------------------------------
class VTPoligono : public TObject
   {
   public:  //property
      __property TPoint* Point[int index] = {read=PM_GetPoint};
      __property int     Size             = {read=PM_GetSize};

   public:
                      __fastcall  VTPoligono(void) {};
                      __fastcall ~VTPoligono(void) {};
      virtual VTArea* __fastcall  AreaUtm_cm(void) = 0;
      virtual bool    __fastcall  AreaInternaAreaRetangular(int x1, int y1, int x2, int y2) = 0;
      virtual void    __fastcall  Clear(void) = 0;
      virtual bool    __fastcall  ContemPonto(int x, int y) = 0;
      virtual bool    __fastcall  InserePonto(int x, int y) = 0;
      virtual TList*  __fastcall  LisPoint(void) = 0;
      virtual bool    __fastcall  PontoInternoPoligono(int x, int y) = 0;
      virtual bool    __fastcall  PontoInternoAreaRetangula(int x, int y) = 0;

   protected:  //métodos acessados via Property
      virtual TPoint* __fastcall PM_GetPoint(int index) = 0;
      virtual int     __fastcall PM_GetSize(void) = 0;
   };

//---------------------------------------------------------------------------
VTPoligono* __fastcall NewObjPoligono(void);

//---------------------------------------------------------------------------
#endif
//eof
