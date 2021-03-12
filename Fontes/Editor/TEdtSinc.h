//---------------------------------------------------------------------------
#ifndef TEdtSincH
#define TEdtSincH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdt.h"

//---------------------------------------------------------------------------
class TFormEdtSinc;
class VTChave;

//---------------------------------------------------------------------------
class TEdtSinc: public TEdt
   {
   public:   //m�todos
           __fastcall TEdtSinc (VTApl *apl, TAction *ResetFocus, TAction *SetFocus,
                                TAction *OnDone, TWinControl *parent);
           __fastcall ~TEdtSinc (void);
      void __fastcall ResetGrafFocus(void);
      void __fastcall SetGrafFocus(void);

   private: //m�todos
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtMouseMove(void);
      
   private:  //atributos
      TFormEdtSinc *FormEdtSinc;
      VTChave      *pchave;
   };

#endif
//---------------------------------------------------------------------------
//eof
