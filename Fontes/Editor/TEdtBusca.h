//---------------------------------------------------------------------------
#ifndef TEdtBuscaH
#define TEdtBuscaH

//---------------------------------------------------------------------------
#include "TEdt.h";

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class TEdtBusca: public TEdt
   {
   public:     //métodos
           __fastcall TEdtBusca (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtBusca (void);
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtMouseMove(void);
      void __fastcall EvtSelArea(int x1, int y1, int x2, int y2);

   private: //métodos
      void __fastcall AtualizaCursor(void);

   protected:  //atributos
      VTEqpto *eqpto;
      TList   *lisEQP;
   };

#endif
//---------------------------------------------------------------------------
//eof
