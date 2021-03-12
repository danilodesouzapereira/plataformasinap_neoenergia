//---------------------------------------------------------------------------
#ifndef TMontaZH
#define TMontaZH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMontaZ.h"

//---------------------------------------------------------------------------
class VTMatC;
class VTMatZ;

//---------------------------------------------------------------------------
class TMontaZ : public VTMontaZ
   {
   public:
           __fastcall TMontaZ(VTApl *apl);
           __fastcall ~TMontaZ(void);
      bool __fastcall Executa(TList *lisLIG);
      bool __fastcall Executa(VTArranjo *arranjo);
      bool __fastcall Executa(VTTrafo *trafo);
      bool __fastcall Executa(VTTramo *tramo);
      bool __fastcall Executa(VTTrecho *trecho);

   private:
      bool __fastcall MontaMatC(VTMatC *matC, double c0, double c1);
      bool __fastcall MontaMatZ(VTArranjo *arranjo);
      void __fastcall MontaMatZ(VTMatZ *matZ, double r0, double x0, double r1, double x1);

   private: //objetos externos
      VTApl *apl;
   };

//---------------------------------------------------------------------------
#endif
//eof
