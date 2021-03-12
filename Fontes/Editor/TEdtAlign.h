//---------------------------------------------------------------------------
#ifndef TEdtAlignH
#define TEdtAlignH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdt.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTEqpto;
class VTLigacao;
class VTMarca;
class VTRedegraf;

//---------------------------------------------------------------------------
class TEdtAlign : public TEdt
   {
   public:
             __fastcall  TEdtAlign(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
             __fastcall ~TEdtAlign(void);

   private:  //métodos
      void     __fastcall Align(VTLigacao *ligacao, VTBarra *bar_move);
      void     __fastcall AvisoEqptoBloqueado(VTEqpto *eqpto);
      VTEqpto* __fastcall BuscaEqpto(void);
      VTMarca* __fastcall BuscaMarca(void);
      void     __fastcall EvtMouseDown(void);
      void     __fastcall EvtMouseMove(void);
      void     __fastcall MarcaCancela(void);
      void     __fastcall MarcaInicia(VTLigacao *ligacao);
      bool     __fastcall VerificaEqptoBloqueado(VTEqpto *eqpto);

   private:  //objetos externos
       VTRedegraf *redegraf;

   private:  //dados
      VTLigacao *ligacao;       //Ligacao atualmente selecionado
      VTMarca   *marca;
   };

#endif
//---------------------------------------------------------------------------
//eof
