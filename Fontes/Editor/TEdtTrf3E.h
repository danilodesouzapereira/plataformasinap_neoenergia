//---------------------------------------------------------------------------
#ifndef TEdtTrf3EH
#define TEdtTrf3EH
#include "TEdt.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;
class VTTrafo3E;

//---------------------------------------------------------------------------
class TEdtTrf3E : public TEdt
   {
   public:
           __fastcall TEdtTrf3E (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
           __fastcall ~TEdtTrf3E (void);
      void __fastcall EvtMouseDownMove(void);
      void __fastcall EvtMouseUp(void);
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtMouseMove(void);
   
   private: //metodos
      VTBarra*  __fastcall BuscaBarra(void);
      void      __fastcall CancelaSelecao(void);
      bool      __fastcall ConfirmaLigacao(void);
      void      __fastcall CriaLigacao(void);
      void      __fastcall InsereLigacao(void);
      void      __fastcall OrdenaBarras(void);

   private: //dados
      enum     enumEST {estLig1Bar1=0, estLig1Bar2, estLig2Bar1, estLig2Bar2};
      int      estado;
      struct   {
               VTBarra   *pbar;
               VTLigacao *pliga;
               } tmp;
      VTBarra   *pbar1;
      VTTrafo3E *ptrf;
   };

#endif
//---------------------------------------------------------------------------
//eof

