//---------------------------------------------------------------------------
#ifndef TEdtCurtoH
#define TEdtCurtoH

//---------------------------------------------------------------------------
#include "TEdt.h"

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class TEdtCurto: public TEdt
   {
   public:     //métodos
           __fastcall TEdtCurto (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtCurto (void);
      void __fastcall DefineActionCurto(TAction *OnCurto);
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtMouseMove(void);

   private: //métodos
      void     __fastcall AtualizaCursor(void);
      VTEqpto* __fastcall BuscaEqpto(void);

   protected:  //objetos externos
      TAction *ActionOnCurto;

   protected:  //dados locais
      VTEqpto *eqpto;
   };

#endif
//---------------------------------------------------------------------------
//eof
