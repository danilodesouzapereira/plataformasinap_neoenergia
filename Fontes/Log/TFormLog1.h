//---------------------------------------------------------------------------
#ifndef TFormLog1H
#define TFormLog1H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ToolWin.hpp>

//---------------------------------------------------------------------------
class TFormLog1 : public TForm
{
__published:	// IDE-managed Components
   TRichEdit *RichEdit;
	TToolBar *ToolBar;
	TToolButton *ButFecha;
	TToolButton *ButSalvar;
	TSaveDialog *SaveDialog;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ButSalvarClick(TObject *Sender);
	void __fastcall ButFechaClick(TObject *Sender);

public:		// User declarations
        __fastcall  TFormLog1(TComponent* Owner);
        __fastcall ~TFormLog1(void);
   void __fastcall  ExibeTexto(AnsiString arquivo);
   void __fastcall  ExibeTexto(TStrings *Lines);

private:	// User declarations
};

//---------------------------------------------------------------------------
#endif
//eof
