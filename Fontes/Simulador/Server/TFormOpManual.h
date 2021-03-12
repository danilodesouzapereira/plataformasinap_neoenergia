//---------------------------------------------------------------------------
#ifndef TFormOpManualH
#define TFormOpManualH

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

//---------------------------------------------------------------------------
class VTApl;
class VTEdt;
class VTChave;

//---------------------------------------------------------------------------
class TFormOpManual : public TFormEdt
{
__published:	// IDE-managed Components
	TGroupBox *GBoxChave;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TAction *ActionChaveSelAll;
	TAction *ActionChaveSelNone;
	TAction *ActionSelEqpto;
	TComboBox *CBoxChave;
	TListView *LViewChave;
	TGroupBox *GroupBox1;
	TRadioGroup *RadioGroupOperacao;
	TRadioGroup *RadioGroupCBTL;
	TBitBtn *ButConfirma;
	TBitBtn *ButCancela;
	TRadioGroup *RadioGroupCBR;
	TBitBtn *ButAltera;
	TAction *ActionAltera;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall ActionSelEqptoExecute(TObject *Sender);
	void __fastcall ActionEdtExecute(TObject *Sender);
	void __fastcall ActionAlteraExecute(TObject *Sender);
   void __fastcall CBoxChaveChange(TObject *Sender);

public:		// User declarations
		 __fastcall  TFormOpManual(TComponent* Owner, VTApl *apl_owner, TWinControl *parent, TAction *ActionOnClose);
		 __fastcall ~TFormOpManual(void);

private:	//métodos
	bool __fastcall AlteraChaveSelecionada(void);
	void __fastcall CBoxChaveInicia(void);
	void __fastcall LViewChaveInicia(VTChave *chave);

private:	//objetos externos
	VTApl   *apl;
	TAction *ActionOnClose;

private:	//dados locais
	TList *lisEQP;
	VTEdt *edt;

};

//---------------------------------------------------------------------------
#endif
//eof
