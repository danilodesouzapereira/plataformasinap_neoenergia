//---------------------------------------------------------------------------
#ifndef VTQuadH
#define VTQuadH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTQuad : public TObject
   {
   public: //métodos
                     __fastcall  VTQuad(void) {};
                     __fastcall  VTQuad(int xa, int ya, int xb, int yb) {};
                     __fastcall ~VTQuad(void) {};
      virtual bool   __fastcall  ContemPonto(int x, int y) = 0;
      virtual void   __fastcall  CopiaAtributosDe(VTQuad &quad) = 0;
      virtual int    __fastcall  Height(void) = 0;
      virtual bool   __fastcall  Igual(VTQuad *quad) = 0;
      virtual int    __fastcall  Width(void) = 0;

   public: //dados
      int x1, y1, x2, y2;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTQuad
//---------------------------------------------------------------------------
VTQuad* __fastcall NewObjQuad(void);

//---------------------------------------------------------------------------
#endif
//eof

