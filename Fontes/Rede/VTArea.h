//---------------------------------------------------------------------------
#ifndef VTAreaH
#define VTAreaH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTArea : public TObject
   {
   public: //métodos
                     __fastcall  VTArea(void) {};
                     __fastcall  VTArea(double xa, double ya, double xb, double yb) {};
                     __fastcall ~VTArea(void) {};
      virtual bool   __fastcall  ContemPonto(double x, double y) = 0;
		virtual void   __fastcall  CopiaAtributosDe(VTArea &ref) = 0;
      virtual double __fastcall  Height(void) = 0;
      virtual bool   __fastcall  Igual(VTArea *area) = 0;
      virtual double __fastcall  Width(void) = 0;

   public: //dados
      double x1, y1, x2, y2;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTArea
//---------------------------------------------------------------------------
VTArea* __fastcall NewObjArea(void);

//---------------------------------------------------------------------------
#endif
//eof

