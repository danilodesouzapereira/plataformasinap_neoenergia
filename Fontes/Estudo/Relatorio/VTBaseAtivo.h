//---------------------------------------------------------------------------
#ifndef VTBaseAtivoH
#define VTBaseAtivoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//class VTApl;
class VTAtivo;
class VTAlternativa;

//---------------------------------------------------------------------------
class VTBaseAtivo : public TObject
   {
   public:
                      __fastcall  VTBaseAtivo(void) {};
     virtual          __fastcall ~VTBaseAtivo(void) {};
     virtual void     __fastcall  Inicia(VTAlternativa *alternativa) = 0;
     virtual void     __fastcall  InsereAtivo(VTAtivo *ativo) = 0;
     virtual TList*   __fastcall  LisAtivo(void) = 0;
     virtual int      __fastcall  NumeroDeAtivos(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTBaseAtivo* __fastcall NewObjBaseAtivo(void);

#endif
//---------------------------------------------------------------------------
//eof

