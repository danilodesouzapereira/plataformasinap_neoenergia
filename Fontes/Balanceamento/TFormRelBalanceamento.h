//---------------------------------------------------------------------------

#ifndef TFormRelBalanceamentoH
#define TFormRelBalanceamentoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TFormRelBalanceamento : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TImageList *ImageList;
	TActionList *ActionList1;
	TAction *ActionSalvar;
	TAction *ActionSair;
	TSaveDialog *SaveDialog;
	TRichEdit *RichEdit;
	TToolButton *ToolButton3;
	TToolButton *ToolButton4;
	TAction *ActionImprimir;
	TPrintDialog *PrintDialog;
	void __fastcall ActionSalvarExecute(TObject *Sender);
	void __fastcall ActionSairExecute(TObject *Sender);
	void __fastcall ActionImprimirExecute(TObject *Sender);
public:		// User declarations
	__fastcall TFormRelBalanceamento(TComponent* Owner, TStringList* relat);
	__fastcall ~TFormRelBalanceamento(void);

private:	// User declarations
	void __fastcall MontaRelatorio(TStringList* relat);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
