//---------------------------------------------------------------------------
#ifndef TEdtChvH
#define TEdtChvH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdtLiga.h"

//---------------------------------------------------------------------------
class VTEqbar;
class VTBarra;
class VTLigacao;

//---------------------------------------------------------------------------
class TEdtChv : public TEdtLiga
   {
   public:
           __fastcall  TEdtChv(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtChv(void);
      void __fastcall  CriaLigacao(void);
      bool __fastcall  ConfirmaLigacao(void);
   };
             
#endif
//---------------------------------------------------------------------------
//eof

