// ---------------------------------------------------------------------------

#ifndef TFormInsereConjuntoH
#define TFormInsereConjuntoH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.CheckLst.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>

// ---------------------------------------------------------------------------
class VTApl;
class VTCalcIndCont;

// ---------------------------------------------------------------------------
class TFormInsereConjunto : public TForm
{
__published: // IDE-managed Components

	TPanel *pnlBackGround;
	TActionList *ActionList;
	TAction *ActionConfirmaCadastro;
	TAction *ActionCancelaCadastro;
	TImageList *ImageList;
	TToolBar *ToolBar1;
	TToolButton *tbtConfirma;
	TToolButton *tbtCancela;
	TGroupBox *grbConjunto;
	TRichEdit *edtCodigo;
	TLabel *Label1;
	TLabel *lblDecVerif;
	TLabel *lblFecVerif;
	TLabel *lblDecAjustado;
	TLabel *lblFecAjustado;
	TGroupBox *grpVerifSe;
	TCheckListBox *clbVerifSe;
	TRichEdit *edtDecVerif;
	TRichEdit *edtFecVerif;
	TRichEdit *edtDecAjustado;
	TRichEdit *edtFecAjustado;

	void __fastcall ActionConfirmaCadastroExecute(TObject *Sender);
	void __fastcall ActionCancelaCadastroExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public: // User declarations

	__fastcall TFormInsereConjunto(TComponent* Owner, VTApl* apl, TList *lisVerifSe, VTCalcIndCont* calcIndCont);
	__fastcall ~TFormInsereConjunto(void);

private:

	void __fastcall ClbVerifSeInicia();

private:

	VTApl* apl;
	VTCalcIndCont* calcIndCont;

	TList *lisVerifSe;

	AnsiString codigo;
	double decAjust;
	double decVerif;
	double fecAjust;
	double fecVerif;
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormInsereConjunto *FormInsereConjunto;
// ---------------------------------------------------------------------------
#endif
