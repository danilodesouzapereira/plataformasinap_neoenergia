//---------------------------------------------------------------------------
#ifndef TFormEditaEstudoH
#define TFormEditaEstudoH

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
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEstudo;

//---------------------------------------------------------------------------
class TFormEditaEstudo : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TToolButton *butConfirma;
	TToolButton *butCancela;
	TGroupBox *GroupBox1;
	TEdit *EditNome;
	TGroupBox *GBoxJustificativa;
	TActionList *ActionList1;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TImageList *ImageList1;
	TRichEdit *RichEditJustificativa;
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TFormEditaEstudo(TComponent* Owner, VTApl *apl_owner, VTEstudo *estudo);
	__fastcall ~TFormEditaEstudo(void);

private: //obj externos
	VTEstudo *estudo;
	VTApl *apl;

};

//---------------------------------------------------------------------------
#endif
