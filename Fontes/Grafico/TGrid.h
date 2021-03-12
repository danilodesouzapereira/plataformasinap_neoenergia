//---------------------------------------------------------------------------
#ifndef TGridH
#define TGridH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTGrid.h"

//---------------------------------------------------------------------------
class TGrid : public VTGrid
   {
   public:
      __fastcall TGrid(void);
      __fastcall ~TGrid(void);
      void __fastcall Default(void);
      void __fastcall SnapToGrid(int &x, int &y);
      void __fastcall SnapToGrid(int &x1, int &y1, int &x2, int &y2);
      bool __fastcall Visible(void);
   };

#endif
//---------------------------------------------------------------------------
//eof
