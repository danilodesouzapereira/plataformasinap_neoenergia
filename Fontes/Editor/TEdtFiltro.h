//---------------------------------------------------------------------------
#ifndef TEdtFiltroH
#define TEdtFiltroH

//---------------------------------------------------------------------------
#include "TEdtEqbar.h"

//---------------------------------------------------------------------------
class TEdtFiltro: public TEdtEqbar
   {
   public:     //m�todos
           __fastcall TEdtFiltro(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtFiltro(void);
      void __fastcall CriaEqbar(void);
      bool __fastcall ConfirmaEqbar(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

