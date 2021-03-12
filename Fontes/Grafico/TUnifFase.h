//---------------------------------------------------------------------------
#ifndef TUnifFaseH
#define TUnifFaseH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TUnifEditor.h"

//---------------------------------------------------------------------------
class VTRedeCFB;

//---------------------------------------------------------------------------
class TUnifFase : public TUnifEditor
   {
   public:
			  __fastcall  TUnifFase(VTApl *apl, int tipo_graf, AnsiString nome);
			  __fastcall ~TUnifFase(void);
		void __fastcall  Redraw(VTEdt *editor);
		void __fastcall  Redraw(int op, VTEdt *editor);

	private: //métodos redefinidos
		void __fastcall RedrawAllBarra(void);
		void __fastcall RedrawAllChave(void);
		void __fastcall RedrawAllEqbar(void);
		void __fastcall RedrawAllLigacao(bool chave_enabled=true);
		void __fastcall RedrawAllTrafo(void);
		void __fastcall RedrawAllTrafo3E(void);
		void __fastcall RedrawAllTrecho(void);

	private: //objetos externos
		VTRedeCFB *redeCC;
	};

#endif
//---------------------------------------------------------------------------
//eof
