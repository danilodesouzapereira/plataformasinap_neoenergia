//---------------------------------------------------------------------------
#ifndef TSinapChildH
#define TSinapChildH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSinapChild.h"

//---------------------------------------------------------------------------
class TFormStk;
class VTEqpto;

//---------------------------------------------------------------------------
class TSinapChild : public VTSinapChild
   {
	public:  //métodos
			  __fastcall  TSinapChild(VTApl *apl_owner);
			  __fastcall ~TSinapChild(void);
		void __fastcall  DefineArranjos(VTApl *apl_ref);
		void __fastcall  DefineCorTipoChave(VTApl *apl_ext);
		void __fastcall  DefinePatamares(VTApl *apl_ref);
		void __fastcall  DefinePatamares(int escala);
		void __fastcall  FormStkAdd(AnsiString caption, TActionList *ActionList, bool expand=false);
		void __fastcall  FormStkAdd(AnsiString caption, TList *LisActionList, bool expand=false);
		void __fastcall  FormStkAdd(TWinControl *WinControl, TAlign align);
		void __fastcall  FormStkClose(void);
		void __fastcall  FormStkHide(void);
		void __fastcall  FormStkRemove(TActionList *ActionList);
		void __fastcall  FormStkShow(AnsiString caption="");
		void __fastcall  LeBaseCurvaTipica(void);
		void __fastcall  PanelToolBarGraficoAdd(TToolBar *ToolBar);

   private: //métodos
      void __fastcall TrataAlteracaoPatamares(int num_pat);
		void __fastcall TrataAlteracaoPatamaresEqpto(VTEqpto *eqpto, int num_pat);
      void __fastcall TrataAlteracaoPatamaresEstudo(int num_pat);
      void __fastcall TrataAlteracaoPatamaresReducao(int num_pat);
      void __fastcall TrataAlteracaoPatamaresRede(int num_pat);

	private:	//objetos externos
		VTApl *apl;

    private: //dados locais
      TFormStk *FormStk;
		TList    *lisEQP;
	};

//---------------------------------------------------------------------------
#endif
//eof
