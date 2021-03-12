//---------------------------------------------------------------------------
#ifndef TUnifConfiabH
#define TUnifConfiabH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TUnifEditor.h"

//---------------------------------------------------------------------------
class VTBloco;
class VTRedeCFB;

//---------------------------------------------------------------------------
class TUnifConfiab : public TUnifEditor
   {
   public:
           __fastcall TUnifConfiab(VTApl *apl, int tipo_graf, AnsiString nome);
			  __fastcall ~TUnifConfiab(void);
      void __fastcall Redraw(VTEdt *editor);
      void __fastcall Redraw(int op, VTEdt *editor);

	private: //métodos redefinidos
	 TColor __fastcall CorBarraConfiab(VTBarra *pbarra);
	 TColor __fastcall CorBlocoConfiab(VTBloco *pbloco);
		void __fastcall RedrawAllBarra(void);
		void __fastcall RedrawAllLigacao(bool chave_enabled=true);
		void __fastcall RedrawAllEqbar(void);

	private: //objetos externos
		VTRedeCFB *redeCC;
	};

#endif
//---------------------------------------------------------------------------
//eof
