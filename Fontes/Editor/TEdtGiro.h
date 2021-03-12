//---------------------------------------------------------------------------
#ifndef TEdtGiroH
#define TEdtGiroH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdt.h"
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTRedegraf;
class VTRedes;

//---------------------------------------------------------------------------
class TEdtGiro : public TEdt
   {
   public:
      __fastcall  TEdtGiro(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
      __fastcall ~TEdtGiro(void);

   private:  //métodos
      VTBarra* __fastcall BuscaBarra(void);
      void     __fastcall EvtMouseDown(void);
      void     __fastcall EvtMouseDownMove(void);
      void     __fastcall EvtMouseMove(void);
      void     __fastcall EvtMouseUp(void);

   private:  //objetos externos
      VTRedegraf *redegraf;
      VTRedes    *redes;

   private:  //dados locais
      VTBarra *barra_ref;
      struct{
            bool enabled;
            }giro;
   };

#endif
//---------------------------------------------------------------------------
//eof
