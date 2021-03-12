//---------------------------------------------------------------------------
#ifndef TEdtSup3H
#define TEdtSup3H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdtEqbar.h"

//---------------------------------------------------------------------------
class VTSuprimento;

//---------------------------------------------------------------------------
class TEdtSup3: public TEdtEqbar
   {
   public:     //métodos
			  __fastcall  TEdtSup3 (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
			  __fastcall ~TEdtSup3 (void);
		void __fastcall  CriaEqbar(void);
		bool __fastcall  ConfirmaEqbar(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

