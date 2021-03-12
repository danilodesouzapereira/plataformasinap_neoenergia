//---------------------------------------------------------------------------
#ifndef TEdicaoH
#define TEdicaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEdicao.h"

//---------------------------------------------------------------------------
class VTBarra;

//---------------------------------------------------------------------------
class TEdicao : public VTEdicao
   {
   public:
           __fastcall  TEdicao(VTApl *apl);
           __fastcall ~TEdicao(void);
      int  __fastcall  Executa(VTEqpto *eqpto, bool criar_copia);
      int  __fastcall  Executa(TList *lisEQP, bool criar_copia);

   private:
      int  __fastcall  EditaEqpto(TList *lisEQP);
      int  __fastcall  EditaTrafo(TList *lisEQP);
      bool __fastcall  ValidaLisEqpto(TList *lisEQP);

   private: //objetos externos
      VTApl *apl;

   private:
      bool  criar_copia;
      TList *lisEQP;
   };

#endif
//---------------------------------------------------------------------------
//eof
