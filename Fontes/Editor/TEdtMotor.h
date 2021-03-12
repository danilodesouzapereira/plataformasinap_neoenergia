//---------------------------------------------------------------------------
#ifndef TEdtMotorH
#define TEdtMotorH

//---------------------------------------------------------------------------
#include "TEdtEqbar.h"

//---------------------------------------------------------------------------
class TEdtMotor: public TEdtEqbar
   {
   public:     //métodos
           __fastcall TEdtMotor(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtMotor(void);
      void __fastcall CriaEqbar(void);
      bool __fastcall ConfirmaEqbar(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

