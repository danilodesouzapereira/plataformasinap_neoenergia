//---------------------------------------------------------------------------
#ifndef TEdtCopiaH
#define TEdtCopiaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdt.h"

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class TEdtCopia : public TEdt
   {
   public:
      __fastcall TEdtCopia(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
      __fastcall ~TEdtCopia(void);

   private:  //métodos
      void __fastcall AtualizaDragdrop(void);
      bool __fastcall BuscaEqpto(void);
      bool __fastcall BuscaEqpto(int x1, int y1, int x2, int y2);
      void __fastcall CancelaSelecao(void);
      void __fastcall EvtEscape(void);
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtMouseDownMove(void);
      void __fastcall EvtMouseMove(void);
      void __fastcall EvtMouseUp(void);
      void __fastcall EvtSelArea(int x1, int y1, int x2, int y2);
		void __fastcall EvtSelEqpto(TList *lisEXT);
		bool __fastcall InsereEqptoSel(VTEqpto *eqpto);

   private:  //dados
      VTEqpto *eqpto;
      TList   *lisBAR;
   };

#endif
//---------------------------------------------------------------------------
//eof
