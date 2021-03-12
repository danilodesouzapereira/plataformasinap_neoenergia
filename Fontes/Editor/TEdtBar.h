//---------------------------------------------------------------------------
#ifndef TEdtBarH
#define TEdtBarH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdt.h"
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTRedegraf;

//---------------------------------------------------------------------------
class TEdtBar : public TEdt
   {
   public:
      __fastcall TEdtBar(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
      __fastcall ~TEdtBar(void);

   private:  //métodos
      void __fastcall EditaBarra(void);
      bool __fastcall EstimaCoordEsquematico(strUTM &utm, strESQ_BARRA &esq);
      void __fastcall EstimaCoordUtm(strUTM &utm, strESQ_BARRA &esq);
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtMouseDownMove(void);
      void __fastcall EvtMouseUp(void);
      void __fastcall InsereBarra(void);
      void __fastcall RetiraBarra(void);

   private:  //objetos externos
      VTRedegraf *redegraf;

   private:  //dados locais
      VTBarra *pbarra;
      bool    estado_alfa;
   };

#endif
//---------------------------------------------------------------------------
//eof
