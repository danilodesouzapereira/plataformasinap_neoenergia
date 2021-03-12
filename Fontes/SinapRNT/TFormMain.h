//---------------------------------------------------------------------------
#ifndef TFormMainH
#define TFormMainH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TActionList *ActionList;
	TPanel *PanelLogo;
	TImage *imagLogo;
	TToolBar *ToolBar;
	TMenuItem *Arquivo1;
	TImageList *ImageList;
	TAction *ActionFechaRede;
	TMenuItem *MenuItemFecha;
	TMenuItem *MenuItemAjuda;
	TMenuItem *MenuItemSobreSinapsis;
	TMenuItem *MenuItemSobreSINAPrnt;
	TAction *ActionSobreSinapsis;
	TAction *ActionSobreSINAPrnt;
	TToolButton *ToolButton1;
	TMenuItem *MenuItemInterfaceGIS;
	TAction *ActionImportaSDBT;
	TAction *ActionImportaSDMT;
	TMenuItem *MenuItemImportaSDMT;
	TMenuItem *MenuItemImportaSDBT;
	TMenuItem *MenuItemEstudo;
	TAction *ActionAbreEstudoSDBT;
	TAction *ActionAbreEstudoSDMT;
	TMenuItem *MenuItemAbreEstudoSDMT;
	TMenuItem *MenuItemAbreEstudoSDBT;
	TMenuItem *MenuItemConfigura;
	TMenuItem *MenuItemCfgArranjo;
	TMenuItem *MenuItemCfgCurvaTipica;
	TAction *ActionCfgArranjo;
	TAction *ActionCfgCurvaTipica;
	TMenuItem *MenuItemLicenca;
	TAction *ActionLicencaAloca;
	TAction *ActionLicencaLibera;
	TAction *ActionLicencaExibe;
	TAction *ActionReadSenha;
	TMenuItem *MenuItemLicencaAloca;
	TMenuItem *MenuItemLicencaLibera;
	TMenuItem *MenuItemLicencaExibe;
	TMenuItem *N1;
	TMenuItem *MenuItemSenhaRead;
	TAction *ActionAlerta;
	TPanel *PanelAlerta;
	TLabel *LabelAlerta;
	TButton *ButExibeAlerta;
	TActionList *ActionList1;
	TAction *ActionTopologia;
	TPanel *PanelGrafico;
	TStatusBar *StatusBar;
	TPanel *PanelToolBarGrafico;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TToolButton *ButTopologia;
	TToolButton *ButFlow;
	TAction *ActionFlow;
   TPopupMenu *PopupMenuFlow_OLD;
	TMenuItem *MenuFlowRelatorioNenhum;
	TMenuItem *MenuFlowRelatorioResumido;
	TMenuItem *MenuFlowRelatorioCompleto;
	TMenuItem *MenuSeparador;
	TMenuItem *MenuFlowAnimacao;
	TMenuItem *MenuFlowCronometro;
	TMenuItem *N4;
	TMenuItem *MenuFlowPorRede;
	TMenuItem *MenuFlowThreads;
	TMenuItem *MenuFlowOtimizacao;
	TMenuItem *MenuItem1;
	TMenuItem *MenuFlowAlgoritmoAutomatico;
	TMenuItem *MenuFlowAlgoritmoEquilibrado;
	TMenuItem *MenuFlowAlgoritmoG1;
	TMenuItem *MenuFlowAlgoritmoG3;
	TMenuItem *MenuFlowAlgoritmoNR1;
	TMenuItem *MenuFlowAlgoritmoNR3;
	TMenuItem *MenuFlowAlgoritmoRadial1;
	TMenuItem *MenuFlowAlgoritmoRadial3;
	TMenuItem *MenuFlowAlgoritmoCC1;
	TMenuItem *N8;
	TMenuItem *MenuFlowCargaAuto;
	TMenuItem *MenuFlowCargaIcte;
	TMenuItem *MenuFlowCargaScte;
	TMenuItem *MenuFlowCargaZcte;
	TMenuItem *MenuFlowCargaAneel;
	TMenuItem *N2;
	TMenuItem *MenuFlowPerdaFe;
	TMenuItem *N3;
	TMenuItem *MenuFlowAnoCarga;
	TMenuItem *MenuFlowAno0;
	TMenuItem *MenuFlowAno1;
	TMenuItem *MenuFlowCurvaTipica;
	TMenuItem *MenuFlowCurvaDiaUtil;
	TMenuItem *MenuFlowCurvaSabado;
	TMenuItem *MenuFlowCurvaDomingo;
	TMenuItem *N5;
	TMenuItem *MenuFlowCurvaSemana;
	TPanel *PanelPatamar;
	TComboBox *CBoxPatamar;
	TAction *ActionSelAnoMercado;
	TAction *ActionMercadoChanged;
	TAction *ActionSalva;
	TAction *ActionSalvaComo;
	TAction *ActionRedeChanged;
	TMenuItem *Salvar1;
	TAction *ActionSair;
	TToolButton *ToolButton4;
	TAction *ActionEditor;
	TToolButton *ToolButton5;
	TToolButton *ToolButton6;
	TToolButton *ToolButton7;
	TAction *ActionDemanda;
	TAction *ActionAlocaBC;
	TAction *ActionAlocaRT;
	TAction *ActionExibeAlerta;
	TAction *ActionLicencaLoginLocal;
	TTimer *TimerDemo;
	TAction *ActionAnimacao;
	TToolButton *ToolButton8;
	TToolButton *ToolButton9;
	TAction *ActionCriaEstudo;
   TAction *ActionHelp;
   TMenuItem *MenuItemHelp;
   TMenuItem *N6;
   TPopupMenu *PopupMenuFlow;
   TMenuItem *MenuFlowCfg;
   TAction *ActionFlowCfg;
	void __fastcall ActionFechaRedeExecute(TObject *Sender);
	void __fastcall ActionSobreSINAPrntExecute(TObject *Sender);
	void __fastcall ActionSobreSinapsisExecute(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall ActionImportaSDBTExecute(TObject *Sender);
	void __fastcall ActionImportaSDMTExecute(TObject *Sender);
	void __fastcall ActionAbreEstudoSDBTExecute(TObject *Sender);
	void __fastcall ActionAbreEstudoSDMTExecute(TObject *Sender);
	void __fastcall ActionCfgArranjoExecute(TObject *Sender);
	void __fastcall ActionCfgCurvaTipicaExecute(TObject *Sender);
	void __fastcall ActionLicencaAlocaExecute(TObject *Sender);
	void __fastcall ActionLicencaLiberaExecute(TObject *Sender);
	void __fastcall ActionLicencaExibeExecute(TObject *Sender);
	void __fastcall ActionReadSenhaExecute(TObject *Sender);
	void __fastcall ActionAlertaExecute(TObject *Sender);
	void __fastcall ActionTopologiaExecute(TObject *Sender);
	void __fastcall ActionFlowExecute(TObject *Sender);
	void __fastcall ActionSelAnoMercadoExecute(TObject *Sender);
	void __fastcall ActionMercadoChangedExecute(TObject *Sender);
	void __fastcall ActionSalvaExecute(TObject *Sender);
	void __fastcall ActionSalvaComoExecute(TObject *Sender);
	void __fastcall ActionRedeChangedExecute(TObject *Sender);
	void __fastcall ActionSairExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ActionEditorExecute(TObject *Sender);
	void __fastcall ActionDemandaExecute(TObject *Sender);
	void __fastcall ActionAlocaBCExecute(TObject *Sender);
	void __fastcall ActionAlocaRTExecute(TObject *Sender);
	void __fastcall ActionExibeAlertaExecute(TObject *Sender);
	void __fastcall ActionLicencaLoginLocalExecute(TObject *Sender);
	void __fastcall CBoxPatamarChange(TObject *Sender);
	void __fastcall ActionAnimacaoExecute(TObject *Sender);
	//void __fastcall ActionFlowAlteraOpcaoExecute(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ActionCriaEstudoExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionFlowCfgExecute(TObject *Sender);

public:		// User declarations
	__fastcall  TFormMain(TComponent* Owner);
	__fastcall ~TFormMain(void);

private:	//métodos
	bool			 __fastcall AbreReclamacao(int tipo_estudo);
	bool         __fastcall AbreEstudoRNT(int tipo_estudo);
	bool 			 __fastcall AlocaLicenca(bool remota, bool demo);
	bool         __fastcall AnoMercado(int &ano);
   void         __fastcall AtualizaCaptions(void);
	void         __fastcall AtualizaJanelas(void);
	void 			 __fastcall AtualizaMudancaDir(void);
	void         __fastcall CBoxPatamarInicia(void);
	//bool         __fastcall ConfiguraFlow(void);
	bool 			 __fastcall CriaNovaBaseDado(AnsiString arq_dest);
	bool         __fastcall DefineNomeEstudo(VTEstudo *estudo);
	bool         __fastcall ExecutaCalculoFluxo(void);
	void         __fastcall ExibeNomeRede(void);
	bool         __fastcall ExisteAlteracaoNaRede(void);
	bool 			 __fastcall ExisteModuloAtivoConflitante(void);
	bool         __fastcall ExisteRedeDoEstudo(void);
	bool         __fastcall ImportaRede(int tipo_estudo);
	void 			 __fastcall IniciaControleUso(void);
	void 			 __fastcall IniciaCursorMouse(void);
	void         __fastcall LogoHide(void);
	void         __fastcall LogoShow(void);
	void         __fastcall PopupMenuFlowIniciaCurvasTipicas(void);
	void         __fastcall PopupMenuFlowIniciaMercado(void);
	void         __fastcall ReiniciaAlerta(void);
	bool         __fastcall ReiniciaRede(bool confirma=false);
	int 			 __fastcall Salva(bool confirma=false);
	int          __fastcall SalvaEstudo(bool confirma);
   bool         __fastcall SalvaEstudo(VTEstudo *estudo, bool aviso=true);
   bool         __fastcall SalvaEstudoComo(VTEstudo *estudo, bool edita_nome);
   int          __fastcall SalvaRede(bool confirma);
   bool         __fastcall SalvaRedeBase(bool base_nova);
   bool         __fastcall SalvaRedeBaseExistente(void);
   bool         __fastcall SalvaRedeBaseNova(void);
	bool         __fastcall SalvaRedeComo(void);
	TWinControl* __fastcall SelecionaParentParaJanela(void);
	void         __fastcall SelecionaPatamarMaiorDemanda(void);
	void         __fastcall TrataRedeCarregada(void);
	bool 			 __fastcall ValidaDirBase(void);
	bool         __fastcall ValidaEqptoIsolado(void);
	bool 			 __fastcall ValidaLicenca(bool show_diag=false);
	bool 			 __fastcall ValidaProtecaoSw(void);
	bool 			 __fastcall VerificaLoginLocal(void);
	void 			 __fastcall VerificaModulos(void);

private:	//dados
	AnsiString start_path;
	bool       aviso_base_salva;
	bool       login_local_ok;
	bool       show_msg_licenca;
	VTApl      *apl;
	TForm      *FormReclamacao;
};

//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;

//---------------------------------------------------------------------------
#endif
//eof

