//---------------------------------------------------------------------------
#ifndef TEdtFigComentarioH
#define TEdtFigComentarioH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Rede\Estrutura.h>
#include <Fontes\Figura\VTFiguraComentario.h>
#include "TEdt.h"

//---------------------------------------------------------------------------
class VTRedegraf;
class VTFiguras;
//---------------------------------------------------------------------------
class EdtFigComentario : public TEdt
   {
   public:
            __fastcall  EdtFigComentario(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
            __fastcall ~EdtFigComentario(void);

   private:  //métodos
      void __fastcall EditaComentario(void);
      bool __fastcall EstimaCoordEsquematico(strUTM &utm, strCOORD &esq);
      void __fastcall EstimaCoordUtm(strCOORD &esq, strUTM &utm);
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtSelArea(int x1, int y1, int x2, int y2);
      void __fastcall InsereComentario(void);
      void __fastcall RetiraComentario(void);

   private:  //objetos externos
	  VTRedegraf 	*redegraf;
	  VTFiguras 	*figuras;

   private:  //dados locais
      VTFiguraComentario *comentario;
   };

#endif
//---------------------------------------------------------------------------
//eof
