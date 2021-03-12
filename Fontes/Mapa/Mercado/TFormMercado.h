//---------------------------------------------------------------------------
#ifndef TFormMercadoH
#define TFormMercadoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TFormEdt.h"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTMercado;
class VTSegmento;
class VTCrescimento;
class VTRedes;
class VTRegiao;
class VTBD;
//class VTBD_mercado;

//class TFormEdtRegiao;

//---------------------------------------------------------------------------
class TFormMercado : public TFormEdt
{
__published:   // IDE-managed Components
   TAction *ActionCancela;
   TAction *ActionConfirma;
	TAction *ActionEditaSegmento;
	TAction *ActionInsereSegmento;
	TAction *ActionRetiraSegmento;
   TToolBar *ToolBarRight;
   TToolButton *ButConfirma;
   TToolButton *butCancela;
   TToolButton *ButEdita;
   TToolButton *ButRetira;
   TSplitter *Splitter2;
   TGroupBox *GroupBoxChart;
   TChart *chart;
   TFastLineSeries *Series1;
   TFastLineSeries *Series2;
   TFastLineSeries *Series3;
   TFastLineSeries *Series4;
   TFastLineSeries *Series5;
   TFastLineSeries *Series6;
   TFastLineSeries *Series7;
   TFastLineSeries *Series8;
   TPanel *PanelMercado;
   TGroupBox *GroupBoxRegiao;
   TTreeView *TreeView;
   THeaderControl *TreeViewHeader;
   TPanel *PanelPeriodo;
   TGroupBox *GBoxAnoInicial;
   TComboBox *CBoxAnoInicial;
   TGroupBox *GBOxAnoFinal;
   TComboBox *CBoxAnoFinal;
   TAction *ActionSelArea;
	TToolButton *ButReiniciaTaxas;
	TAction *ActionReiniciaTaxas;
   TToolButton *ToolButton1;
	TLabel *LabelAviso;
   TPanel *PanelAviso;
   TLabel *LabelRegiao;
	TAction *ActionAddRegiao;
	TToolButton *ToolButton2;
	TToolButton *ToolButton4;
	TAction *ActionHelp;
   TAction *ActionRegiaoEditado;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionEditaSegmentoExecute(TObject *Sender);
   void __fastcall ActionInsereSegmentoExecute(TObject *Sender);
   void __fastcall ActionRetiraSegmentoExecute(TObject *Sender);
   void __fastcall CBoxAnoChange(TObject *Sender);
   void __fastcall TreeViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall TreeViewAdvancedCustomDrawItem(TCustomTreeView *Sender,
          TTreeNode *Node, TCustomDrawState State, TCustomDrawStage Stage,
          bool &PaintImages, bool &DefaultDraw);
   void __fastcall TreeViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormIniciaPosicao(void); // FKM - 2015.10.13
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionSelAreaExecute(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall ActionReiniciaTaxasExecute(TObject *Sender);
	void __fastcall ActionAddRegiaoExecute(TObject *Sender);
	void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionRegiaoEditadoExecute(TObject *Sender);

public:      // User declarations
   __fastcall  TFormMercado(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormMercado(void);

private:   // User declarations
   void           __fastcall AtualizaForm(void);
   void           __fastcall BaseDadoConecta(void);
   void           __fastcall BaseDadoDesconecta(void);
   void           __fastcall CBoxAnoInicia(void);
   TColor         __fastcall ChartCorSerie(int ind);
   void           __fastcall ChartCriaSeries(void);
   void           __fastcall ChartInicia(TList *lisCRESCIMENTO, int index=-1);
   void           __fastcall ChartIniciaSerie(VTCrescimento *crescimento, TChartSeries *serie,
                                              TColor cor, bool first);
	void           __fastcall CriaSegmentoDefault(void);
	void           __fastcall CriaSegmentoParaRedes(void);
   void           __fastcall CriaSegmentoParaRegioes(void);
	TTreeNode* 		__fastcall DefineNodeRede(VTSegmento* regiao);
   //void           __fastcall EliminaRegiaoComCrescimentoNulo(void);
   bool           __fastcall ExisteAlteracao(void);
   bool           __fastcall ExisteAlteracaoEmRegioes(void);
   VTCrescimento* __fastcall ExisteCrescimentoSelecionado(TTreeNode *node);
	VTSegmento*    __fastcall ExisteSegmentoSelecionada(TTreeNode *node);
	//void           __fastcall InsereSegmentoDefinidaPorArea(AnsiString codigo);
	//void           __fastcall RetiraDaBaseSegmentoComCrescimentoNulo(void);
   void           __fastcall RegistraRegioesPreExistentes(void);
   void           __fastcall RegioesVisible(bool visible);
	void           __fastcall RetiraDaBaseSegmentoSemCrescimento(void);
   void           __fastcall RetiraDaBaseSegmentoSemRegiao(VTSegmento *segmento);
	void           __fastcall RetiraSegmentoNova(void);
   void           __fastcall RetiraSegmentoRegiaoRemovida(void);
   void           __fastcall RetornaModoGrafico(void);
   TTreeNode*     __fastcall TreeViewExisteNode(TTreeNode *node_pai, TObject *data);
   void           __fastcall TreeViewInicia(void);
	TTreeNode*     __fastcall TreeViewInsereSegmento(TTreeNode *node_pai, VTSegmento *segmento);
   bool           __fastcall VerificaSomenteSDBT(void);

   //TESTE: funcoes que devem ser alteradas no vTMERCADO
   VTSegmento*  __fastcall  CriaSegmentoNova(VTRegiao *regiao);
   VTSegmento*  __fastcall  ExisteSegmento(VTRegiao *regiao);

private: //objetos externos
   VTApl     *apl;
   VTMercado *mercado;
   VTRedes   *redes;

private: //dados locais
   int          modoGraficoAnterior;
   VTBD         *bd;
   //VTBD_mercado *bd_mercado;
   TList        *lisCRE;
   TList        *lisREGIAO_pre;
	TTreeNode    *node_default, *node_tiporede, *node_rede, *node_carregada, *node_regiao;

	TForm *formEdtRegiao;
};

//---------------------------------------------------------------------------
#endif
//eof

