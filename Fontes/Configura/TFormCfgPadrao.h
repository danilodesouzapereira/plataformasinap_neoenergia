//---------------------------------------------------------------------------
#ifndef TFormCfgPadraoH
#define TFormCfgPadraoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>

//---------------------------------------------------------------------------
class TPad;
class VTApl;
class VTGeral;
class VTGrafico;
class VTGrid;

//---------------------------------------------------------------------------
class TformCfgPadrao : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList1;
   TAction *ActionAlteraPad;
   TColorDialog *ColorDialog;
   TAction *ActionAlteraCor;
   TAction *ActionAlteraExibe;
   TToolBar *toolBar;
   TAction *ActionSelecionaPad;
   TComboBox *cboxEqpto;
   TGroupBox *gboxBar;
   TGroupBox *gboxBarDim;
   TComboBox *cboxBarDim;
   TGroupBox *gboxBarCor;
   TPanel *panelBarCor;
   TGroupBox *gboxBarExibe;
   TComboBox *cboxBarExibe;
   TGroupBox *gboxSwing;
   TGroupBox *gboxSwingDim;
   TComboBox *cboxSwingDim;
   TGroupBox *gboxSwingCor;
   TPanel *panelSwingCor;
   TGroupBox *gboxSwingExibe;
   TComboBox *cboxSwingExibe;
   TGroupBox *gboxChvNA;
   TGroupBox *gboxChvNADim;
   TComboBox *cboxChvNADim;
   TGroupBox *gboxChvNACor;
   TPanel *panelChvNACor;
   TGroupBox *gboxChvNAExibe;
   TComboBox *cboxChvNAExibe;
   TGroupBox *gboxTrf;
   TGroupBox *gboxTrfDim;
   TComboBox *cboxTrfDim;
   TGroupBox *gboxTrfCor;
   TPanel *panelTrfCor;
   TGroupBox *gboxTrfExibe;
   TComboBox *cboxTrfExibe;
   TCheckBox *checkPieTrf;
   TGroupBox *gboxTrf3e;
   TGroupBox *gboxTrf3eDim;
   TComboBox *cboxTrf3eDim;
   TGroupBox *gboxTrf3eCor;
   TPanel *panelTrf3eCor;
   TGroupBox *gboxTrf3eExibe;
   TComboBox *cboxTrf3eExibe;
   TCheckBox *checkPieTrf3e;
   TGroupBox *gboxGer;
   TGroupBox *gboxGerDim;
   TComboBox *cboxGerDim;
   TGroupBox *gboxGerCor;
   TPanel *panelGerCor;
   TGroupBox *gboxGerExibe;
   TComboBox *cboxGerExibe;
   TCheckBox *checkPieGer;
   TGroupBox *gboxCap;
   TGroupBox *gboxCapDim;
   TComboBox *cboxCapDim;
   TGroupBox *gboxCapCor;
   TPanel *panelCapCor;
   TGroupBox *gboxCapExibe;
   TComboBox *cboxCapExibe;
   TGroupBox *gboxCar;
   TGroupBox *gboxCarDim;
   TComboBox *cboxCarDim;
   TGroupBox *gboxCarCor;
   TPanel *panelCarCor;
   TGroupBox *gboxCarExibe;
   TComboBox *cboxCarExibe;
   TGroupBox *gboxBorda;
   TPanel *panelBordaCor;
   TPanel *panelFundoCor;
   TGroupBox *gboxPad;
   TComboBox *cboxPad;
   TButton *butPadrao;
   TGroupBox *gboxChvNF;
   TGroupBox *gboxChvNFDim;
   TComboBox *cboxChvNFDim;
   TGroupBox *gboxChvNFCor;
   TPanel *panelChvNFCor;
   TGroupBox *gboxChvNFExibe;
   TComboBox *cboxChvNFExibe;
   TCheckBox *checkPieChvNF;
   TGroupBox *gboxTre;
   TGroupBox *gboxTreDim;
   TComboBox *cboxTreDim;
   TGroupBox *gboxTreCor;
   TPanel *panelTreCor;
   TGroupBox *gboxTreExibe;
   TComboBox *cboxTreExibe;
   TCheckBox *checkPieTre;
   TAction *ActionSelecionaEqpto;
   TGroupBox *gboxSup;
   TGroupBox *gboxSupDim;
   TComboBox *cboxSupDim;
   TGroupBox *gboxSupCor;
   TPanel *panelSupCor;
   TGroupBox *gboxSupExibe;
   TComboBox *cboxSupExibe;
   TCheckBox *checkPieSup;
   TGroupBox *gboxReducao;
   TGroupBox *gboxReducaoDim;
   TComboBox *cboxReducaoDim;
   TGroupBox *gboxReducaoCor;
   TPanel *panelReducaoCor;
   TGroupBox *gboxReducaoExibe;
   TComboBox *cboxReducaoExibe;
   TGroupBox *gboxGrid;
   TGroupBox *gboxGridDim;
   TComboBox *cboxGridDim;
   TGroupBox *gboxGridCor;
   TPanel *panelGridCor;
   TGroupBox *gboxAlign;
   TComboBox *cboxGridAlign;
   TCheckBox *checkGridExibir;
   TAction *ActionSelGridDim;
   TAction *ActionSelGridCor;
   TAction *ActionSelGridAlign;
   TAction *ActionSelGridVisible;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionAlteraPadExecute(TObject *Sender);
   void __fastcall ActionAlteraCorExecute(TObject *Sender);
   void __fastcall ActionAlteraExibeExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionSelecionaPadExecute(TObject *Sender);
   void __fastcall ActionSelecionaEqptoExecute(TObject *Sender);
   void __fastcall ActionSelGridDimExecute(TObject *Sender);
   void __fastcall ActionSelGridCorExecute(TObject *Sender);
   void __fastcall ActionSelGridAlignExecute(TObject *Sender);
   void __fastcall ActionSelGridVisibleExecute(TObject *Sender);

public:		// métodos
   __fastcall TformCfgPadrao(TForm* Owner, VTApl *apl, VTGrafico *graf);
   void __fastcall DefObjPadrao(TPad *pad);

private: //métodos
   void __fastcall IniciaCBox(TComboBox *cbox, AnsiString opcao);
   void __fastcall IniciaCBoxEqpto(void);
   void __fastcall IniciaGBoxGrid(void);
   void __fastcall IniciaTela(void);
   void __fastcall LeConfiguracao(void);

protected:	//dados
   //VTGeral   *geral;
   TForm     *my_owner;
   TPad      *pad;
   VTGrafico *graf;
   VTGrid    *grid;
   TGroupBox *gbox;
};

#endif
//---------------------------------------------------------------------------
//eof

