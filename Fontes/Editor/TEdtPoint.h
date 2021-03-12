//---------------------------------------------------------------------------
#ifndef TEdtPointH
#define TEdtPointH

//---------------------------------------------------------------------------
#include "TEdt.h";

//---------------------------------------------------------------------------
class TEdtPoint: public TEdt
   {
   public:     //métodos
           __fastcall  TEdtPoint (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtPoint (void);
      void __fastcall  EvtMouseDown(void);

   protected:  //atributos
      TPoint point;
      TList  *lisPOINT;
   };

#endif
//---------------------------------------------------------------------------
//eof
