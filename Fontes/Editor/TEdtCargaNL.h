//---------------------------------------------------------------------------
#ifndef TEdtCargaNLH
#define TEdtCargaNLH

//---------------------------------------------------------------------------
#include "TEdtEqbar.h"

//---------------------------------------------------------------------------
class TEdtCargaNL: public TEdtEqbar
   {
   public:     //métodos
           __fastcall TEdtCargaNL (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtCargaNL (void);
      void __fastcall CriaEqbar(void);
      bool __fastcall ConfirmaEqbar(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

