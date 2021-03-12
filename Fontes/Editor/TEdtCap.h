//---------------------------------------------------------------------------
#ifndef TEdtCapH
#define TEdtCapH

//---------------------------------------------------------------------------
#include "TEdtEqbar.h"

//---------------------------------------------------------------------------
class TEdtCap: public TEdtEqbar
   {
   public:     //métodos
           __fastcall TEdtCap(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtCap(void);
      void __fastcall CriaEqbar(void);
      bool __fastcall ConfirmaEqbar(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

