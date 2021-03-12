//---------------------------------------------------------------------------
#ifndef TEdtFigRetanguloH
#define TEdtFigRetanguloH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Rede\Estrutura.h>
#include <Fontes\Figura\VTRetangulo.h>
#include "TEdt.h"

//---------------------------------------------------------------------------
class VTRedegraf;
class VTFiguras;
//---------------------------------------------------------------------------
class TEdtFigRetangulo : public TEdt
   {
   public:
            __fastcall  TEdtFigRetangulo(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
            __fastcall ~TEdtFigRetangulo(void);

   private:  //métodos
      void __fastcall EditaRetangulo(void);
      bool __fastcall EstimaCoordEsquematico(strUTM &utm, strCOORD &esq);
      void __fastcall EstimaCoordUtm(strCOORD &esq, strUTM &utm);
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtSelArea(int x1, int y1, int x2, int y2);
      void __fastcall InsereRetangulo(void);
      void __fastcall RetiraRetangulo(void);

   private:  //objetos externos
	  VTRedegraf *redegraf;
	  VTFiguras  *figuras;

   private:  //dados locais
      VTRetangulo *retangulo;
   };

#endif
//---------------------------------------------------------------------------
//eof
