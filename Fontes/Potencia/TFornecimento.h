//---------------------------------------------------------------------------
#ifndef TFornecimentoH
#define TFornecimentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TForBasico.h"

//---------------------------------------------------------------------------
class TFornecimento : public TForBasico
   {
   public:
      __fastcall  TFornecimento(VTApl *apl);
      __fastcall ~TFornecimento(void);

   protected: //métodos a serem implementados em classes derivadas
      bool __fastcall GetPQVTNo(VTNo *no, int np, strSUP vetor[MAX_FASE]);
   };

#endif
//---------------------------------------------------------------------------
//eof
