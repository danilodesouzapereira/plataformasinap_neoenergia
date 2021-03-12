//---------------------------------------------------------------------------

#ifndef TFormCustomImportaH
#define TFormCustomImportaH
#include "EstruturaMed.h"
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ActnCtrls.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Grids.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ImgList.hpp>
//---------------------------------------------------------------------------
class VTApl;
class TMedidores;
//---------------------------------------------------------------------------
class TFormCustomImporta : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *OpenDialogMed;
	TGroupBox *GroupBox1;
	TStringGrid *sgFilePreview;
	TEdit *edData;
	TLabel *Label1;
	TLabel *Label3;
	TEdit *edV1;
	TLabel *Label4;
	TEdit *edHora;
	TLabel *Label7;
	TEdit *edV2;
	TLabel *Label8;
	TEdit *edV3;
	TToolBar *ToolBar1;
	TToolButton *tbSalva;
	TToolButton *tbCancela;
	TImageList *ImageList;
	TActionList *ActionList1;
	TAction *ActionSalvar;
	TAction *ActionCancel;
	TEdit *edNome;
	TLabel *Label2;
	void __fastcall ActionSalvarExecute(TObject *Sender);
	void __fastcall ActionCancelExecute(TObject *Sender);

private:	//objetos externos
	VTApl *apl;
	TMedidores *mMedidores;

private:	//dados
	strMEDIDOR strMedidor;

private:	// User declarations
   void __fastcall Inicia(void);
	bool __fastcall LeArqMedicao(AnsiString arquivo);
	bool __fastcall PreencheGrid(AnsiString filename);

public:		// User declarations
	__fastcall TFormCustomImporta(TComponent* Owner, VTApl *apl_owner, TMedidores *medidores);
	__fastcall ~TFormCustomImporta(void);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormCustomImporta *FormCustomImporta;
//---------------------------------------------------------------------------
#endif
