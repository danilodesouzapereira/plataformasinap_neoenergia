//---------------------------------------------------------------------------
#ifndef TPonto16H
#define TPonto16H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TPonto.h"

//---------------------------------------------------------------------------
class TPonto16 : public TPonto
   {
   public:  //funções públicas
               __fastcall  TPonto16(void);
               __fastcall ~TPonto16(void);
      VTPonto* __fastcall  Clone(int num_val);

   private: //dados locais
      float valor[16];
   };

//---------------------------------------------------------------------------
#endif
//eof
