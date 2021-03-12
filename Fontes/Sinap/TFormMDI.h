//----------------------------------------------------------------------------
#ifndef TFormMDIH
#define TFormMDIH

//----------------------------------------------------------------------------
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Messages.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <StdCtrls.hpp>
#include <Menus.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <System.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <StdActns.hpp>
#include <ToolWin.hpp>
#include <AppEvnts.hpp>
#include <System.Actions.hpp>
#include <jpeg.hpp>
#include <System.ImageList.hpp>

//----------------------------------------------------------------------------
class VTApl;
class TFormChild;

//----------------------------------------------------------------------------
class TFormMDI : public TForm
{
__published:
   TMainMenu *MainMenu;
   TMenuItem *File1;
   TMenuItem *N2;
   TMenuItem *MainMenuCriarRede;
   TMenuItem *MainMenuAbreSel;
   TMenuItem *MainMenuFechar;
   TMenuItem *N4;
   TMenuItem *MainMenuSalvar;
   TMenuItem *MainMenuSalvarComo;
   TMenuItem *N1;
   TMenuItem *MainMenuSair;
   TMenuItem *mnuConfiguracao;
   TMenuItem *mnuCfgGeral;
   TMenuItem *Window1;
   TMenuItem *WindowCascadeItem;
   TMenuItem *WindowTileItem;
   TMenuItem *WindowTileItem2;
   TMenuItem *WindowMinimizeItem;
   TMenuItem *Help1;
   TMenuItem *MenuAjuda;
   TMenuItem *MenuSobreSinap;
   TActionList *ActionList;
   TAction *ActionCriaRede;
   TAction *ActionAbreSel;
   TWindowClose *ActionFechaRede;
   TAction *ActionSair;
   TWindowCascade *ActionWinCascade;
   TWindowTileHorizontal *ActionWinTileHorizontal;
   TWindowTileVertical *ActionWinTileVertical;
   TWindowMinimizeAll *ActionWinMinimizeAll;
   TAction *ActionHelp;
   TAction *ActionSobreSinap;
   TAction *ActionChildClosed;
   TImageList *ImageList;
   TToolBar *ToolBar;
   TToolButton *butExtrator;
   TToolButton *ButCriaRede;
   TToolButton *ButAbreRede;
   TToolButton *butSave;
   TToolButton *ToolButton4;
   TToolButton *butWinCascade;
   TToolButton *butWinTileHorizontal;
   TToolButton *butWinTileVertical;
   TPanel *PanelLogo;
   TApplicationEvents *ApplicationEvents;
   TAction *ActionSalva;
   TAction *ActionSalvaComo;
   TMenuItem *MainMenuImportaInterplanMT;
   TPopupMenu *PopupMenuImporta;
   TAction *ActionCfgArranjo;
   TMenuItem *mnuCfgArranjo;
   TAction *ActionImportaAnafas;
   TAction *ActionImportaAnarede;
   TMenuItem *PopupItemImportaAnarede;
   TMenuItem *PopupItemImportaAnafas;
   TAction *ActionLicencaAlocaRemota;
   TAction *ActionLicencaLibera;
   TMenuItem *MenuLicenca;
   TMenuItem *MenuLicencaLibera;
   TMenuItem *MenuLicencaRenova;
   TMenuItem *N6;
   TTimer *TimerDemo;
   TAction *ActionLoginOn;
   TMenuItem *N7;
   TMenuItem *MenuLicencaLoginLocal;
   TAction *ActionCfgPatamares;
   TMenuItem *MenuCfgPatamares;
   TMenuItem *MainMenuImportaAnafas;
   TMenuItem *MainMenuImportaAnarede;
   TMenuItem *N3;
   TMenuItem *MenuMultiMonitor;
   TAction *ActionMultiMonitor;
   TAction *ActionMonitorExchange;
   TMenuItem *MenuMonitorExchange;
   TMenuItem *MenuExportaXml;
   TMenuItem *MenuImportaXml;
   TAction *ActionImportaXml;
   TAction *ActionExportaXml;
   TMenuItem *N8;
   TMenuItem *N9;
   TMenuItem *PopupItemImportaXml;
   TAction *ActionSobreSinapsis;
   TMenuItem *MenuSobreSinapsis;
   TMenuItem *N12;
   TAction *ActionCronometraBaseDado;
   TAction *ActionCfgFlow;
   TAction *ActionCfgCurvaDemanda;
   TMenuItem *MenuCfgCurvaDemanda;
   TAction *ActionCfgTipoChave;
   TAction *ActionCfgTipoRede;
   TMenuItem *MenuCfgTipoChave;
   TMenuItem *MenuCfgTipoRede;
   TMenuItem *MenuItemBaseDado;
   TMenuItem *MenuItemBaseAccess;
   TMenuItem *MenuItemBaseOracle;
   TAction *ActionBaseAccess;
   TAction *ActionBaseOracle;
   TToolButton *ButBaseAccess;
   TToolButton *ButBaseOracle;
   TPanel *PanelBaseDado;
   TLabel *LabelBaseDado;
   TToolButton *ButAbreRedeVazia;
   TAction *ActionAbreNone;
   TMenuItem *MainMenuAbreNone;
   TToolButton *ButPerdaTecnica;
   TAction *ActionRelatorioPerdatec;
   TMenuItem *MenuItemPerdatec;
   TMenuItem *MenuItemRelatorioPerdatec;
   TAction *ActionBatchPerdatec;
   TMenuItem *MenuItemCalculaPerdatec;
   TAction *ActionRelatorioDensidadeCarga;
   TToolButton *ToolButton2;
   TToolButton *ButRelDensidadeCarga;
   TAction *ActionExportaTxt;
   TAction *ActionImportaTxt;
   TMenuItem *MenuItemExportaTxt;
   TMenuItem *MenuItemImportaTxt;
   TMenuItem *PopupItemImportaTxt;
   TToolButton *ToolButton3;
   TAction *ActionAbreAll;
   TMenuItem *MainMenuAbreAll;
   TAction *ActionConverteBase;
   TMenuItem *MenuItemConverteBase;
   TMenuItem *PopupItemConverteBase;
   TToolButton *ButUsuario;
   TAction *ActionUsuario;
   TAction *ActionLicencaExibe;
   TMenuItem *ExibirLicenadeUsoalocada1;
   TAction *ActionAlerta;
	TAction *ActionNewChild;
	TMenuItem *N5;
   TAction *ActionBatchDensidadeCarga;
   TMenuItem *MenuItemDensiadeCarga;
   TMenuItem *MenuItemCalculaDensidadeCarga;
   TMenuItem *MenuItemDensidadeCargaRelatorio;
   TMenuItem *MenuItemGis;
   TMenuItem *MenuItemImportaBDGD;
   TMenuItem *MenuItemGisImporta;
   TAction *ActionBatchImportaGis;
   TMenuItem *MenuItemPerdareg;
   TMenuItem *MenuItemCalculaPerdareg;
   TMenuItem *MenuItemRelatorioPerdareg;
   TAction *ActionBatchPerdareg;
   TAction *ActionRelatorioPerdareg;
   TAction *ActionImportaGis;
   TMenuItem *PopupItemImportaGis;
   TMenuItem *MenuItemImportaGis;
   TAction *ActionLoginOff;
   TAction *ActionImportaBDGD;
   void __fastcall ActionAbreSelExecute(TObject *Sender);
   void __fastcall ActionChildClosedExecute(TObject *Sender);
   void __fastcall ActionSairExecute(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall ActionWinCascadeExecute(TObject *Sender);
   void __fastcall ActionWinTileHorizontalExecute(TObject *Sender);
   void __fastcall ActionWinTileVerticalExecute(TObject *Sender);
   void __fastcall ApplicationEventsMinimize(TObject *Sender);
   void __fastcall ActionSobreSinapExecute(TObject *Sender);
   void __fastcall ActionCriaRedeExecute(TObject *Sender);
   void __fastcall ActionSalvaExecute(TObject *Sender);
   void __fastcall ActionSalvaComoExecute(TObject *Sender);
   void __fastcall ActionCfgArranjoExecute(TObject *Sender);
   void __fastcall ActionImportaAnafasExecute(TObject *Sender);
   void __fastcall ActionImportaAnaredeExecute(TObject *Sender);
   void __fastcall ActionLicencaLiberaExecute(TObject *Sender);
   void __fastcall ActionLicencaAlocaRemotaExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall TimerDemoTimer(TObject *Sender);
   void __fastcall ActionLoginOnExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionCfgPatamaresExecute(TObject *Sender);
   void __fastcall ActionMultiMonitorExecute(TObject *Sender);
   void __fastcall ActionMonitorExchangeExecute(TObject *Sender);
   void __fastcall ActionExportaXmlExecute(TObject *Sender);
   void __fastcall ActionImportaXmlExecute(TObject *Sender);
   void __fastcall ActionFechaRedeExecute(TObject *Sender);
   void __fastcall ActionSobreSinapsisExecute(TObject *Sender);
   void __fastcall ActionCfgFlowExecute(TObject *Sender);
   void __fastcall ActionCfgCurvaDemandaExecute(TObject *Sender);
   void __fastcall ActionCfgTipoChaveExecute(TObject *Sender);
   void __fastcall ActionCfgTipoRedeExecute(TObject *Sender);
   void __fastcall ActionBaseAccessExecute(TObject *Sender);
   void __fastcall ActionBaseOracleExecute(TObject *Sender);
   void __fastcall ActionAbreNoneExecute(TObject *Sender);
   void __fastcall ActionRelatorioPerdatecExecute(TObject *Sender);
   void __fastcall ActionRelatorioDensidadeCargaExecute(TObject *Sender);
   void __fastcall ActionBatchPerdatecExecute(TObject *Sender);
   void __fastcall ActionExportaTxtExecute(TObject *Sender);
   void __fastcall ActionImportaTxtExecute(TObject *Sender);
   void __fastcall ActionAbreAllExecute(TObject *Sender);
   void __fastcall ActionConverteBaseExecute(TObject *Sender);
   void __fastcall ActionUsuarioExecute(TObject *Sender);
   void __fastcall ActionLicencaExibeExecute(TObject *Sender);
   void __fastcall ActionAlertaExecute(TObject *Sender);
	void __fastcall ActionNewChildExecute(TObject *Sender);
   void __fastcall ActionBatchImportaGisExecute(TObject *Sender);
   void __fastcall ActionBatchDensidadeCargaExecute(TObject *Sender);
   void __fastcall ActionBatchPerdaregExecute(TObject *Sender);
   void __fastcall ActionRelatorioPerdaregExecute(TObject *Sender);
   void __fastcall ActionImportaGisExecute(TObject *Sender);
   void __fastcall ActionLoginOffExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ActionImportaBDGDExecute(TObject *Sender);

public:
		   __fastcall  TFormMDI(TComponent *Owner);
			__fastcall ~TFormMDI(void);
	void  __fastcall InicializacaoDMS(int tipoInicializacao);

private: //métodos
//   bool        __fastcall AlocaLicenca(bool remota, bool demo);
   void        __fastcall AtualizaCaptions(void);
   void        __fastcall AtualizaMudancaDir(void);
   bool        __fastcall ConfirmaBaseDado(void);
   bool        __fastcall CriaNovaBaseDado(AnsiString arq_dest="");
   bool        __fastcall DeleteTmp(void);
   void        __fastcall DesabilitaModulos(void);
   void        __fastcall IniciaControleUso(void);
	void        __fastcall IniciaCursorMouse(void);
   void        __fastcall IniciaMultiMonitor(void);
   void        __fastcall IniciaObjetosBasicos(void);
   void        __fastcall LiberaLicenca(void);
   void        __fastcall LogoHide(void);
   void        __fastcall LogoShow(void);
   TFormChild* __fastcall NewFormChild(int stk_option=-1);
   void        __fastcall RetiraPerdaRegulatoria(void);
   void        __fastcall SinapToolKitInicia(void);
   bool        __fastcall ValidaDirBase(void);
   bool        __fastcall ValidaDirRede(void);
   bool        __fastcall ValidaLicenca(bool show_diag=false);
   bool        __fastcall ValidaProtecaoSw(void);
   void        __fastcall VerificaBaseDados(void);
   void        __fastcall VerificaLicencaFree(void);
   bool        __fastcall VerificaLoginLocal(void);
   void        __fastcall VerificaModulos(void);

	// dados
   AnsiString start_path;
   bool       show_msg_licenca;
   bool       login_local_ok;
   bool       base_oracle_enabled;
	VTApl      *apl;
   TFormChild* FormChildDMS;
};

//----------------------------------------------------------------------------
extern TFormMDI *FormMDI;

//----------------------------------------------------------------------------
#endif
//eof
