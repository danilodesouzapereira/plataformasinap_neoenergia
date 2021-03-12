//---------------------------------------------------------------------------
#ifndef VTLixeiraH
#define VTLixeiraH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class VTLixeira : public TObject
   {
   public:
                     __fastcall VTLixeira(void) {};
      virtual        __fastcall ~VTLixeira(void) {};
      virtual void   __fastcall Clear(void) = 0;
      virtual void   __fastcall ClearEqptoNovo(void) = 0;
      virtual void   __fastcall InsereEqpto(VTEqpto *eqpto) = 0;
      virtual TList* __fastcall LisEqpto(void) = 0;
      virtual void   __fastcall LisEqpto(TList *lisEQP, int tipo) = 0;
      virtual void   __fastcall RemoveEqpto(VTEqpto *eqpto) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTLixeira
//---------------------------------------------------------------------------
VTLixeira* __fastcall NewObjLixeira(void);

//---------------------------------------------------------------------------
#endif
//eof



 