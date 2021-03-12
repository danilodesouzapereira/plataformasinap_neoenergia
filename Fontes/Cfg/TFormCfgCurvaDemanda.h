// ---------------------------------------------------------------------------
#ifndef TFormCfgCurvaDemandaH
#define TFormCfgCurvaDemandaH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>
#include <VCLTee.Chart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.Series.hpp>
#include <Vcl.Menus.hpp>
#include <System.ImageList.hpp>

// ---------------------------------------------------------------------------
class TAplCfg;
class VTApl;
class VTBD;
class VTClasse;
class VTCurva;
class VTCurvas;

// ---------------------------------------------------------------------------
class TFormCfgCurvaDemanda : public TForm
{
__published: // IDE-managed Components

    TToolBar *ToolBar;
    TToolButton *ButFecha;
    TToolButton *ButInsere;
    TToolButton *ButEdtita;
    TToolButton *ButRetira;
    TActionList *ActionList;
    TAction *ActionFecha;
    TAction *ActionEdita;
    TAction *ActionInsere;
    TAction *ActionRetira;
    TImageList *ImageList;
    TGroupBox *GroupBoxCurvas;
    TTreeView *TreeView;
    TGroupBox *GroupBoxCurva;
    TSplitter *Splitter1;
    TPageControl *PageControl;
    TTabSheet *TabSheetTabela;
    TStringGrid *SGrid;
    TTabSheet *TabSheetGrafico;
    TRichEdit *RichEdit;
    TAction *ActionSalvaOracle;
    TToolButton *ButImportaXML;
    TOpenDialog *OpenDialog;
    TAction *ActionImportaXML;
    TAction *ActionImportaTXT;
    TAction *ActionExportaXML;
    TAction *ActionExportaTXT;
    TToolButton *ButExportaXML;
    TToolButton *ButImportaTXT;
    TToolButton *ButExportaTXT;
    TSaveDialog *SaveDialog;
    TChart *chart;
    TLineSeries *Series1;
    TLineSeries *Series2;
    TToolButton *btImportarCurvas;
    TPopupMenu *PopupMenuImportacaoCurva;
    TMenuItem *mnExportarArquivoModelo;
    TMenuItem *mnImportarArquivoCurvas;
    TAction *ActionExportarArquivoModelo;
    TAction *ActionImportarArquivoCurvas;
    TMenuItem *mnDemanda;
	TToolButton *ToolButton1;
	TAction *ActionImportarArquivoCurvasLote;
	TRadioGroup *RGroupSerie;

    void __fastcall TreeViewChange(TObject *Sender, TTreeNode *Node);
    void __fastcall ActionEditaExecute(TObject *Sender);
    void __fastcall ActionFechaExecute(TObject *Sender);
    void __fastcall ActionInsereExecute(TObject *Sender);
    void __fastcall ActionRetiraExecute(TObject *Sender);
    void __fastcall TreeViewDblClick(TObject *Sender);
    void __fastcall ActionSalvaOracleExecute(TObject *Sender);
    void __fastcall ActionImportaXMLExecute(TObject *Sender);
    void __fastcall ActionImportaTXTExecute(TObject *Sender);
    void __fastcall ActionExportaXMLExecute(TObject *Sender);
    void __fastcall ActionExportaTXTExecute(TObject *Sender);
    void __fastcall ActionImportarArquivoCurvasExecute(TObject *Sender);
    void __fastcall ActionExportarArquivoCurvasExecute(TObject *Sender);
	void __fastcall ActionImportarArquivoCurvasLoteExecute(TObject *Sender);
	void __fastcall ChartClick(TObject *Sender);


public: // User declarations
    __fastcall TFormCfgCurvaDemanda(TComponent* Owner, VTApl *apl_owner, VTApl *apl_child);
    __fastcall ~TFormCfgCurvaDemanda(void);

private: // métodos
    void __fastcall AtualizaCurvas(void);
    void __fastcall ChartInicia(VTCurva *curva);
	TList* __fastcall LisCurvaTipicaCarga(VTClasse *classe, int dia_medicao);
	void __fastcall ChartIniciaCurvaPQ(VTCurva* curva);
    void __fastcall ChartIniciaCurvaPQVT(VTCurva* curva);
    void __fastcall ChartIniciaCurvaPQ_MDP(VTCurva* curva);
    void __fastcall ChartIniciaCurvaPFP(VTCurva* curva);
    void __fastcall ChartIniciaCurvaPFP_MDP(VTCurva* curva);
    void __fastcall ChartIniciaCurvaLD(VTCurva* curva);
    bool __fastcall DefineCodigoCurva(AnsiString &codigo);
    void __fastcall DefineIdInvalido(TList *lisCURVA);
    bool __fastcall EditaCurva(VTCurva *curva);
    void __fastcall ExcluiCurvasExistentes(void);
    bool __fastcall ExistePlanejamento(void);
    void __fastcall InsereCurvaConsumidor(VTClasse *classe);
    void __fastcall InsereCurvaCapacitor(void);
    void __fastcall InsereCurvaGeradorFotovoltaico(void);
    void __fastcall InsereCurvaIP(void);
    void __fastcall InsereCurvaReator(void);
    void __fastcall ReiniciaBaseDados(void);
    void __fastcall RichEditIniciaCurvaPQ(VTCurva* curva);
    void __fastcall RichEditIniciaCurvaPQVT(VTCurva* curva);
    void __fastcall RichEditIniciaCurvaPQ_MDP(VTCurva* curva);
    void __fastcall RichEditIniciaCurvaPFP(VTCurva* curva);
    void __fastcall RichEditIniciaCurvaPFP_MDP(VTCurva* curva);
    void __fastcall RichEditIniciaCurvaLD(VTCurva* curva);
    void __fastcall SGridIniciaCurvaPQ(VTCurva *curva);
    void __fastcall SGridIniciaCurvaPQVT(VTCurva *curva);
    void __fastcall SGridIniciaCurvaPQ_MDP(VTCurva *curva);
    void __fastcall SGridIniciaCurvaPFP(VTCurva *curva);
    void __fastcall SGridIniciaCurvaPFP_MDP(VTCurva *curva);
    void __fastcall SGridIniciaCurvaLD(VTCurva *curva);
    VTClasse* __fastcall TreeViewClasseSelecionada(void);
    VTCurva* __fastcall TreeViewCurvaSelecionada(void);
    void __fastcall TreeViewInicia(void);
    void __fastcall TreeViewInsereLisCurva(TTreeNode *node_pai, TList *lisCURVA);
    AnsiString __fastcall UnidadeCurva(int unidade);

    // externo
    VTApl *apl_child;

    // dados locais
    TAplCfg *apl;
    VTCurvas *curvas;
    TList *lisCURVA;
};

// ---------------------------------------------------------------------------
#endif
// eof
