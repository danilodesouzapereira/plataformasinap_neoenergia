//---------------------------------------------------------------------------
#ifndef TEdtMedicaoH
#define TEdtMedicaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdtEqbar.h"

//---------------------------------------------------------------------------
class VTMedidor;

//---------------------------------------------------------------------------
class TEdtMedicao: public TEdtEqbar
   {
   public:     //métodos
           __fastcall TEdtMedicao (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtMedicao (void);
      void __fastcall CriaEqbar(void);
      bool __fastcall ConfirmaEqbar(void);
   };

#endif
//---------------------------------------------------------------------------
//eof

