//---------------------------------------------------------------------------
#ifndef TUnifReguladorH
#define TUnifReguladorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TUnifEditor.h"

//---------------------------------------------------------------------------
class VTBloco;

//---------------------------------------------------------------------------
class TUnifRegulador : public TUnifEditor
   {
   public:
           __fastcall  TUnifRegulador(VTApl *apl, int tipo_graf, AnsiString nome);
           __fastcall ~TUnifRegulador(void);
      void __fastcall  Redraw(VTEdt *editor);
      void __fastcall  Redraw(int op, VTEdt *editor);

   private: //métodos redefinidos
      void __fastcall RedrawAllBarra(void);
      void __fastcall RedrawAllCarga(void);
      void __fastcall RedrawAllLigacao(bool chave_enabled=true);
   };

#endif
//---------------------------------------------------------------------------
//eof
