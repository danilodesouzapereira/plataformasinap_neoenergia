//---------------------------------------------------------------------------
#ifndef TSinapChildH
#define TSinapChildH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//#include <System.SyncObjs.hpp>
#include <set>
#include "VTSinapChild.h"

//---------------------------------------------------------------------------
class TFormStk;
class VTEqpto;

//---------------------------------------------------------------------------
class TSinapChild : public VTSinapChild
   {
	public:  //m�todos
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
			//acoes envolvendo o Editor
		void __fastcall  BeginEditorBlock(TObject *obj);
		void __fastcall  EndEditorBlock(TObject *obj);
		void __fastcall  HabilitaEditor(bool habilitado);
		void __fastcall  InsereBotaoEditor(TControl *botao);
		void __fastcall  RemoveBotaoEditor(TControl *botao);

   private: //m�todos
      void __fastcall HabilitaBotoesEditor(bool habilitado);
      void __fastcall TrataAlteracaoPatamares(int num_pat);
		void __fastcall TrataAlteracaoPatamaresEqpto(VTEqpto *eqpto, int num_pat);
      void __fastcall TrataAlteracaoPatamaresEstudo(int num_pat);
      void __fastcall TrataAlteracaoPatamaresReducao(int num_pat);
      void __fastcall TrataAlteracaoPatamaresRede(int num_pat);

	private:	//objetos externos
		VTApl *apl;

	private: //dados locais
		TFormStk *FormStk;
        bool     editorBlocked;
		TList    *lisEQP;
		TList    *lisBOTAOEDITOR;
        std::set<void *> semaforo;
	};

//---------------------------------------------------------------------------
#endif
//eof
