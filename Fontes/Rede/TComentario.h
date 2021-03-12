//---------------------------------------------------------------------------
#ifndef TComentarioH
#define TComentarioH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTComentario.h"

//---------------------------------------------------------------------------
class TComentario : public VTComentario
   {
   public:
                    __fastcall  TComentario(void);
                    __fastcall ~TComentario(void);
      VTComentario* __fastcall  Clone(void);
      void          __fastcall  CoordenadasEsquematico(int &x, int &y);
      bool          __fastcall  CoordenadasUtm_cm(int &x, int &y);
      void          __fastcall  DefinePosicaoEsquematico(int dx, int dy);
      void          __fastcall  DefinePosicaoUtm_cm(int dx, int dy);

   private: //métodos
      VTComentario& __fastcall operator<<(VTComentario &comentario);

   private: //dados locais
      struct   {
               int dx, dy;
               } pos_esq, pos_utm;
   };

//---------------------------------------------------------------------------
#endif
//eof

