//---------------------------------------------------------------------------
#ifndef TPonto4H
#define TPonto4H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TPonto.h"

//---------------------------------------------------------------------------
class TPonto4 : public TPonto
   {
   public:  //funções públicas
               __fastcall  TPonto4(void);
               __fastcall ~TPonto4(void);
      VTPonto* __fastcall  Clone(int num_val);

   private: //dados locais
      float valor[4];
   };

//---------------------------------------------------------------------------
#endif
//eof
