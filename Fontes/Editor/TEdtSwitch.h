//---------------------------------------------------------------------------
#ifndef TEdtSwitchH
#define TEdtSwitchH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdt.h"

//---------------------------------------------------------------------------
class VTChave;
class VTApl;
//---------------------------------------------------------------------------
class TEdtSwitch: public TEdt
   {
   public:     //métodos
              __fastcall TEdtSwitch (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
      virtual __fastcall ~TEdtSwitch (void);

   private:
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtMouseMove(void);
      
   private:  //atributos
	  VTChave *pchave;
	  VTApl 	*apl;
   };

#endif
//---------------------------------------------------------------------------
//eof
