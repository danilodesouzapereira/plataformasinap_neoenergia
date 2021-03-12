//---------------------------------------------------------------------------
#ifndef TAcaoAlteraH
#define TAcaoAlteraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAcao.h"

//---------------------------------------------------------------------------
class TAcaoAltera : public VTAcao
   {
   public:
                 __fastcall  TAcaoAltera(VTEqpto *original, VTEqpto *alterado, VTEqpto *copia_original=NULL);
                 __fastcall ~TAcaoAltera(void);
      VTEqpto*   __fastcall  Eqpto(void);
      VTEqpto*   __fastcall  CopiaDoEqptoOriginal(void);
      VTEqpto*   __fastcall  CopiaDoEqptoAlterado(void);
      int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);

   private: //dados externos
      VTEqpto *eqpto;
      struct   {
               VTEqpto *original;
               VTEqpto *alterado;
               } copia;

   };

#endif
//---------------------------------------------------------------------------
//eof

