//---------------------------------------------------------------------------
#ifndef TEdtMedidorH
#define TEdtMedidorH

//---------------------------------------------------------------------------
#include "TEdt.h"

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class TEdtMedidor: public TEdt
   {
   public:     //métodos
           __fastcall TEdtMedidor (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtMedidor (void);
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtMouseMove(void);

   private: //métodos
      void     __fastcall AtualizaCursor(void);
      VTEqpto* __fastcall BuscaEqpto(void);

   protected:  //atributos
      VTEqpto *eqpto;
   };

#endif
//---------------------------------------------------------------------------
//eof
