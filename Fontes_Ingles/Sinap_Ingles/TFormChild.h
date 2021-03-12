//----------------------------------------------------------------------------
#ifndef TFormChildH
#define TFormChildH

//----------------------------------------------------------------------------
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <Windows.hpp>
#include <System.hpp>
#include <StdCtrls.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <System.Actions.hpp>
#include <Vcl.Buttons.hpp>

//----------------------------------------------------------------------------
class VTApl;
class VTEstudo;
class VTGeral;
class TFormMultiMonitor;
class VTMultiPanel;

//----------------------------------------------------------------------------
class TFormChild : public TForm
{
__published:
   TActionList *ActionList;
   TImageList *ImageList;
   TAction *ActionAbreSel;
   TPanel *panelGrafico;
   TAction *ActionRedeChanged;
   TAction *ActionSalva;
   TPopupMenu *PopupMenuFlow;
   TAction *ActionSalvaComo;
   TToolButton *ToolButton1;
   TToolButton *butTopo;
   TToolButton *butFlow;
   TToolButton *ToolButton2;
   TToolButton *butEditor;
   TToolButton *butExplorer;
   TToolButton *butMedicao;
   TToolButton *butLocaliza;
   TPanel *panelToolBarGrafico;
   TAction *ActionInsRede;
   TToolButton *butFasometro;
   TToolButton *ButCurto;
   TAction *ActionCriarEsquematico;
   TPopupMenu *PopupModoGrafico;
   TAction *ActionModoGrafico;
   TMenuItem *MenuItemGraficoGeo;
   TMenuItem *MenuItemGraficoEsq;
   TAction *ActionImportaAnarede;
   TPanel *PanelPatamar;
   TComboBox *CBoxPatamar;
   TToolButton *ButCfgGrafica;
   TAction *ActionImportaXml;
   TAction *ActionExportaXml;
   TAction *ActionImportaAnafas;
   TAction *ActionGraficoAnimacao;
   TToolButton *butManobra;
   TAction *ActionManobraDo;
   TPopupMenu *PopupMenuManobra;
   TToolButton *ButAberturaMonopolar;
   TToolButton *ButConfiabilidade;
   TPopupMenu *PopupMenuTopologia;
   TMenuItem *MenuTopoEsquematico;
   TAction *ActionTopoArranjo;
   TAction *ActionTopoFases;
   TMenuItem *MenuTopo;
   TMenuItem *MenuTopoArranjo;
   TMenuItem *MenuTopoFase;
   TAction *ActionManobraUndo;
   TAction *ActionStk;
   TActionList *ActionListBase;
   TAction *ActionAberturaMonopolar;
   TAction *ActionContinuidade;
   TAction *ActionCurto;
   TAction *ActionEditor;
   TAction *ActionFlow;
   TAction *ActionLocaliza;
   TAction *ActionTopo;
   TAction *ActionGrafico;
   TAction *ActionExplorer;
   TAction *ActionMedidor;
   TAction *ActionFasometro;
   TAction *ActionManobra;
   TAction *ActionLocaliza1;
   TAction *ActionMercadoChanged;
   TAction *ActionExportaTxt;
   TAction *ActionImportaTxt;
   TAction *ActionAbreNone;
   TAction *ActionAbreAll;
   TAction *ActionConverteBase;
   TPanel *PanelToolBar;
   TPanel *PanelStk;
   TToolBar *ToolBarStk;
   TToolButton *ButStk;
   TToolButton *ToolButton4;
   TToolBar *ToolBar;
   TToolButton *ToolButton6;
   TToolBar *ToolBarManobra;
	TPanel *PanelAlerta;
	TButton *ButExibeAlerta;
	TLabel *LabelAlerta;
	TAction *ActionExibeAlerta;
   TActionList *ActionListBatch;
   TAction *ActionBatchImportaGis;
   TAction *ActionBatchPerdatec;
   TAction *ActionBatchDensidadeCarga;
   TAction *ActionBatchPerdareg;
   TAction *ActionImportaGis;
   TMenuItem *MenuFlowCfg;
   TAction *ActionFlowCfg;
   TMenuItem *N1;
   TMenuItem *MenuItemGraficoAnimacao;
   TMenuItem *MenuItemGraficoFun;
   TMenuItem *MenuItemTopoBrancoPreto;
   TAction *ActionTopoBrancoPreto;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionAbreSelExecute(TObject *Sender);
   void __fastcall ActionFlowExecute(TObject *Sender);
   void __fastcall ActionTopoExecute(TObject *Sender);
   void __fastcall ActionEditorExecute(TObject *Sender);
   void __fastcall ActionRedeChangedExecute(TObject *Sender);
   void __fastcall ActionExplorerExecute(TObject *Sender);
   void __fastcall ActionSalvaExecute(TObject *Sender);
   void __fastcall ActionSalvaComoExecute(TObject *Sender);
   void __fastcall ActionMedidorExecute(TObject *Sender);
   void __fastcall ActionInsRedeExecute(TObject *Sender);
   void __fastcall CBoxPatamarChange(TObject *Sender);
   void __fastcall ActionFasometroExecute(TObject *Sender);
   void __fastcall ActionCurtoExecute(TObject *Sender);
   void __fastcall ActionCriarEsquematicoExecute(TObject *Sender);
   void __fastcall ActionModoGraficoExecute(TObject *Sender);
   void __fastcall ActionImportaAnafasExecute(TObject *Sender);
   void __fastcall ActionImportaAnaredeExecute(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall PopupModoGraficoPopup(TObject *Sender);
   void __fastcall FormActivate(TObject *Sender);
   void __fastcall ActionGraficoExecute(TObject *Sender);
   void __fastcall ActionLocalizaExecute(TObject *Sender);
	void __fastcall ActionExportaXmlExecute(TObject *Sender);
   void __fastcall ActionImportaXmlExecute(TObject *Sender);
   void __fastcall ActionGraficoAnimacaoExecute(TObject *Sender);
   void __fastcall ActionManobraDoExecute(TObject *Sender);
   void __fastcall PopupMenuManobraPopup(TObject *Sender);
   void __fastcall ActionAberturaMonopolarExecute(TObject *Sender);
   void __fastcall ActionContinuidadeExecute(TObject *Sender);
   void __fastcall ActionTopoArranjoExecute(TObject *Sender);
   void __fastcall ActionTopoFasesExecute(TObject *Sender);
   void __fastcall ActionManobraUndoExecute(TObject *Sender);
   void __fastcall ActionStkExecute(TObject *Sender);
   void __fastcall ActionMercadoChangedExecute(TObject *Sender);
   void __fastcall ActionExportaTxtExecute(TObject *Sender);
   void __fastcall ActionImportaTxtExecute(TObject *Sender);
	void __fastcall ActionAbreNoneExecute(TObject *Sender);
   void __fastcall ActionAbreAllExecute(TObject *Sender);
   void __fastcall ActionConverteBaseExecute(TObject *Sender);
	void __fastcall ActionExibeAlertaExecute(TObject *Sender);
   void __fastcall ActionBatchDensidadeCargaExecute(TObject *Sender);
   void __fastcall ActionBatchImportaGisExecute(TObject *Sender);
   void __fastcall ActionBatchPerdatecExecute(TObject *Sender);
   void __fastcall ActionBatchPerdaregExecute(TObject *Sender);
   void __fastcall ActionImportaGisExecute(TObject *Sender);
   void __fastcall ActionFlowCfgExecute(TObject *Sender);
   void __fastcall ActionTopoBrancoPretoExecute(TObject *Sender);

public:
			 __fastcall  TFormChild(TComponent *Owner, VTApl *apl_owner, TAction *ActionOnClose, int num_seq, int stk_option=-1);
          __fastcall ~TFormChild(void);
   VTApl* __fastcall  AplChild(void);
   void   __fastcall  AtualizaDataReferencia(void);
   void   __fastcall  ConfiguraArranjos(VTApl *apl_ext);
	void   __fastcall  ConfiguraCurvasTipicas(VTApl *apl_ext);
	void   __fastcall  ConfiguraGeral(VTApl *apl_ext);
	void   __fastcall  ConfiguraGrafico(VTApl *apl_ext);
	void   __fastcall  ConfiguraPatamares(VTApl *apl_ext);
	void   __fastcall  InsereMsgAlerta(AnsiString txt);
	void   __fastcall  Reinicia(bool starting=true);

private: //métodos
	void         __fastcall AtualizaCaptionActionFlow(void);
	void         __fastcall AtualizaCaptionForm(void);
	void         __fastcall AtualizaJanelas(void);
	void         __fastcall CBoxPatamarInicia(void);
	void         __fastcall CloseForm(AnsiString class_name);
	bool         __fastcall DefineNomeEstudo(VTEstudo *estudo);
   void         __fastcall FechaJanelas(void);
   bool         __fastcall ExecutaCalculoFluxo(void);
   bool         __fastcall ExisteAlteracaoNaRede(void);
	void         __fastcall IniciaMultiPanel(void);
	void         __fastcall IniciaObjSinapChild(void);
	void         __fastcall ModuloEspecial_Inicia(void);
   void         __fastcall PopupMenuFlowIniciaCurvasTipicas(void);
   void         __fastcall PopupMenuFlowIniciaMercado(void);
   int          __fastcall Salva(bool confirma=false);
   int          __fastcall SalvaEstudo(bool confirma);
   bool         __fastcall SalvaEstudo(VTEstudo *estudo);
   bool         __fastcall SalvaEstudoComo(VTEstudo *estudo, bool edita_nome);
   int          __fastcall SalvaRede(bool confirma);
   bool         __fastcall SalvaRedeBase(bool base_nova);
   bool         __fastcall SalvaRedeBaseExistente(void);
   bool         __fastcall SalvaRedeBaseNova(void);
   bool         __fastcall SalvaRedeComo(void);
   TWinControl* __fastcall SelecionaParentParaJanela(void);
   void         __fastcall SelecionaPatamarMaiorDemanda(void);
	void         __fastcall SinapToolKitInicia(int stk_option);
   bool         __fastcall ValidaEqptoIsolado(void);
   //bool         __fastcall ValidaExecucaoConfiabilidade(void);
   bool         __fastcall ValidaLimiteLicenca(void);
   void         __fastcall VerificaModulos(void);

private: //dados locais
   VTApl   *apl;

private: //dados externos
   bool              aviso_base_salva;
   TAction           *ActionOnClose;
   TFormMultiMonitor *FormMultiMonitor;
   VTMultiPanel      *MultiPanel;
};

//----------------------------------------------------------------------------
#endif
//eof
