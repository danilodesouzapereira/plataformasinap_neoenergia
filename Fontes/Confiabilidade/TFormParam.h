//---------------------------------------------------------------------------
#ifndef TFormParamH
#define TFormParamH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <CheckLst.hpp>
#include "..\Confiabilidade\VTConfiab.h"
#include <ExtCtrls.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>//hk10a

//---------------------------------------------------------------------------
class VTApl;
class VTDataEqpto;
class VTDadoConfiab;

//---------------------------------------------------------------------------
class TFormParam : public TForm
{
__published:	// IDE-managed Components
		  TImageList *ImageList1;
		  TActionList *ActionList1;
		  TAction *ActionCancela;
		  TAction *ActionConfirma;
		  TAction *ActionRetira;
	TToolBar *tbConfirma;
		  TToolButton *butConfirma;
		  TToolButton *butCancela;
	TActionList *ActionList2;
	TAction *ActionDisableProt;
	TAction *ActionEnableProt;
	TPageControl *PageControl;
	TTabSheet *TabSheet1;
	TStringGrid *sgridParamTipo;
	TGroupBox *GroupBox3;
	TToolBar *ToolBarPatamar;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TCheckListBox *CLBoxProt;
	TGroupBox *GroupBox2;
	TLabel *Label3;
	TComboBox *cbxOrdem;
	TCheckBox *chkNA;
	TTabSheet *TabSheet2;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet4;
	TTabSheet *TabSheet5;
	TStringGrid *sgridParamTrecho;
	TStringGrid *sgridParamGerador;
	TStringGrid *sgridParamTrafo;
	TStringGrid *sgridParamChave;
	TStatusBar *StatusBar1;
	TAction *ActionIncluirTipoTrecho;
	TPanel *Panel1;
	TSplitter *Splitter1;
	TCheckListBox *lbTipoTrecho;
	TToolBar *ToolBar2;
	TToolButton *ToolButton1;
	TToolButton *ToolButton4;
	TAction *ActionEnableTipoTrecho;
	TAction *ActionDisableTipoTrecho;
	TTabSheet *Equipamentos;
	TPanel *Panel2;
	TCheckListBox *lbEqpto;
	TToolBar *ToolBar3;
	TToolButton *ToolButton5;
	TToolButton *ToolButton6;
	TSplitter *Splitter2;
	TStringGrid *sgridParamEqpto;
	TAction *ActionIncluirEqpto;
	TAction *ActionEnableEqpto;
	TAction *ActionDisableEqpto;
	TPanel *Panel3;
	TCheckListBox *lbTipoGerador;
	TToolBar *ToolBar4;
	TToolButton *ToolButton7;
	TToolButton *ToolButton8;
	TSplitter *Splitter3;
	TPanel *Panel4;
	TCheckListBox *lbTipoTrafo;
	TToolBar *ToolBar5;
	TToolButton *ToolButton9;
	TToolButton *ToolButton10;
	TSplitter *Splitter4;
	TPanel *Panel5;
	TCheckListBox *lbTipoChave;
	TToolBar *ToolBar6;
	TToolButton *ToolButton11;
	TToolButton *ToolButton12;
	TSplitter *Splitter5;
	TAction *ActionIncluirTipoGerador;
	TAction *ActionIncluirTipoTrafo;
	TAction *ActionIncluirTipoChave;
	TAction *ActionEnableTipoGerador;
	TAction *ActionEnableTipoTrafo;
	TAction *ActionEnableTipoChave;
	TAction *ActionDisableTipoGerador;
	TAction *ActionDisableTipoTrafo;
	TAction *ActionDisableTipoChave;
	TCheckBox *chkBloco;
	TTabSheet *TabSheet6;
	TPanel *Panel6;
	TCheckListBox *lbBlocos;
	TToolBar *ToolBar1;
	TToolButton *ToolButton13;
	TToolButton *ToolButton14;
	TSplitter *Splitter6;
	TStringGrid *sgridParamBloco;
	TAction *ActionMostraEqpto;
	TTabSheet *TabSheet7;
	TPanel *Panel7;
	TSplitter *Splitter7;
	TStringGrid *sgridParamEqptoMutua;
	TListBox *lbEqptoMutua;
	TGroupBox *GroupBox1;
	TButton *Button1;
	TAction *ActionInsereMutua;
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CLBoxProtClick(TObject *Sender);
	void __fastcall ActionDisableProtExecute(TObject *Sender);
	void __fastcall ActionEnableProtExecute(TObject *Sender);
	void __fastcall ActionIncluirTipoTrechoExecute(TObject *Sender);
	void __fastcall ActionEnableTipoTrechoExecute(TObject *Sender);
	void __fastcall ActionDisableTipoTrechoExecute(TObject *Sender);
	void __fastcall ActionIncluirEqptoExecute(TObject *Sender);
	void __fastcall ActionEnableEqptoExecute(TObject *Sender);
	void __fastcall ActionDisableEqptoExecute(TObject *Sender);
	void __fastcall ActionIncluirTipoGeradorExecute(TObject *Sender);
	void __fastcall ActionIncluirTipoTrafoExecute(TObject *Sender);
	void __fastcall ActionIncluirTipoChaveExecute(TObject *Sender);
	void __fastcall ActionEnableTipoGeradorExecute(TObject *Sender);
	void __fastcall ActionEnableTipoTrafoExecute(TObject *Sender);
	void __fastcall ActionEnableTipoChaveExecute(TObject *Sender);
	void __fastcall ActionDisableTipoGeradorExecute(TObject *Sender);
	void __fastcall ActionDisableTipoTrafoExecute(TObject *Sender);
	void __fastcall ActionDisableTipoChaveExecute(TObject *Sender);
	void __fastcall ActionMostraEqptoExecute(TObject *Sender);
	void __fastcall ActionInsereMutuaExecute(TObject *Sender);

public:
	//construtor
	__fastcall TFormParam(TComponent* Owner, TWinControl *parent, VTApl *apl);
	__fastcall ~TFormParam(void);
	//interface
	void __fastcall Atualiza(VTDadoConfiab* dataConfiab);

	//objetos internos
	VTDadoConfiab* dataConfiab;

private:
	//métodos internos
	void __fastcall AdicionaReg(TStringGrid* sgrid, PAR_CONF pconf, AnsiString aux, TObject* data = NULL);
	void __fastcall CLBoxProtInicia(void);
	void __fastcall IniciaProt(void);
	void __fastcall sgridParamInicia(TStringGrid* sgrid);
	//objetos externos
	VTApl    *apl;

};
//---------------------------------------------------------------------------
#endif
