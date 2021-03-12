//---------------------------------------------------------------------------
#ifndef TDrawCurtoH
#define TDrawCurtoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TDrawEditor.h"

//---------------------------------------------------------------------------
class VTEqpto;
class VTRedeCC;
class VTResCC;

//---------------------------------------------------------------------------
class TDrawCurto : public TDrawEditor
   {
   public:
      __fastcall TDrawCurto(VTApl *apl);
      __fastcall ~TDrawCurto(void);

   protected: //método abstrato na classe básica
      void __fastcall MontaLegenda(VTEqpto *eqpto, int x, int y, TColor cor);

   private:   //dados locais
      VTRedeCC *redeCC;
      VTResCC  *resCC;
   };

//---------------------------------------------------------------------------
#endif
//eof
