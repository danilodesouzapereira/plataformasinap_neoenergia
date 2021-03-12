//---------------------------------------------------------------------------
#ifndef TPonto2H
#define TPonto2H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TPonto.h"

//---------------------------------------------------------------------------
class TPonto2 : public TPonto
   {
   public:  //funções públicas
               __fastcall  TPonto2(void);
               __fastcall ~TPonto2(void);
      VTPonto* __fastcall  Clone(int num_val);

   private: //dados locais
      float valor[2];
   };

//---------------------------------------------------------------------------
#endif
//eof
