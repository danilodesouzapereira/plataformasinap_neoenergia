//---------------------------------------------------------------------------
#ifndef TPontoGH
#define TPontoGH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TPonto.h"

//---------------------------------------------------------------------------
class TPontoG : public TPonto
   {
   public:  //funções públicas
               __fastcall  TPontoG(int num_val);
               __fastcall ~TPontoG(void);
      VTPonto* __fastcall  Clone(int num_val);

   private: //dados locais
      float *valor;
   };

//---------------------------------------------------------------------------
#endif
//eof
