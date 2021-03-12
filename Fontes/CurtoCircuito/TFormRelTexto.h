//---------------------------------------------------------------------------

#ifndef TFormRelTextoH
#define TFormRelTextoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormRelTexto : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
   TRichEdit *RichEdit;
   TSaveDialog *SaveDialog;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TActionList *ActionList;
   TAction *ActionClose;
   TAction *ActionExporta;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionCloseExecute(TObject *Sender);
   void __fastcall ActionExportaExecute(TObject *Sender);

public:		// User declarations
        __fastcall  TFormRelTexto(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormRelTexto(void);
   void __fastcall Atualiza(AnsiString arq_texto);

private:	//métodos
   void  __fastcall ExportaRelatorio(void);

private:	//objetos externos
   VTApl *apl;

};

//---------------------------------------------------------------------------
#endif
//eof
