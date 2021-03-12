//---------------------------------------------------------------------------
#ifndef TFormTUSDH
#define TFormTUSDH
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
#include <Vcl.Grids.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class VTApl;
//---------------------------------------------------------------------------
class TFormTUSD : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TActionList *ActionList;
	TAction *ActionConfirma;
	TAction *ActionCancela;
	TImageList *ImageList;
	TStringGrid *SGridTUSD;
	TGroupBox *GroupBox1;
   TRadioGroup *RadioGroupEUSD;
   TEdit *EditEUSD;
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);

public:		// User declarations
	__fastcall TFormTUSD(TComponent* Owner, VTApl *apl_owner);
	__fastcall ~TFormTUSD(void);

private:	// User declarations
	void __fastcall LeSGrid(void);
	void __fastcall LimpaSGrid(void);
	void __fastcall PreencheSGrid(void);
	bool __fastcall SalvaSGrid(void);

private: //obj. externo
	VTApl *apl;

private: //interno
	bool confirma;
};
//---------------------------------------------------------------------------
#endif
