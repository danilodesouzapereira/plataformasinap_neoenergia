//---------------------------------------------------------------------------
#ifndef TEdtCapsH
#define TEdtCapsH

//---------------------------------------------------------------------------
#include "TEdtLiga.h"

//---------------------------------------------------------------------------
class VTCapserie;

//---------------------------------------------------------------------------
class TEdtCaps : public TEdtLiga
   {
   public:
           __fastcall TEdtCaps(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtCaps(void);
      void __fastcall CriaLigacao(void);
      bool __fastcall ConfirmaLigacao(void);
   };
             
#endif
//---------------------------------------------------------------------------
//eof

