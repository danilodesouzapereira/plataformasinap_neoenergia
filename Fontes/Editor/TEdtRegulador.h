//---------------------------------------------------------------------------
#ifndef TEdtReguladorH
#define TEdtReguladorH

//---------------------------------------------------------------------------
#include "TEdtLiga.h"

//---------------------------------------------------------------------------
class TEdtRegulador : public TEdtLiga
   {
   public:
           __fastcall TEdtRegulador(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtRegulador(void);
      void __fastcall CriaLigacao(void);
	  bool __fastcall ConfirmaLigacao(void);

   private:
	  bool    __fastcall IsReguladorBT(void);
   };
             
#endif
//---------------------------------------------------------------------------
//eof

