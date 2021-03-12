//---------------------------------------------------------------------------
#ifndef TDrawFlowH
#define TDrawFlowH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TDrawEditor.h"

//---------------------------------------------------------------------------
class VTLegenda;
class VTEqbar;

//---------------------------------------------------------------------------
class TDrawFlow : public TDrawEditor
   {
   public:
      __fastcall TDrawFlow(VTApl *apl);
      __fastcall ~TDrawFlow(void);

   protected: //método abstrato na classe básica
      void __fastcall MontaLegenda(VTEqpto *eqpto, int x, int y, TColor cor);

   private: //dados locais
      VTLegenda   *legenda;
   };

//---------------------------------------------------------------------------
#endif
//eof
