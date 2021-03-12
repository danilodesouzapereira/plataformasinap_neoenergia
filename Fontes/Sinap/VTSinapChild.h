//-----------------------------------------------------------------------------
#ifndef VTSinapChildH
#define VTSinapChildH

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTSinapChild : public TObject
   {
	public:   //property - objetos de TFormChild que podem ser usadas por outros objetos
		__property TPanel*  PanelToolBarGrafico  = {read=PD.PanelToolBarGrafico,  write=PD.PanelToolBarGrafico };
		__property TToolButton*  ButFlow  		 = {read=PD.ButFlow,  write=PD.ButFlow };

	public:   //property - Actions de TFormChild que podem ser usadas por outros objetos
		__property TAction* ActionCurto                = {read=PD.ActionCurto,          write=PD.ActionCurto         };
		__property TAction* ActionEditor               = {read=PD.ActionEditor,         write=PD.ActionEditor        };
		__property TAction* ActionFlow                 = {read=PD.ActionFlow,           write=PD.ActionFlow          };
		__property TAction* ActionTopo                 = {read=PD.ActionTopo,           write=PD.ActionTopo          };
		__property TAction* ActionRedeChanged          = {read=PD.ActionRedeChanged,    write=PD.ActionRedeChanged   };
		__property TAction* ActionSalva                = {read=PD.ActionSalva,          write=PD.ActionSalva         };
		__property TAction* ActionSalvaComo            = {read=PD.ActionSalvaComo,      write=PD.ActionSalvaComo     };
		__property TAction* ActionSalvarBaseConvertida = {read=PD.ActionSalvarBaseConvertida, write=PD.ActionSalvarBaseConvertida     };
		__property TAction* ActionMercadoChanged       = {read=PD.ActionMercadoChanged, write=PD.ActionMercadoChanged};

	public:   //property - Actions que podem ser definidas por outros objetos para uso de TFormChild (ou outros objetos)
		__property TAction* ActionRedeCarregada  = {read=PD.ActionRedeCarregada,  write=PD.ActionRedeCarregada };

	public:
						 __fastcall  VTSinapChild(void){};
						 __fastcall ~VTSinapChild(void){};
		virtual void __fastcall  DefineArranjos(VTApl *apl_ref) = 0;
		virtual void __fastcall  DefineCorTipoChave(VTApl *apl_ref) = 0;
		virtual void __fastcall  DefinePatamares(VTApl *apl_ref) = 0;
		virtual void __fastcall  DefinePatamares(int escala) = 0;
		virtual void __fastcall  FormStkAdd(AnsiString caption, TActionList *ActionList, bool expand=false) = 0;
		virtual void __fastcall  FormStkAdd(AnsiString caption, TList *LisActionList, bool expand=false) = 0;
		virtual void __fastcall  FormStkAdd(TWinControl *WinControl, TAlign align) = 0;
		virtual void __fastcall  FormStkClose(void) = 0;
		virtual void __fastcall  FormStkHide(void) = 0;
		virtual void __fastcall  FormStkRemove(TActionList *ActionList) = 0;
		virtual void __fastcall  FormStkShow(AnsiString caption="") = 0;
		virtual void __fastcall  LeBaseCurvaTipica(void) = 0;
		virtual void __fastcall  PanelToolBarGraficoAdd(TToolBar *ToolBar) = 0;

		//acoes envolvendo o Editor
		virtual void __fastcall  BeginEditorBlock(TObject *obj) = 0;
		virtual void __fastcall  EndEditorBlock(TObject *obj) = 0;
		virtual void __fastcall  HabilitaEditor(bool habilitado) = 0;
		virtual void __fastcall  InsereBotaoEditor(TControl *botao) = 0;
		virtual void __fastcall  RemoveBotaoEditor(TControl *botao) = 0;

	protected:   //objetos externos acessados via property
      struct   {//objetos de TFormChild
				TPanel  *PanelToolBarGrafico;
				TToolButton *ButFlow;
			   //Actions de TFormChild
			   TAction *ActionCurto;
			   TAction *ActionEditor;
			   TAction *ActionFlow;
			   TAction *ActionTopo;
			   TAction *ActionRedeChanged;
			   TAction *ActionSalva;
			   TAction *ActionSalvaComo;
			   TAction *ActionSalvarBaseConvertida;
			   TAction *ActionMercadoChanged;
			   //Actions externas
			   TAction *ActionRedeCarregada;
			   }PD;
	 };

//-----------------------------------------------------------------------------
VTSinapChild* __fastcall NewObjSinapChild(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
