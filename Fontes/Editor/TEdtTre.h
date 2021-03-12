//---------------------------------------------------------------------------
#ifndef TEdtTreH
#define TEdtTreH

//---------------------------------------------------------------------------
#include "TEdtLiga.h"

//---------------------------------------------------------------------------
class VTTrecho;

//---------------------------------------------------------------------------
class TEdtTre : public TEdtLiga
   {
   public:
           __fastcall TEdtTre(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtTre(void);
      void __fastcall CriaLigacao(void);
      bool __fastcall ConfirmaLigacao(void);
   };
             
#endif
//---------------------------------------------------------------------------
//eof

