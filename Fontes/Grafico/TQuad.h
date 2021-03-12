//---------------------------------------------------------------------------
#ifndef TQuadH
#define TQuadH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTQuad.h"

//---------------------------------------------------------------------------
class TQuad : public VTQuad
   {
   public: //métodos
             __fastcall  TQuad(void);
             __fastcall  TQuad(int xa, int ya, int xb, int yb);
             __fastcall ~TQuad(void);
      bool   __fastcall  ContemPonto(int x, int y);
      void   __fastcall  CopiaAtributosDe(VTQuad &quad);
      int    __fastcall  Height(void);
      bool   __fastcall  Igual(VTQuad *quad);
      int    __fastcall  Width(void);
   };

//---------------------------------------------------------------------------
#endif
//eof

