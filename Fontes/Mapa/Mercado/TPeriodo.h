//---------------------------------------------------------------------------
#ifndef TPeriodoH
#define TPeriodoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTPeriodo.h"

//---------------------------------------------------------------------------
class TPeriodo : public VTPeriodo
   {
   public:  //funções públicas
           __fastcall TPeriodo(void);
           __fastcall ~TPeriodo(void);
      bool __fastcall ContemAno(int ano);

   protected: //dados acessados via property
      double __fastcall PM_GetValor(void);
      void   __fastcall PM_SetValor(double valor);
   };

//---------------------------------------------------------------------------
#endif
//eof
