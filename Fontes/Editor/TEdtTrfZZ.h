//---------------------------------------------------------------------------
#ifndef TEdtTrfZZH
#define TEdtTrfZZH

//---------------------------------------------------------------------------
#include "TEdtEqbar.h"

//---------------------------------------------------------------------------
class TChave;
class TTrafoZZ;

//---------------------------------------------------------------------------
class TEdtTrfZZ: public TEdtEqbar
   {
   public:     //métodos
           __fastcall TEdtTrfZZ(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtTrfZZ(void);
      void __fastcall CriaEqbar(void);
      bool __fastcall ConfirmaEqbar(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

