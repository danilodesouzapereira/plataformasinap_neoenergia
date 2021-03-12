//---------------------------------------------------------------------------
#ifndef TBancoH
#define TBancoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBanco.h"

//---------------------------------------------------------------------------
class TBanco : public VTBanco
   {
   public:
               __fastcall  TBanco(VTApl *apl);
               __fastcall ~TBanco(void);
      void     __fastcall  DefineCoordenadasBarraNova(void);
		int      __fastcall  NumUnidades(void);
		double   __fastcall  Snom_mvar(void);

   private: //métodos

   private: //objetos externos
      VTApl *apl;
   };

#endif
//---------------------------------------------------------------------------
//eof

