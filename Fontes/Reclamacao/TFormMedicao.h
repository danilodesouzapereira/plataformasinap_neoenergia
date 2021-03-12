//---------------------------------------------------------------------------
#ifndef TFormMedicaoH
#define TFormMedicaoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class VTApl;
//---------------------------------------------------------------------------
class TFormMedicao : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TImageList *ImageList1;
	TActionList *ActionList1;
	TAction *ActionConfirma;
	TAction *ActionCancela;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TAction *ActionSelArq;
	TComboBox *CBoxTipos;
	TListBox *LBoxLines;
	TGroupBox *GroupBox1;
	TOpenDialog *OpenDialog;
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionSelArqExecute(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// User declarations
	void __fastcall Inicia(void);

public:		// User declarations
	__fastcall TFormMedicao(TComponent* Owner, VTApl *apl);
	__fastcall ~TFormMedicao(void);

private: //obj externos
	VTApl *apl;

};
//---------------------------------------------------------------------------
extern PACKAGE TFormMedicao *FormMedicao;
//---------------------------------------------------------------------------
#endif
