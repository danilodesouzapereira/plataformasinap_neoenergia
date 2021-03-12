//---------------------------------------------------------------------------
#ifndef TAcaoRetiraH
#define TAcaoRetiraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAcao.h"

//---------------------------------------------------------------------------
class TAcaoRetira : public VTAcao
   {
   public:
                 __fastcall TAcaoRetira(VTEqpto *eqpto);
                 __fastcall ~TAcaoRetira(void);
      VTEqpto*   __fastcall Eqpto(void);
      VTEqpto*   __fastcall CopiaDoEqptoOriginal(void);
      VTEqpto*   __fastcall CopiaDoEqptoAlterado(void);
      int        __fastcall Tipo(void);
      AnsiString __fastcall TipoAsString(void);

   protected:  //métodos acessados via property
      VTAcao* __fastcall PM_GetPai(void);
      void    __fastcall PM_SetPai(VTAcao *pai);

   private: //dados externos
      VTEqpto *eqpto;
      struct   {
               VTEqpto *original;
               } copia;
 	};

#endif
//---------------------------------------------------------------------------
//eof

