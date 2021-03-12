//---------------------------------------------------------------------------
#ifndef TAcaoCompostaH
#define TAcaoCompostaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAcao.h"

//---------------------------------------------------------------------------
class TAcaoComposta : public VTAcao
   {
   public:
                 __fastcall TAcaoComposta(void);
                 __fastcall ~TAcaoComposta(void);
      void       __fastcall Add(VTAcao *acao);
      VTEqpto*   __fastcall Eqpto(void);
      VTEqpto*   __fastcall CopiaDoEqptoOriginal(void);
      VTEqpto*   __fastcall CopiaDoEqptoAlterado(void);
      TList*     __fastcall LisAcao(void);
      int        __fastcall Tipo(void);
      AnsiString __fastcall TipoAsString(void);

   protected:  //métodos acessados via property
      VTAcao* __fastcall PM_GetPai(void);
      void    __fastcall PM_SetPai(VTAcao *pai);

   private: //dados externos
      TList *lisACAO;

   private: //dados acessados via property
      struct   {
               VTAcao *pai;
               }PD;
   };

#endif
//---------------------------------------------------------------------------
//eof

