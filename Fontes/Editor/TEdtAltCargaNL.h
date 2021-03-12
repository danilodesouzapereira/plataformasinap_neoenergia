//---------------------------------------------------------------------------
#ifndef TEdtAltCargaNLH
#define TEdtAltCargaNLH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdtAlt.h"

//---------------------------------------------------------------------------
class TEdtAltCargaNL : public TEdtAlt
   {
   public:
             __fastcall  TEdtAltCargaNL(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
             __fastcall ~TEdtAltCargaNL(void);

   private:  //métodos
      bool __fastcall BuscaEqpto(void);
      void __fastcall EstadoSel1_EvtMouseUp(void);

   };

#endif
//---------------------------------------------------------------------------
//eof
