//---------------------------------------------------------------------------
#ifndef TReduzH
#define TReduzH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTReduz.h"

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TReduz : public VTReduz
   {
   public:  //métodos
                 __fastcall  TReduz(VTApl *apl_owner);
                 __fastcall ~TReduz(void);
      bool       __fastcall  Executa(void);

   private:  //métodos
      bool __fastcall ReduzRamal(void);
      bool __fastcall ReduzSecao(void);

   private:  //objetos externos
      VTApl       *apl;

   private: //dados locais
   };

#endif
//---------------------------------------------------------------------------
//eof
