//---------------------------------------------------------------------------
#ifndef TRamaisH
#define TRamaisH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTRamais.h"

//---------------------------------------------------------------------------
//class ;

//---------------------------------------------------------------------------
class TRamais : public VTRamais
   {
   public:
                 __fastcall  TRamais(VTApl *apl);
                 __fastcall ~TRamais(void);
      VTArranjo* __fastcall  ArranjoRamal(double r_ohm_km, int fases);

   private: //métodos
      VTArranjo* __fastcall  ExisteArranjo(double r_ohm_km, int fases);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TList *lisARRANJO;
   };

#endif
//---------------------------------------------------------------------------
//eof

