//---------------------------------------------------------------------------
#ifndef TAcaoInsereH
#define TAcaoInsereH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAcao.h"

//---------------------------------------------------------------------------
class TAcaoInsere : public VTAcao
   {
   public:
                 __fastcall TAcaoInsere(VTEqpto *eqpto);
                 __fastcall ~TAcaoInsere(void);
      VTEqpto*   __fastcall Eqpto(void);
      VTEqpto*   __fastcall CopiaDoEqptoOriginal(void);
      VTEqpto*   __fastcall CopiaDoEqptoAlterado(void);
      int        __fastcall Tipo(void);
      AnsiString __fastcall TipoAsString(void);

   private: //dados externos
      VTEqpto *eqpto;
      struct   {
               VTEqpto *original;
               } copia;
   };

#endif
//---------------------------------------------------------------------------
//eof

