//---------------------------------------------------------------------------
#ifndef TFormPadraoH
#define TFormPadraoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include "TFORMEDT.h"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <Dialogs.hpp>
#include "..\Forms\TFormEdt.h"
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEdt;
class VTEqpto;
class VTReducao;
class VTGeral;
class VTGrafico;
class VTGrid;
class VTLegenda;
class TPadrao;
class TAtributo;

//---------------------------------------------------------------------------
class TFormPadrao : public TFormEdt
{
__published:   // IDE-managed Components
   TColorDialog *ColorDialog;
   TTreeView *tview;
   TSplitter *Splitter1;
   TAction *ActionAlteraPadrao;
   TAction *ActionAlteraCor;
   TAction *ActionSelecionaEqpto;
   TAction *ActionSelGridAlign;
   TAction *ActionSelGridCor;
   TAction *ActionSelGridDim;
   TAction *ActionSelGridVisible;
   TAction *ActionAtributoChanged;
   TAction *ActionAtributoAllEqpto;
   TAction *ActionAtributoSelAll;
   TAction *ActionAtributoSelNone;
   TAction *ActionSelEqpto;
   TAction *ActionTViewReinicia;
   TPanel *Panel;
   TGroupBox *gboxChave;
   TGroupBox *gboxChvNFDim;
   TComboBox *cboxChvDim;
   TGroupBox *gboxFundo;
   TPanel *panelBordaCor;
	TPanel *panelFundoCor;
   TGroupBox *gboxIsolado;
   TPanel *Panel1;
   TPanel *panelIsoladoCor;
   TGroupBox *gboxGrid;
   TGroupBox *gboxGridDim;
   TComboBox *cboxGridDim;
   TGroupBox *gboxGridCor;
   TPanel *panelGridCor;
   TGroupBox *gboxAlign;
   TComboBox *cboxGridAlign;
   TCheckBox *checkGridExibir;
   TGroupBox *gboxEqpto;
   TCheckListBox *clboxEqpto;
   TToolBar *ToolBarEqpto;
   TBitBtn *butSelAll;
   TBitBtn *butSelNone;
   TBitBtn *butCfgPadrao;
   TGroupBox *gboxBarra;
   TGroupBox *gboxBarDim;
   TComboBox *cboxBarDim;
   TGroupBox *gboxBarCor;
   TPanel *panelBarCor;
   TGroupBox *gboxTrecho;
   TGroupBox *gboxTreDim;
   TComboBox *cboxLigDim;
	TAction *ActionMudaCor;
	TGroupBox *gboxTrafo;
	TGroupBox *gboxTrfDim;
	TComboBox *cboxTrfDim;
   TRadioGroup *RadioGroupChvCor;
   TAction *ActionHelp;
   TToolButton *ToolButton1;
   TToolButton *ButReinicia;
   TToolButton *ButAjuda;
	TGroupBox *gboxLegenda;
	TRadioGroup *RadioGroupLegenda;
	TPanel *Panel3;
	TPanel *Panel4;
	TGroupBox *gboxCorLegenda;
	TPanel *panelLegendaCor;
	TAction *ActionSelLegendaCor;
	TFontDialog *FontDialog;
	TGroupBox *gboxComentario;
	TLabel *Label1;
	TLabel *Label2;
	TCheckBox *CheckBoxComentarioVisible;
	TRadioGroup *RadioGroupComentarioEstilo;
	TBitBtn *ButComentarioFundoCor;
	TBitBtn *ButComentarioFont;
	TBitBtn *ButComentarioBordaCor;
	TComboBox *CBoxBordaEspessura;
	TComboBox *CBoxBordaEstilo;
	TPanel *PanelFont;
	TImage *ImageFont;
	TAction *ActionComentarioFont;
	TAction *ActionComentarioFundoColor;
	TAction *ActionComentarioBordaCor;
	TAction *ActionComentarioBordaEspessura;
	TAction *ActionComentarioBordaEstilo;
   void __fastcall ActionAlteraCorExecute(TObject *Sender);
   void __fastcall ActionSelGridAlignExecute(TObject *Sender);
   void __fastcall ActionSelGridCorExecute(TObject *Sender);
   void __fastcall ActionSelGridDimExecute(TObject *Sender);
   void __fastcall ActionSelGridVisibleExecute(TObject *Sender);
   void __fastcall ActionAtributoChangedExecute(TObject *Sender);
   void __fastcall ActionAtributoAllEqptoExecute(TObject *Sender);
   void __fastcall ActionAtributoSelAllExecute(TObject *Sender);
   void __fastcall ActionAtributoSelNoneExecute(TObject *Sender);
   void __fastcall ActionSelEqptoExecute(TObject *Sender);
   void __fastcall ActionTViewReiniciaExecute(TObject *Sender);
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall tviewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall ActionAlteraPadraoExecute(TObject *Sender);
   void __fastcall tviewDblClick(TObject *Sender);
	void __fastcall ActionMudaCorExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
	void __fastcall RadioGroupLegendaClick(TObject *Sender);
	void __fastcall ActionSelLegendaCorExecute(TObject *Sender);
	void __fastcall ActionComentarioFontExecute(TObject *Sender);
	void __fastcall ActionComentarioFundoColorExecute(TObject *Sender);
	void __fastcall ActionComentarioBordaCorExecute(TObject *Sender);
	void __fastcall ActionComentarioBordaEspessuraExecute(TObject *Sender);
	void __fastcall ActionComentarioBordaEstiloExecute(TObject *Sender);
	void __fastcall CheckBoxComentarioVisibleClick(TObject *Sender);
	void __fastcall RadioGroupComentarioEstiloClick(TObject *Sender);

public:      // métodos
        __fastcall TFormPadrao(TForm* Owner, VTApl *apl, TWinControl *parent);
        __fastcall ~TFormPadrao(void);

private: //métodos
   void       __fastcall AtualizaAtributo(TAtributo *atributo);
   void       __fastcall AtualizaAtributoAllEqpto(TAtributo *atributo);
   void       __fastcall AtualizaAtributoEqpto(TAtributo *atributo);
   void       __fastcall AtualizaAtributoEqptoReducao(VTReducao* reducao);
   void       __fastcall CalculaDimensaoGrid(void);
   void       __fastcall CBoxGridDimInicia(void);
   void       __fastcall EqptoShowAsUnsigned(int eqpto_tipo, unsigned &checked, unsigned &grayed);
   void       __fastcall FormIniciaPosicao(void);
   void       __fastcall IniciaCBox(TComboBox *cbox, AnsiString opcao);
   void       __fastcall IniciaCLBoxEqpto(VTEqpto *eqpto, bool all_eqpto);
   void       __fastcall IniciaGBoxGrid(void);
   void       __fastcall IniciaGeral(void);
   void       __fastcall LeConfiguracao(void);
   void       __fastcall Moldura(VTEqpto *eqpto=NULL);
   TTreeNode* __fastcall TreeViewExisteEqpto(VTEqpto *eqpto);
   TTreeNode* __fastcall TreeViewExisteTipoEqpto(VTEqpto *eqpto);
   void       __fastcall TreeViewInicia(void);
   void       __fastcall TreeViewIniciaEqpto(TTreeNode *node_pai, TList *lisEQP);
   TTreeNode* __fastcall TreeViewInsereEqpto(VTEqpto *eqpto);
   void       __fastcall Zoom(VTEqpto *eqpto);

private:   //dados externos
   VTApl     *apl;
   TForm     *my_owner;
   VTGrafico *grafico;
   VTGrid    *grid;

private:   //dados locais
   struct   {
            TTreeNode *geral;
            TTreeNode *reducao;
            TTreeNode *eqpto;
            } node_raiz;
   VTEdt     *edt;
   VTLegenda *legenda;
   TList     *lisATR;
   TList     *lisEQP;
   TGroupBox *gbox_atual;
};

#endif
//---------------------------------------------------------------------------
//eof

