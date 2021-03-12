//---------------------------------------------------------------------------

#ifndef TFormRecondutorarH
#define TFormRecondutorarH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TFormEdt.h"
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEdt;
class VTEqpto;
class TRecondutorar;

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14217067
class TFormRecondutorar : public TFormEdt
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TAction *ActionFechar;
	TAction *ActionSelEqpto;
	TRadioGroup *RadioGroupOpSel;
	TGroupBox *GroupBox1;
	TLabel *LabelCompTotal;
	TPanel *PanelColorSel;
	TGroupBox *GroupBox2;
	TComboBox *CBoxArranjo;
	TGroupBox *GBoxPatamar;
	TComboBox *CBoxPatamar;
	TColorDialog *ColorDialogSel;
	TToolButton *ToolButton2;
	TAction *ActionAplicar;
	TAction *ActionAlteraCorSel;
	void __fastcall ActionFecharExecute(TObject *Sender);
	void __fastcall ActionEdtExecute(TObject *Sender);
	void __fastcall ActionSelEqptoExecute(TObject *Sender);
	void __fastcall ActionAplicarExecute(TObject *Sender);
	void __fastcall ActionAlteraCorSelExecute(TObject *Sender);
	void __fastcall CBoxPatamarChange(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall RadioGroupOpSelClick(TObject *Sender);

public:		// User declarations
	__fastcall TFormRecondutorar(TComponent* Owner, VTApl *apl, TAction *ActionOnDone, TWinControl *parent);
    __fastcall ~TFormRecondutorar(void);

private:	// User declarations
    void __fastcall AtualizaSelecao(void);
	void __fastcall CBoxArranjoInicia(void);
	void __fastcall CBoxPatamarInicia(void);
	void __fastcall DestacaEqpto(void);

private:	//objetos externos
	TAction *ActionOnDone;
	VTApl   *apl;
	VTEqpto *eqpto;

private:	//dados locais
	TList         *lisEQP;
	TRecondutorar *recondutorar;
	VTEdt         *edt;
};

//---------------------------------------------------------------------------
#endif
