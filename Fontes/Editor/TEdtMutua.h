//---------------------------------------------------------------------------
#ifndef TEdtMutuaH
#define TEdtMutuaH

//---------------------------------------------------------------------------
#include "TEdt.h"

//---------------------------------------------------------------------------
class VTMutua;
class VTBarra;
class VTTrecho;

//---------------------------------------------------------------------------
class TEdtMutua: public TEdt
   {
   public:     //métodos
           __fastcall TEdtMutua(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtMutua(void);
      void __fastcall EvtMouseDownMove(void);
      void __fastcall EvtMouseUp(void);
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtMouseMove(void);

   private:
      VTTrecho* __fastcall BuscaTrecho(void);
      void      __fastcall CancelaSelecao(void);
      bool      __fastcall ConfirmaMutua(void);
      void      __fastcall RedefineCoordenadasMutua(void);

   private:
      VTTrecho  *ptre1, *ptre2;
      struct   {//objetos temporários
               VTTrecho *ptre;
               VTBarra  *pbar;
               VTMutua  *mutua;
               }tmp;
   };

#endif
//---------------------------------------------------------------------------
//eof

