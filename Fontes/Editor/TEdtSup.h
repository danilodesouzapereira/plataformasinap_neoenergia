//---------------------------------------------------------------------------
#ifndef TEdtSupH
#define TEdtSupH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdtEqbar.h"

//---------------------------------------------------------------------------
class VTSuprimento;

//---------------------------------------------------------------------------
class TEdtSup: public TEdtEqbar
   {
   public:     //métodos
           __fastcall TEdtSup (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtSup (void);
      void __fastcall CriaEqbar(void);
      bool __fastcall ConfirmaEqbar(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

