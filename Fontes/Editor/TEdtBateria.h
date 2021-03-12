//---------------------------------------------------------------------------
#ifndef TEdtBateriaH
#define TEdtBateriaH

//---------------------------------------------------------------------------
#include "TEdtEqbar.h"

//---------------------------------------------------------------------------
class TEdtBateria: public TEdtEqbar
   {
   public:     //métodos
		   __fastcall TEdtBateria (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
		   __fastcall ~TEdtBateria (void);
      void __fastcall CriaEqbar(void);
	  bool __fastcall ConfirmaEqbar(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

