//---------------------------------------------------------------------------
#ifndef TFormConfiabilidadeH
#define TFormConfiabilidadeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Forms.hpp>
#include "TFormEdt.h"
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
#include "..\Confiabilidade\VTConfiab.h"
#include <Grids.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
#define IMAGE 14 //imagem de seleção
//---------------------------------------------------------------------------
class VTApl;
class VTArvores;
class VTDadoConfiab;
class VTEdt;
class VTEqpto;
class VTTipoRede;
class VTRede;
//---------------------------------------------------------------------------
class TFormConfiabilidade : public TFormEdt
{
__published:	// IDE-managed Components
	TAction *ActionCfgUmido;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TAction *ActionConfiab;
   TPanel *PanelEsquerdo;
	TTreeView *TreeViewRedes;
   TPanel *PanelDireito;
	TGroupBox *gboxGrid;
	TListView *ListViewConfiab;
	TSplitter *Splitter1;
	TPopupMenu *PopupMenuConfiabCfg;
	TAction *ActionCfgSeco;
	TMenuItem *PerodoSeco1;
	TMenuItem *Parmetros1;
	TToolButton *ToolButton4;
	TAction *ActionCfgGraf;
	TPopupMenu *PopupMenuConfiabExec;
	TAction *ActionExecSeco;
	TAction *ActionExecUmido;
	TMenuItem *Perodoseco2;
	TMenuItem *Perodomido1;
	TToolButton *ToolButton5;
	TAction *ActionSelEqpto;
	TToolButton *ToolButton6;
	TToolButton *ToolButton7;
	TAction *ActionViewDEC;
	TAction *ActionViewFEC;
	TAction *ActionViewEND;
	TAction *ActionViewPOT;
	TPopupMenu *PopupMenuView;
	TMenuItem *DEChano1;
	TMenuItem *FECintano1;
	TMenuItem *ENDMWhano1;
	TMenuItem *PotnciaMW1;
	TStatusBar *StatusBar;
	TAction *ActionViewTOPO;
	TMenuItem *opologia1;
	TGroupBox *GroupBox2;
	TAction *ActionMostraArv;
	TGroupBox *GroupBox1;
	TTreeView *tviewArv;
	TTreeView *tviewCorte;
	TSplitter *Splitter2;
	TAction *ActionMostraCorte;
	TAction *ActionMostraFontes;
	TPanel *Panel1;
	TStringGrid *sgridDadosEqpto;
	TToolBar *ToolBar1;
	TToolButton *butConfirma;
	TTreeView *tviewBlocos;
	TAction *ActionMostraBlocos;
	TAction *ActionMostraLigaBlocos;
	TSplitter *Splitter3;
	TToolButton *ToolButton8;
	TAction *ActionRelatCargaBlocos;
	TAction *ActionExportaResultados;
	TToolButton *ToolButton9;
	TSaveDialog *saveDialog;
	TAction *ActionMostraEqptos;
	TToolButton *ToolButton10;
	TAction *ActionImportaParam;
	TOpenDialog *OpenDialog;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
	void __fastcall ActionCfgUmidoExecute(TObject *Sender);
	void __fastcall ActionConfiabExecute(TObject *Sender);
	void __fastcall ActionCfgSecoExecute(TObject *Sender);
	void __fastcall ActionCfgGrafExecute(TObject *Sender);
	void __fastcall ActionExecSecoExecute(TObject *Sender);
	void __fastcall ActionExecUmidoExecute(TObject *Sender);
	void __fastcall ActionEdtExecute(TObject *Sender);
	void __fastcall ActionSelEqptoExecute(TObject *Sender);
	void __fastcall ActionViewDECExecute(TObject *Sender);
	void __fastcall ActionViewENDExecute(TObject *Sender);
	void __fastcall ActionViewFECExecute(TObject *Sender);
	void __fastcall ActionViewPOTExecute(TObject *Sender);
	void __fastcall TreeViewRedesClick(TObject *Sender);
	void __fastcall TreeViewRedesChange(TObject *Sender, TTreeNode *Node);
	void __fastcall ActionViewTOPOExecute(TObject *Sender);
	void __fastcall ActionMostraArvExecute(TObject *Sender);
	void __fastcall ActionMostraCorteExecute(TObject *Sender);
	void __fastcall ActionMostraFontesExecute(TObject *Sender);
	void __fastcall ActionMostraBlocosExecute(TObject *Sender);
	void __fastcall ActionMostraLigaBlocosExecute(TObject *Sender);
	void __fastcall ActionRelatCargaBlocosExecute(TObject *Sender);
	void __fastcall ActionExportaResultadosExecute(TObject *Sender);
	void __fastcall ActionMostraEqptosExecute(TObject *Sender);
	void __fastcall ActionImportaParamExecute(TObject *Sender);

public:		// User declarations
   __fastcall  TFormConfiabilidade(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormConfiabilidade(void);

private:	//métodos
	void __fastcall ApresentaIndices(TTreeNode *node);
	void __fastcall ApresentaEqpto(VTEqpto *carga);
	void __fastcall ApresentaGeral(void);
	void __fastcall ApresentaCargas(VTRede *rede);
	void __fastcall ApresentaRede(VTRede *rede);
	void __fastcall ApresentaTipoRede(VTTipoRede *tipoRede);
   void __fastcall DefineCargas(TList* lisGrupoCargas);
	void __fastcall DefineSuprimentos(TList* lisSUP);
	void __fastcall DefLigaExclud(TList* lisExcl);
	void __fastcall DefineViewMenu(TPopupMenu *menu, int index, int imageIndex);
	void __fastcall ExecutaConfSC(void);
   void __fastcall ImprimeDataConfiab(VTDadoConfiab *data);
	void __fastcall Inicia(void);
	//void __fastcall IniciaParamConf(void);
	bool __fastcall IniciaParamConf(VTDadoConfiab* dataConfiab);
	void __fastcall IniciaRede(void);
	void __fastcall IniciaTreeViewRede(void);
	void __fastcall ListViewInsereConf(TListView *ListView, RES_CONF rconf, TObject* pobj, AnsiString txt_col1);
	void __fastcall Moldura(VTEqpto *eqpto = NULL);
	void __fastcall MostraArvores(void);
	void __fastcall MostraArvores(TTreeView *tview, TList *lisArvs);
	void __fastcall MostraCortes(TTreeView* tview);
   void __fastcall MostraEqptos(TObject *pobj);

private:	//objetos externos

private:	//dados locais
	VTApl  *apl;
	TList  *lisEQS;
	TList  *lisTMP;
	VTDadoConfiab *dataSeco, *dataUmido;
	VTEdt         *edt_busca;

};

//---------------------------------------------------------------------------
#endif
//eof

