//---------------------------------------------------------------------------
#ifndef TEdtReatorH
#define TEdtReatorH

//---------------------------------------------------------------------------
#include "TEdtEqbar.h"

//---------------------------------------------------------------------------
class TEdtReator: public TEdtEqbar
   {
   public:     //métodos
           __fastcall TEdtReator(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtReator(void);
      void __fastcall CriaEqbar(void);
      bool __fastcall ConfirmaEqbar(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

