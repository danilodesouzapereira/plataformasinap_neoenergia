//---------------------------------------------------------------------------
#ifndef TEdtGerH
#define TEdtGerH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdtEqbar.h"

//---------------------------------------------------------------------------
class VTGerador;

//---------------------------------------------------------------------------
class TEdtGer: public TEdtEqbar
   {
   public:     //métodos
           __fastcall TEdtGer (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtGer (void);
      void __fastcall CriaEqbar(void);
      bool __fastcall ConfirmaEqbar(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

