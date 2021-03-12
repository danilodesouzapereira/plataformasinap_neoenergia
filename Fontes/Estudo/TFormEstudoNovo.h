// ---------------------------------------------------------------------------
#ifndef TFormEstudoNovoH
#define TFormEstudoNovoH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>
#include <System.Actions.hpp>
#include <Vcl.Buttons.hpp>
#include <System.ImageList.hpp>
#include <Vcl.Dialogs.hpp>

// ---------------------------------------------------------------------------
class VTApl;
class VTAlternativa;
class VTEstudo;
class VTPlanejamento;
class VTObra;
class VTAcao;
class VTEqpto;
class VTSinapChild;
// ---------------------------------------------------------------------------
class TFormEstudoNovo : public TForm
{
__published: // IDE-managed Components
	TGroupBox *GroupBoxAlternativas;
	TToolBar *ToolBarRede;
	TActionList *ActionList;
	TAction *ActionAlternativaInsere;
	TAction *ActionAlternativaRetira;
	TTreeView *TreeView;
	TAction *ActionAnaliseDefault;
	TAction *ActionObraInsere;
	TAction *ActionObraRetira;
	TPopupMenu *PopupMenuAlternativa;
	TPopupMenu *PopupMenuObra;
	TMenuItem *MenuAlternativaInsere;
	TMenuItem *MenuAlternativaRetira;
	TMenuItem *MenuObr;
	TMenuItem *MenuObraRetira;
	TToolButton *ButEditor;
	TAction *ActionEditor;
	TAction *ActionAlignNone;
	TAction *ActionAlignLeft;
	TAction *ActionAlignRight;
	TAction *ActionAlignUp;
	TAction *ActionAlignDown;
	TPopupMenu *PopupMenuAlign;
	TMenuItem *mnuAlignLeft;
	TMenuItem *mnuAlignRight;
	TMenuItem *mnuAlignDown;
	TMenuItem *mnuAlignUp;
	TMenuItem *mnuAlignNode;
	TAction *ActionObraAlterada;
	TMenuItem *MenuItemAlternativaEdita;
	TAction *ActionAlternativaEdita;
	TAction *ActionObraEdita;
	TMenuItem *MenuItemObraEdita;
	TAction *ActionAlternativaCopia;
	TMenuItem *MenuAlternativaCopia;
	TAction *ActionAnaliseCfgPeriodo;
	TPopupMenu *PopupMenuAnalise;
	TMenuItem *N1;
	TAction *ActionObraExibeAcao;
	TMenuItem *MenuItemObraExibeAcao;
	TMenuItem *N2;
	TAction *ActionRelatorio;
	TPopupMenu *PopupMenuRelatorio;
	TMenuItem *MenuItem4;
	TMenuItem *MenuItem5;
	TMenuItem *MenuItemAnaliseDefault;
	TPopupMenu *TViewPopupMenu;
	TMenuItem *MenuItem2;
	TMenuItem *MenuItem3;
	TMenuItem *MenuItem6;
	TMenuItem *VisualizarAlternativa1;
	TAction *ActionVisualAltAno;
	TGroupBox *GroupBoxEstudo;
	TLabel *LabelEstudo;
	TBitBtn *ButEstudoEdita;
	TImageList *ImageList;
	TAction *ActionEstudoEdita;
	TMenuItem *MenuItemObraColorEnabled;
	TAction *ActionObraColorEnabled;
	TPanel *PanelAviso;
	TLabel *LabelAviso;
	TAction *ActionAnaliseCfgConfiabilidade;
	TMenuItem *MenuItemAnaliseCfgConfiabilidade;
	TAction *ActionSalvaAlt;
	TToolBar *ToolBarForm;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TToolButton *ToolButton4;
	TAction *ActionCriarCroqui;
	TPanel *PanelNomeEstudo;
	TLabel *LabelNomeEstudo;
	TGroupBox *GroupBoxPeriodo;
	TToolBar *ToolBar1;
	TToolButton *ButInsAlt;
	TToolButton *ButExcluirAlt;
	TToolButton *ToolButton7;
	TToolButton *ButDupliAlt;
	TLabel *Label1;
	TEdit *EditAnoIni;
	TLabel *Label2;
	TEdit *EditAnoFim;
	TPanel *PanelAnoIni;
	TPanel *PanelAnoFin;
	TPanel *PanelEstudo;
	TPanel *PanelAlternativa;
	TGroupBox *GroupBoxAnalise;
	TToolBar *ToolBar2;
	TToolButton *ButAnalise;
	TToolButton *ToolButton10;
	TToolButton *ButObra;
	TToolButton *ButExcObra;
	TToolButton *ToolButton13;
	TToolButton *ToolButton14;
	TToolButton *ButFinalizaObra;
	TToolButton *ButDesbloqObra;
	TToolButton *ToolButton17;
	TToolButton *ButEdtObra;
	TLabel *LabelDataUltObra;
	TAction *ActionFinalizaObra;
	TAction *ActionDesbloqueiaObra;
	TToolButton *ButEdtAlt;
	TToolButton *ToolButton1;
	TToolButton *ButSalvaAlt;
	TToolButton *ToolButton5;
	TImageList *ImageListIconesTreeView;
	TSaveDialog *SaveDialog;
	TToolButton *ButVisualizaObra;
	TAction *ActionVisualizaObra;
	TToolButton *ToolButton6;
	TToolButton *butCfgExtras;
	TAction *ActionCfgEstudo;
    TToolButton *ToolButton8;

	void __fastcall ActionAlternativaInsereExecute(TObject *Sender);
	void __fastcall ActionAlternativaRetiraExecute(TObject *Sender);
	void __fastcall TreeViewChange(TObject *Sender, TTreeNode *Node);
	void __fastcall ActionAnaliseDefaultExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ActionObraInsereExecute(TObject *Sender);
	void __fastcall ActionObraRetiraExecute(TObject *Sender);
	void __fastcall ActionEditorExecute(TObject *Sender);
	void __fastcall ActionAlignDownExecute(TObject *Sender);
	void __fastcall ActionAlignNoneExecute(TObject *Sender);
	void __fastcall ActionAlignLeftExecute(TObject *Sender);
	void __fastcall ActionAlignRightExecute(TObject *Sender);
	void __fastcall ActionAlignUpExecute(TObject *Sender);
	void __fastcall TreeViewDblClick(TObject *Sender);
	void __fastcall ActionObraAlteradaExecute(TObject *Sender);
	void __fastcall ActionAlternativaEditaExecute(TObject *Sender);
	void __fastcall ActionObraEditaExecute(TObject *Sender);
	void __fastcall ActionAlternativaCopiaExecute(TObject *Sender);
	void __fastcall ActionAnaliseCfgPeriodoExecute(TObject *Sender);
	void __fastcall ActionObraExibeAcaoExecute(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall ActionRelatorioExecute(TObject *Sender);
	void __fastcall TreeViewMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		int X, int Y);
	void __fastcall ActionVisualAltAnoExecute(TObject *Sender);
	void __fastcall TreeViewChanging(TObject *Sender, TTreeNode *Node, bool &AllowChange);
	void __fastcall ActionEstudoEditaExecute(TObject *Sender);
	void __fastcall ActionObraColorEnabledExecute(TObject *Sender);
	void __fastcall ActionAnaliseCfgConfiabilidadeExecute(TObject *Sender);
	void __fastcall ActionSalvaAltExecute(TObject *Sender);
	void __fastcall ActionFinalizaObraExecute(TObject *Sender);
	void __fastcall ActionDesbloqueiaObraExecute(TObject *Sender);
	void __fastcall ActionCriarCroquiExecute(TObject *Sender);
	void __fastcall ButEditorMouseEnter(TObject *Sender);
	void __fastcall PopupMenuRelatorioPopup(TObject *Sender);
	void __fastcall ActionVisualizaObraExecute(TObject *Sender);
	void __fastcall ActionCfgEstudoExecute(TObject *Sender);

public: // User declarations
	__fastcall TFormEstudoNovo(TComponent* Owner, VTApl *apl, TWinControl *parent);
	__fastcall ~TFormEstudoNovo(void);
	void __fastcall AtualizaEstado(bool gravando_base);

private: // métodos
	// void           __fastcall AvisoLimiteDeBarras(void);
	VTAlternativa* __fastcall AlternativaSelecionada(void);
	VTAlternativa* __fastcall AlternativaSelecionada(TTreeNode *node);
	void __fastcall AtualizaTreeView(VTObra *obra);
	bool __fastcall ConfiguraOpcaoAnalise(void);
	bool __fastcall CriaCroqui(AnsiString filepath);
	bool __fastcall EditaObra(VTObra *obra);
	void __fastcall EstadoFormAlternativaSelecionada(VTAlternativa *alternativa);
	void __fastcall EstadoFormObraSelecionada(VTObra *obra);
	void __fastcall ExibeDataAnalise(void);
    bool __fastcall ExisteEqptoNovoIsolado(VTObra *obra);
	bool __fastcall ExistePlanejamentoEstudo(void);
	bool __fastcall ExisteRedeEqvBT(void);
    bool __fastcall ExisteRedeSemTaxa(AnsiString &redes_sem_taxa, VTObra *obra);
	void __fastcall FormIniciaPosicao(void);
	void __fastcall FundeAcoesDasAlternativas(void);
	void __fastcall GraficoMoldura(VTEqpto *eqpto = NULL);
	void __fastcall GraficoZoom(VTEqpto *eqpto);
	void __fastcall HabilitaApresentacaoDasAcoes(bool enabled);
    void __fastcall HabilitaButtonsDev(void);
	void __fastcall Inicia(void);
    void __fastcall IniciaObras(void);
	VTAlternativa* __fastcall InsereAlternativa(AnsiString nome, AnsiString justificativa, bool padrao = false);
	void __fastcall LeArqPeriodoAnalise(void);
//	bool __fastcall LeDataAnalise(void);
	void __fastcall MostraCroqui(AnsiString filepath);
    AnsiString __fastcall NomeCroqui(void);
	AnsiString __fastcall NomeProximaAlternativa(void);
	VTObra* __fastcall ObraSelecionada(void);
	void __fastcall PopupMenuInicia(TPopupMenu *PopupMenu, TList *lisACTION);
	void __fastcall RetiraAlternativa(VTAlternativa *alternativa);
//	void __fastcall SalvaArqPeriodoAnalise(void);
    TWinControl* __fastcall SelecionaParentParaJanela(void);
	VTAlternativa* __fastcall SelecionaAlternativaPadrao(void);
	VTAlternativa* __fastcall TreeViewAlternativaSelecionada(TTreeNode *node);
	VTAlternativa* __fastcall TreeViewAlternativaSelecionada(void);
	void __fastcall TreeViewDescricaoAcao(VTAcao *acao, AnsiString &text);
	TTreeNode* __fastcall TreeViewExisteTreeNode(void *data);
	void __fastcall TreeViewInicia(void);
	void __fastcall TreeViewInsereTreeNodeAcao(TTreeNode *node_obra, VTAcao *acao);
	void __fastcall TreeViewInsereTreeNodeAcao(TTreeNode *node_obra, VTObra *obra);
	void __fastcall TreeViewInsereTreeNodeObra(TTreeNode *node_alter, VTAlternativa *alternativa);
	void __fastcall TreeViewInsereTreeNodeObra(TTreeNode *node_alter, VTObra *obra);
	AnsiString __fastcall TreeViewNomeDaObra(VTObra *obra);
	VTObra* __fastcall TreeViewObraSelecionada(void);
	void __fastcall TrocaRedeEqvBTporCarga(void);
	bool __fastcall ValidaDataTodasObras(void);

	// objetos externos
	VTApl *apl;
	VTEstudo *estudo;
	VTPlanejamento *planejamento;
	VTSinapChild *sinap_child;
    TList *lisNomesRedes;

	// dados locais
	bool usuario_definiu_opcao_analise;
	TList        *lisEQP, *lisTMP;

	struct
	{ // coordenadas p/ janela flutuante
		int top;
		int left;
		int width;
		int height;
	}

	janela;
};

// ---------------------------------------------------------------------------
#endif
// eof
