//---------------------------------------------------------------------------
#ifndef TListeqpH
#define TListeqpH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTListeqp.h"

//---------------------------------------------------------------------------
//class

//---------------------------------------------------------------------------
class TListeqp : public VTListeqp
   {
   public:
               __fastcall  TListeqp(void);
               __fastcall ~TListeqp(void);
      int      __fastcall  Add(VTEqpto *eqpto);
      VTEqpto* __fastcall  ExisteEqpto(int eqpto_id);
      bool     __fastcall  Ordena(void);

   private: //métodos
      VTEqpto* __fastcall  BuscaBinaria(int eqpto_id);

   private:  //dados
      bool ordenada;
   };

#endif
//---------------------------------------------------------------------------
//eof
