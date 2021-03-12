//---------------------------------------------------------------------------
#ifndef TUnifTopobpH
#define TUnifTopobpH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TUnifEditor.h"

//---------------------------------------------------------------------------
class VTEqbar;
class VTTrecho;

//---------------------------------------------------------------------------
class TUnifTopobp : public TUnifEditor
   {
   public:
           __fastcall  TUnifTopobp(VTApl *apl, int tipo_graf, AnsiString nome);
           __fastcall ~TUnifTopobp(void);
      void __fastcall  Redraw(VTEdt *editor);
      void __fastcall  Redraw(int op, VTEdt *editor);

   private: //métodos locais
      TColor __fastcall Color(VTEqpto *eqpto, TColor cor);
      void   __fastcall LimpaTela(void);

   private:
      TColor cor_fundo;
      TColor cor_eqpto;
   };

#endif
//---------------------------------------------------------------------------
//eof
