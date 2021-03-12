//---------------------------------------------------------------------------
#ifndef TEdtCargaIPH
#define TEdtCargaIPH

//---------------------------------------------------------------------------
#include "TEdtEqbar.h"

//---------------------------------------------------------------------------
class TEdtCargaIP: public TEdtEqbar
   {
   public:     //métodos
           __fastcall TEdtCargaIP (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtCargaIP (void);
      void __fastcall CriaEqbar(void);
      bool __fastcall ConfirmaEqbar(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

