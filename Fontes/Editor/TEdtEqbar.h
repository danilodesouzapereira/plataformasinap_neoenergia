//---------------------------------------------------------------------------
#ifndef TEdtEqbarH
#define TEdtEqbarH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdt.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTEqbar;

//---------------------------------------------------------------------------
class TEdtEqbar: public TEdt
   {
   public:     //métodos
                   __fastcall TEdtEqbar(VTApl *apl, TCursor cursor, TAction *ResetFocus,
                                        TAction *SetFocus, TAction *OnDone);
      virtual      __fastcall ~TEdtEqbar (void);
      virtual void __fastcall CriaEqbar(void) = 0;
      virtual bool __fastcall ConfirmaEqbar(void) = 0;

   private:
      void __fastcall CancelaSelecao(void);
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtMouseDownMove(void);
      void __fastcall EvtMouseMove(void);
      void __fastcall EvtMouseUp(void);
      void __fastcall InsereEqbar(void);
      void __fastcall NaoInsereEqbar(void);
      void __fastcall ReiniciaEditor(void);
      
   protected:  //atributos
      enum    enumEST {SelecaoBarra = 0, PosicaoRelativa};
      int     estado;
      VTBarra *pbarra;
      VTEqbar *eqbar;
      TList   *lisBAR;
   };

#endif
//---------------------------------------------------------------------------
//eof
