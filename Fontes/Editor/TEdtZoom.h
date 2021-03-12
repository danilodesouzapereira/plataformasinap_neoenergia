//---------------------------------------------------------------------------
#ifndef TEdtZoomH
#define TEdtZoomH

//---------------------------------------------------------------------------
#include "TEdt.h";

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class TEdtZoom: public TEdt
   {
   public:     //métodos
           __fastcall  TEdtZoom (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtZoom (void);
      void __fastcall  EvtMouseDown(void);
      void __fastcall  EvtSelArea(int x1, int y1, int x2, int y2);

   private: //métodos
      void __fastcall AtualizaCursor(void);

   protected:  //atributos
   };

#endif
//---------------------------------------------------------------------------
//eof
