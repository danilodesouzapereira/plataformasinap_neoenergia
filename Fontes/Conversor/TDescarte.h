//---------------------------------------------------------------------------
#ifndef TDescarteH
#define TDescarteH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TDescarte
   {
   public:
           __fastcall  TDescarte(VTApl *apl);
           __fastcall ~TDescarte(void);
      void __fastcall  Arranjos(int num_arranjos);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TList *lisEQP;
   };

//---------------------------------------------------------------------------
#endif
//eof
