//---------------------------------------------------------------------------
#ifndef VTComentarioH
#define VTComentarioH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
//#include "Estrutura.h"

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class VTComentario : public TObject
   {
   public:  //property
      __property VTEqpto    *Eqpto = {read=PD.eqpto, write=PD.eqpto};
      __property AnsiString Texto  = {read=PD.texto, write=PD.texto};

   public:
                            __fastcall  VTComentario(void) {};
      virtual               __fastcall ~VTComentario(void) {};
      virtual VTComentario* __fastcall  Clone(void) = 0;
      virtual void          __fastcall  CoordenadasEsquematico(int &x, int &y) = 0;
      virtual bool          __fastcall  CoordenadasUtm_cm(int &x, int &y) = 0;
      virtual void          __fastcall  DefinePosicaoEsquematico(int dx, int dy) = 0;
      virtual void          __fastcall  DefinePosicaoUtm_cm(int dx, int dy) = 0;

   protected:  //dados acessados via property
      struct   {
               AnsiString texto;
               VTEqpto    *eqpto;
               }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTComentario
//---------------------------------------------------------------------------
VTComentario* __fastcall NewObjComentario(VTEqpto *eqpto);

//---------------------------------------------------------------------------
#endif
//eof

