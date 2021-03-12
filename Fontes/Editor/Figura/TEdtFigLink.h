//---------------------------------------------------------------------------
#ifndef TEdtFigLinkH
#define TEdtFigLinkH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEdt.h"

//---------------------------------------------------------------------------
class VTFiguraComentario;
class VTEqpto;
class VTRedegraf;
class VTBarra;
class VTTrecho;

//---------------------------------------------------------------------------
class TEdtFigLink : public TEdt
   {
   public:
      __fastcall TEdtFigLink(VTApl *apl, TCursor cursor, TAction *ResetFocus,
                             TAction *SetFocus, TAction *OnDone);
      __fastcall ~TEdtFigLink(void);

   public:
      void __fastcall EvtMouseDownMove(void);
      void __fastcall EvtMouseUp(void);
      void __fastcall EvtMouseDown(void);
      void __fastcall EvtMouseMove(void);

   private: //metodos
      VTFiguraComentario* __fastcall BuscaComentario(void);
      VTEqpto*      __fastcall BuscaEqpto(void);
      void          __fastcall InsereFigLink(void);

   protected:  //objetos externos
      VTRedegraf *redegraf;

   protected: //dados locais
      struct{
            VTEqpto      *eqpto;
            VTFiguraComentario *comentario;
            }selecao;
      struct{
            VTBarra  *barra1;
            VTBarra  *barra2;
            VTTrecho *trecho;
            }tmp;
   };

#endif
//---------------------------------------------------------------------------
//eof
