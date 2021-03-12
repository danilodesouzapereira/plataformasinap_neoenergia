//---------------------------------------------------------------------------
#ifndef TFormCfgH
#define TFormCfgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TFormEdt.h"
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
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEdt;
class VTEqpto;
class VTSinapServer;

//---------------------------------------------------------------------------
class TFormCfg : public TFormEdt
{
__published:	// IDE-managed Components
	TGroupBox *GBoxChave;
	TCheckListBox *CLBoxChave;
	TToolBar *ToolBarMontante;
   TBitBtn *ButChaveSelAll;
   TBitBtn *ButChaveSelNone;
	TAction *ActionCancela;
	TAction *ActionConfirma;
   TAction *ActionCapacitorSelAll;
   TAction *ActionCapacitorSelNone;
   TAction *ActionEqptoSelMouse;
	TToolBar *ToolBar1;
	TToolButton *ToolButton2;
	TToolButton *ButLeArqChave;
	TToolButton *ButArqChaveSalva;
	TAction *ActionArqChaveLe;
	TAction *ActionArqChaveSalva;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
   TGroupBox *GBoxTrafo;
   TToolBar *ToolBar2;
   TBitBtn *ButTrafoSelAll;
   TBitBtn *ButTrafoSelNone;
   TCheckListBox *CLBoxTrafo;
   TGroupBox *GBoxCapacitor;
   TToolBar *ToolBar3;
   TBitBtn *ButCapacitorSelAll;
   TBitBtn *ButCapacitorSelNone;
   TCheckListBox *CLBoxCapacitor;
   TAction *ActionChaveSelAll;
   TAction *ActionChaveSelNone;
   TAction *ActionTrafoSelAll;
   TAction *ActionTrafoSelNone;
   TToolButton *ToolButton1;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall ActionCapacitorSelAllExecute(TObject *Sender);
	void __fastcall ActionCapacitorSelNoneExecute(TObject *Sender);
	void __fastcall ActionEqptoSelMouseExecute(TObject *Sender);
	void __fastcall ActionEdtExecute(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ActionArqChaveLeExecute(TObject *Sender);
	void __fastcall ActionArqChaveSalvaExecute(TObject *Sender);
   void __fastcall CLBoxClick(TObject *Sender);
   void __fastcall CLBoxDblClick(TObject *Sender);
   void __fastcall ActionChaveSelAllExecute(TObject *Sender);
   void __fastcall ActionChaveSelNoneExecute(TObject *Sender);
   void __fastcall ActionTrafoSelAllExecute(TObject *Sender);
   void __fastcall ActionTrafoSelNoneExecute(TObject *Sender);

public:		// User declarations
		  __fastcall  TFormCfg(TComponent* Owner, VTApl *apl_owner, TWinControl *parent, TAction *ActionOnClose);
		  __fastcall ~TFormCfg(void);
	void __fastcall  Stop(void);

private:	//métodos
	void __fastcall  CLBoxCapacitorAtualiza(TList *lisCAP);
   void __fastcall  CLBoxCapacitorInicia(void);
	void __fastcall  CLBoxChaveAtualiza(TList *lisCHV);
	void __fastcall  CLBoxChaveInicia(void);
	void __fastcall  CLBoxTrafoAtualiza(TList *lisTRF);
	void __fastcall  CLBoxTrafoInicia(void);
	void __fastcall  LeArquivoSupervisaoAutomatica(AnsiString filename);
	void __fastcall  Moldura(VTEqpto *eqpto);
	void __fastcall  SalvaArquivoSupervisaoAutomatica(AnsiString filename);
	bool __fastcall  ValidaSelecao(void);

private:	//objetos externos
	TAction       *ActionOnClose;
	VTApl         *apl;
	VTSinapServer *server;

private:	//dados locais
	TList *lisEQP;
	VTEdt *edt;

};

//---------------------------------------------------------------------------
#endif
//eof
