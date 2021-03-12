//---------------------------------------------------------------------------
#ifndef VTEqbarH
#define VTEqbarH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "Estrutura.h"
#include "VTEqpto.h"

//---------------------------------------------------------------------------
class VTBarra;

//---------------------------------------------------------------------------
class VTEqbar : public VTEqpto
   {
   public:
                       __fastcall VTEqbar(void) {};
      virtual          __fastcall ~VTEqbar(void) {};
      virtual VTEqbar* __fastcall Clone(void) = 0;
      virtual void     __fastcall DefineObjBarra(VTBarra *pbar) = 0;
      virtual void     __fastcall DefinePosicao(int x, int y) = 0;

   public:  //dados externos
      VTBarra *pbarra;

   public:  //dados locais
      strESQ_EQBAR esq;  //informações p/ diagrama esquemático
   };

//---------------------------------------------------------------------------
#endif
//eof

