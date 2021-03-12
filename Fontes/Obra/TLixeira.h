//---------------------------------------------------------------------------
#ifndef TLixeiraH
#define TLixeiraH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTLixeira.h"

//---------------------------------------------------------------------------
class TLixeira : public VTLixeira
   {
   public:
             __fastcall TLixeira(void);
             __fastcall ~TLixeira(void);
      void   __fastcall Clear(void);
      void   __fastcall ClearEqptoNovo(void);
      void   __fastcall InsereEqpto(VTEqpto *eqpto);
      TList* __fastcall LisEqpto(void);
      void   __fastcall LisEqpto(TList *lisEQP, int tipo);
      void   __fastcall RemoveEqpto(VTEqpto *eqpto);

   public:  //dados locais
      TList  *lisLIXO;    //lista de objetos VTEqpto
   };

//---------------------------------------------------------------------------
#endif
//eof
