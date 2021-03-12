//---------------------------------------------------------------------------
#ifndef TPonto8H
#define TPonto8H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TPonto.h"

//---------------------------------------------------------------------------
class TPonto8 : public TPonto
   {
   public:  //funções públicas
               __fastcall  TPonto8(void);
               __fastcall ~TPonto8(void);
      VTPonto* __fastcall  Clone(int num_val);

   private: //dados locais
      float valor[8];
   };

//---------------------------------------------------------------------------
#endif
//eof
