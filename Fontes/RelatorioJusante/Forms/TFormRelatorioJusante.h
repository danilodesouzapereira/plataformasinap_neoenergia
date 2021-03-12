// ---------------------------------------------------------------------------
#ifndef TFormRelatorioJusanteH
#define TFormRelatorioJusanteH

// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.CheckLst.hpp>
#include "TFormEdt.h"
#include <Vcl.Grids.hpp>
#include <Vcl.ValEdit.hpp>
#include <Vcl.Dialogs.hpp>
#include <System.ImageList.hpp>

// ---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTEdt;
class VTGrafico;
class VTRelatorioJusante;
class VTPatamar;

// ---------------------------------------------------------------------------
class TFormRelatorioJusante : public TFormEdt
{
__published: // IDE-managed Components

	TAction *ActionSelecionaLigacao;
	TPanel *pnlBackGround;
	TTabControl *tabControl;
	TValueListEditor *vleAtributos;
	TGroupBox *gboxOpcoes;
	TPanel *panelPatamar;
	TLabel *lblPatamar;
	TComboBox *CBoxPatamar;
	TCheckBox *checkFlow;
	TPanel *PanelColor;
	TPanel *pnlSelectColor;
	TLabel *Label1;
	TColorDialog *dlgSelColor;
	TSaveDialog *SaveDialog;
	TToolBar *ToolBar1;
	TToolBar *ToolBar2;
	TToolButton *butExportarRel;
	TAction *ActionExporta;
	TPanel *panelTipoRelatorio;
	TLabel *Tipo;
	TComboBox *CBoxRelatorio;

	void __fastcall ActionSelecionaLigacaoExecute(TObject *Sender);
	void __fastcall ActionEdtExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall tabControlChange(TObject *Sender);
	void __fastcall checkFlowClick(TObject *Sender);
	void __fastcall CBoxPatamarChange(TObject *Sender);
	void __fastcall CBoxRelatorioChange(TObject *Sender);
	void __fastcall PanelColorClick(TObject *Sender);
	void __fastcall ActionExportaExecute(TObject *Sender);

private: // User declarations

	//void __fastcall CalculaPotenciaTransformadoresJusante(void);
	void __fastcall CBoxPatamarInicia(void);
	void __fastcall CheckBoxFlowInicia(void);
	bool __fastcall ExisteRedeEqvBT(void);
	void __fastcall SelecionaEquipamento(void);
	void __fastcall PreencheListValueEditor(int tipoeqpto);
	void __fastcall DefineTitleCaptionVlwATributos(AnsiString primeiraColuna, AnsiString segundaColuna);
	bool __fastcall SelecionaArquivo(void);
	void __fastcall PreparaRelatorio(TStrings *lisAtribValor, TStringList *relatorio);
	void __fastcall PreparaRelatorioTrechos(TStrings *lisAtribValor,TStringList *relatorio);

public: // User declarations
	__fastcall TFormRelatorioJusante(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);
	__fastcall ~TFormRelatorioJusante(void);

	// Variáveis da classe;
private:
	VTApl *apl;
	VTEdt *edt;
	VTEqpto *eqpto;
	VTGrafico *grafico;
	VTRelatorioJusante *relatorioJusante;
	VTPatamar *patamar_sel;

	int tipo_relatorio;
	double somaTrafosJusante;

	// memória alocada p/ TImage
	//Graphics::TBitmap *graph_bmp;

};

// ---------------------------------------------------------------------------
extern PACKAGE TFormRelatorioJusante *FormRelatorioJusante;
// ---------------------------------------------------------------------------
#endif
