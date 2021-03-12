//---------------------------------------------------------------------------
#ifndef TEdtAreaH
#define TEdtAreaH

//---------------------------------------------------------------------------
#include "TEdt.h";

//---------------------------------------------------------------------------
class TEdtArea: public TEdt
   {
   public:     //métodos
           __fastcall TEdtArea (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
			  __fastcall ~TEdtArea (void);
		void __fastcall EvtMouseDown(void);
		//void __fastcall EvtMouseMove(void);
		void __fastcall EvtSelArea(int x1, int y1, int x2, int y2);
	};

#endif
//---------------------------------------------------------------------------
//eof
