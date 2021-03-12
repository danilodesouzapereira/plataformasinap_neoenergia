//---------------------------------------------------------------------------
#ifndef TEdtTrfMonoH
#define TEdtTrfMonoH

//---------------------------------------------------------------------------
#include "TEdtLiga.h"

//---------------------------------------------------------------------------
class TEdtTrfMono : public TEdtLiga
   {
   public:
           __fastcall TEdtTrfMono(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
			  __fastcall ~TEdtTrfMono(void);
      void __fastcall CriaLigacao(void);
      bool __fastcall ConfirmaLigacao(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

