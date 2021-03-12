//---------------------------------------------------------------------------
#ifndef TUnifTopoH
#define TUnifTopoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TUnifEditor.h"

//---------------------------------------------------------------------------
class TUnifTopo : public TUnifEditor
   {
   public:
           __fastcall TUnifTopo(VTApl *apl, int tipo_graf, AnsiString nome);
           __fastcall ~TUnifTopo(void);
      void __fastcall Redraw(VTEdt *editor);
      void __fastcall Redraw(int op, VTEdt *editor);
   };

#endif
//---------------------------------------------------------------------------
//eof
