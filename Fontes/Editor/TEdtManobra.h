//---------------------------------------------------------------------------
#ifndef TEdtManobraH
#define TEdtManobraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdt.h"

//---------------------------------------------------------------------------
class TFormEdtManobra;
class VTChave;

//---------------------------------------------------------------------------
class TEdtManobra: public TEdt
   {
   public:   //m�todos
           __fastcall TEdtManobra (VTApl *apl, TAction *ResetFocus, TAction *SetFocus,
                                   TAction *OnDone, TWinControl *parent);
           __fastcall ~TEdtManobra (void);
      void __fastcall ResetGrafFocus(void);
      void __fastcall SetGrafFocus(void);

   private: //m�todos
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtMouseMove(void);
      
   private:  //atributos
      TFormEdtManobra *FormEdtManobra;
      VTChave         *pchave;
   };

#endif
//---------------------------------------------------------------------------
//eof
