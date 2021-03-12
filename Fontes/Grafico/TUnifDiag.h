//---------------------------------------------------------------------------
#ifndef TUnifDiagH
#define TUnifDiagH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TUnifEditor.h"

//---------------------------------------------------------------------------
class VTDiagRede;

//---------------------------------------------------------------------------
class TUnifDiag : public TUnifEditor
   {
   public:
           __fastcall  TUnifDiag(VTApl *apl, int tipo_graf, AnsiString nome);
			  __fastcall ~TUnifDiag(void);
      void __fastcall  Redraw(VTEdt *editor);
      void __fastcall  Redraw(int op, VTEdt *editor);

	private: //métodos redefinidos
		void __fastcall RedrawAllBarra(void);
		void __fastcall RedrawAllLigacao(bool chave_enabled=true);
		void __fastcall RedrawAllEqbar(void);
		void __fastcall RedrawAllMutua(void);

   private: //objetos externos
      VTDiagRede *diag_rede;
	};

#endif
//---------------------------------------------------------------------------
//eof
