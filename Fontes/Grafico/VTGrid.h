//---------------------------------------------------------------------------
#ifndef VTGridH
#define VTGridH

//---------------------------------------------------------------------------
#include <Classes.hpp>


//---------------------------------------------------------------------------
enum gridALIGN {alNONE=0, alCENTER, alUP_LEFT, alUP_RIGHT, alDOWN_LEFT, alDOWN_RIGHT};

//---------------------------------------------------------------------------
class VTGrid : public TObject
   {
   public:
      __fastcall VTGrid(void) {};
      __fastcall ~VTGrid(void) {};
      virtual void __fastcall Default(void) = 0;
      virtual void __fastcall SnapToGrid(int &x, int &y) = 0;
      virtual void __fastcall SnapToGrid(int &x1, int &y1, int &x2, int &y2) = 0;
      virtual bool __fastcall Visible(void) = 0;

   public:  //dados
      bool   visible;
      int    escala;
      int    dim;
      TColor cor;
      int    align;
   };

//-----------------------------------------------------------------------------
// função global  p/ criar objeto VTGrid
//-----------------------------------------------------------------------------
VTGrid* NewObjGrid(void);

#endif
//---------------------------------------------------------------------------
//eof
