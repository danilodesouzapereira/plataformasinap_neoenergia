//---------------------------------------------------------------------------
#ifndef TFormAlertaH
#define TFormAlertaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormAlerta : public TForm
{
__published:	// IDE-managed Components
   TRichEdit *RichEdit;
   TToolBar *ToolBar;
   TToolButton *ButExportaTxt;
   TToolButton *ButFecha;
   TSaveDialog *SaveDialog;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionFecha;
   TAction *ActionSalva;
	TToolButton *ButClear;
	TAction *ActionClear;
	TToolButton *ToolButton1;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionSalvaExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
	void __fastcall ActionClearExecute(TObject *Sender);

public:		// User declarations
        __fastcall  TFormAlerta(TComponent* Owner, VTApl *apl_owner);
        __fastcall ~TFormAlerta(void);
   void __fastcall  ExibeTexto(AnsiString arquivo);
   void __fastcall  ExibeTexto(TStrings *Lines);

private:	//objetos exeternos
   VTApl *apl;
};

//---------------------------------------------------------------------------
#endif
//eof
