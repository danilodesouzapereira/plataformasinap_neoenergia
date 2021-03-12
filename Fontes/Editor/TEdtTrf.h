//---------------------------------------------------------------------------
#ifndef TEdtTrfH
#define TEdtTrfH

//---------------------------------------------------------------------------
#include "TEdtLiga.h"

//---------------------------------------------------------------------------
class TEdtTrf : public TEdtLiga
   {
   public:
           __fastcall TEdtTrf(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtTrf(void);
      void __fastcall CriaLigacao(void);
      bool __fastcall ConfirmaLigacao(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

