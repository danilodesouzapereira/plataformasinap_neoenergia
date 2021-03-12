//---------------------------------------------------------------------------
#ifndef TDrawTopoH
#define TDrawTopoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TDrawEditor.h"

//---------------------------------------------------------------------------
class VTLegenda;
class VTEqbar;

//---------------------------------------------------------------------------
class TDrawTopo : public TDrawEditor
   {
   public:
      __fastcall TDrawTopo(VTApl *apl);
      __fastcall ~TDrawTopo(void);

   protected: //m�todo abstrato na classe b�sica
      void __fastcall MontaLegenda(VTEqpto *eqpto, int x, int y, TColor cor);

   private: //dados locais
      VTLegenda   *legenda;
   };

//---------------------------------------------------------------------------
#endif
//eof
