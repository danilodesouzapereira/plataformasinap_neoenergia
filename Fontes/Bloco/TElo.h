//---------------------------------------------------------------------------
#ifndef TEloH
#define TEloH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTElo.h"

//---------------------------------------------------------------------------
class TElo : public VTElo
   {
   public:
             __fastcall TElo(void);
             __fastcall ~TElo(void);
      bool __fastcall ConectaBloco(VTBloco *bloco);
      bool __fastcall ConectaRede(VTRede *rede);
      bool __fastcall ConectaRede1Rede2(VTRede *rede1, VTRede *rede2);

   protected:  //métodos acessados via property
      VTBloco* __fastcall PM_GetBloco1(void);
      VTBloco* __fastcall PM_GetBloco2(void);
      VTChave* __fastcall PM_GetChave(void);
      void     __fastcall PM_SetBloco1(VTBloco *bloco);
      void     __fastcall PM_SetBloco2(VTBloco *bloco);
      void     __fastcall PM_SetChave(VTChave *chave);

   private: //dados acessados via property
      struct   {
               VTBloco *bloco1;
               VTBloco *bloco2;
               VTChave *chave;
               }PD;
   };

#endif
//---------------------------------------------------------------------------
//eof

