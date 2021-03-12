//---------------------------------------------------------------------------
#ifndef TEdtCargaH
#define TEdtCargaH

//---------------------------------------------------------------------------
#include "TEdtEqbar.h"

//---------------------------------------------------------------------------
class TEdtCarga: public TEdtEqbar
   {
   public:     //métodos
           __fastcall TEdtCarga (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtCarga (void);
      void __fastcall CriaEqbar(void);
      bool __fastcall ConfirmaEqbar(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

