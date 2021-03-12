//---------------------------------------------------------------------------

#ifndef TFormFinalizaObraH
#define TFormFinalizaObraH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTObra;
class VTMultiObraBO;
class VTMultiObra;
class VTItemObra;
class VTNode;
class VTNodeDetalhe;
class VTObras;

//---------------------------------------------------------------------------
class TFormFinalizaObra : public TForm
{
__published:	// IDE-managed Components
    TAction *ActionSalvaFecha;
	TAction *ActionConfirma;
	TAction *ActionConfirmaItem;
	TAction *ActionDetalharMais;
	TAction *ActionDetalharMenos;
	TActionList *ActionList1;
	TCheckBox *CheckBoxQualidade;
	TEdit *EditNome;
	TEdit *EditPNT;
	TEdit *EditTaxaInsercao;
	TEdit *EditTaxaSubstituicao;
	TGroupBox *GBoxItems;
	TGroupBox *GBoxQualidade;
	TGroupBox *GBoxTaxaFalha;
	TGroupBox *GroupBoxDetalhes;
	TGroupBox *gboxPNT;
	TImageList *ImageList1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *LabelNome;
	TPanel *PanelEsq;
	TPanel *PanelDir;
	TPanel *Panel3;
	TPanel *Panel4;
	TPanel *Panel5;
	TPanel *PanelAriesDir;
	TPanel *PanelComum;
	TPanel *PanelEmpresaDir;
	TPanel *PanelEmpresaEsq;
	TPanel *PanelNome;
	TSplitter *Splitter1;
	TToolBar *ToolBarItem;
	TToolButton *ToolButtonConfirma;
	TToolButton *butCancela;
	TToolButton *butConfirma;
	TTreeView *TreeViewItems;
TToolBar *ToolBarForm;
	TGroupBox *GBoxNomeItem;
	TPanel *PanelId;
	TEdit *EditNomeItem;
	TAction *ActionEdtExcluir;
	TToolButton *ToolButtonExcluir;
	TPanel *PanelAriesEsq;
	TImageList *ImageListEstado;
	TToolButton *butHabilitaEdt;
	TAction *ActionHabilitaEdt;
	TToolBar *ToolBarDetalhamento;
	TLabel *Label5;
	TToolButton *ToolButtonDetMais;
	TToolButton *ToolButtonDetMenos;
    TToolButton *ToolButton1;
    TAction *ActionCancela;
	void __fastcall TreeViewItemsClick(TObject *Sender);
	void __fastcall ActionDetalharMaisExecute(TObject *Sender);
	void __fastcall ActionDetalharMenosExecute(TObject *Sender);
	void __fastcall EditPNTChange(TObject *Sender);
	void __fastcall EditPNTClick(TObject *Sender);
	void __fastcall CheckBoxQualidadeClick(TObject *Sender);
	void __fastcall EditTaxaSubstituicaoClick(TObject *Sender);
	void __fastcall ActionConfirmaItemExecute(TObject *Sender);
	void __fastcall ActionEdtExcluirExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall TreeViewItemsAdvancedCustomDrawItem(TCustomTreeView *Sender, TTreeNode *Node,
          TCustomDrawState State, TCustomDrawStage Stage, bool &PaintImages,
          bool &DefaultDraw);
	void __fastcall EditTaxaSubstituicaoExit(TObject *Sender);
	void __fastcall EditTaxaInsercaoExit(TObject *Sender);
	void __fastcall EditTaxaInsercaoClick(TObject *Sender);
	void __fastcall EditPNTExit(TObject *Sender);
	void __fastcall CheckBoxQualidadeExit(TObject *Sender);
	void __fastcall ActionHabilitaEdtExecute(TObject *Sender);
	void __fastcall ActionSalvaFechaExecute(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TreeViewItemsExpanded(TObject *Sender, TTreeNode *Node);
	void __fastcall TreeViewItemsCollapsed(TObject *Sender, TTreeNode *Node);
	void __fastcall TreeViewItemsDeletion(TObject *Sender, TTreeNode *Node);
	void __fastcall TreeViewItemsCustomDrawItem(TCustomTreeView *Sender, TTreeNode *Node,
          TCustomDrawState State, bool &DefaultDraw);
    void __fastcall ActionCancelaExecute(TObject *Sender);


	public:
		__property TPanel* 	PnlAriesDir	  = {read=PanelAriesDir};
		__property TPanel* 	PnlAriesEsq	  = {read=PanelAriesEsq};
		__property TPanel* 	PnlEmpresaDir = {read=PanelEmpresaDir};
		__property TPanel* 	PnlEmpresaEsq = {read=PanelEmpresaEsq};
		__property bool 	MontantePNT   = {read=PM_GetMontantePNT, write=PM_SetMontantePNT};
		__property bool 	ARIES	      = {read=PM_GetARIES, write=PM_SetARIES};
		__property bool 	Empresa       = {read=PM_GetEmpresa , write=PM_SetEmpresa};

	public:		// User declarations
				__fastcall TFormFinalizaObra(TComponent* Owner, VTApl *apl_owner, VTObra *obra);
	   virtual	__fastcall ~TFormFinalizaObra(void);

	protected:	// User declarations
				void   __fastcall AtualizaItemObraRef(VTNodeDetalhe* nodeDet);
                void   __fastcall AtualizaIconesTreeView(void);
				TColor __fastcall CorPanelDir(VTNodeDetalhe* nodeDet);
				void   __fastcall ClonaItemObraParaEdicaoTemp(VTItemObra *item);
				void   __fastcall ConfirmaItem(void);
		virtual void   __fastcall ConfirmaItemProprio(void);
				bool   __fastcall ConfirmaObra(void);
		virtual bool   __fastcall ConfirmaObraPropria(void);
				void   __fastcall DeletaItemObraRef(void);
		virtual void   __fastcall EnableDetalheItem(bool enable);
				double __fastcall GetValorDouble(TEdit *Edit, AnsiString descricao);
				int  __fastcall FormHeight(void);
				int  __fastcall FormHeightDir(void);
				int  __fastcall FormHeightEsq(void);
				int  __fastcall Height(TWinControl *control);
				int  __fastcall ImageIndex(VTNodeDetalhe *nodeDet);
				VTNodeDetalhe* __fastcall NodeDetalheSelecionado(void);
                void __fastcall PanelComumAjuste(void);
		virtual bool __fastcall PreencheInfoObra(VTObra *obra);
		virtual bool __fastcall PreencheInfoItemObra(VTNodeDetalhe *nodeDet);
				void __fastcall RemoveCaptionsPanels(void);
                void __fastcall SelecionaNodeRaiz(void);
				void __fastcall TreeViewInicia(void);
//				void __fastcall TrocaEstadoForm(VTNodeDetalhe* nodeDet, bool podeExcluir);
				void __fastcall TrocaEstadoForm(void);
				TTreeNode* __fastcall TreeViewInsereNode(TTreeNode *pai, VTNode *node);

		//Validação dos input
        		bool __fastcall ValidaDouble(AnsiString string, double &valor);
		virtual bool __fastcall ValidaInputObra(void);
		virtual bool __fastcall ValidaInputItemObra(void);
				bool __fastcall ValidaNome(AnsiString nomeItem);
                bool __fastcall ValidaPNT(double &valor);
				bool __fastcall ValidaTaxaInsercao(double &valor);
				bool __fastcall ValidaTaxaSubstituicao(double &valor);

    private:
        void __fastcall ClonaTodosItensObra(void);
        void __fastcall CopiaItensObraOriginal(void);

	protected:   // User declarations
	   AnsiString __fastcall GetNome(void);
	   double     __fastcall GetReducaoPNT(void);

	protected:  //métodos acessados via property
		//getters
		bool __fastcall PM_GetMontantePNT(void);
		bool __fastcall PM_GetARIES(void);
		bool __fastcall PM_GetEmpresa(void);
		//setters
		void __fastcall PM_SetMontantePNT(bool showPnt);
		void __fastcall PM_SetARIES(bool showAries);
		void __fastcall PM_SetEmpresa(bool showEmpresa);

	protected: //objetos externos
	   VTApl         *apl;
	   VTObra        *obra_ref;
	   VTMultiObraBO *multiObraBO;

	protected: //objetos próprios
		double   valorPNT;
		VTItemObra *itemObra_ref;
		VTMultiObra *multiObra_ref;
        TList *lisItemObra_orig;

		struct
		{
			bool pnt;
			bool empresa;
			bool aries;
		}cfg;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormFinalizaObra *FormFinalizaObra;
//---------------------------------------------------------------------------
#endif
