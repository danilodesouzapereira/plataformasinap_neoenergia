//---------------------------------------------------------------------------
#ifndef TPonto1H
#define TPonto1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TPonto.h"

//---------------------------------------------------------------------------
class TPonto1 : public TPonto
   {
   public:  //funções públicas
               __fastcall  TPonto1(void);
               __fastcall ~TPonto1(void);
      VTPonto* __fastcall  Clone(int num_val);

   private: //dados locais
      float valor;
   };

//---------------------------------------------------------------------------
#endif
//eof
