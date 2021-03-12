//---------------------------------------------------------------------------
#ifndef TFormImportaBarrasH
#define TFormImportaBarrasH

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
class VTBarra;

//---------------------------------------------------------------------------
class TFormImportaBarras : public TForm
{
__published:	// IDE-managed Components
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionSelArq;
   TToolBar *ToolBar1;
   TToolButton *butCancela;
   TToolButton *ToolButton1;
   TOpenDialog *OpenDialog;
   TToolButton *ToolButton2;
   TGroupBox *GroupBox1;
   TEdit *EditVnom;
   void __fastcall ActionSelArqExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);

private:	// User declarations
   VTBarra* __fastcall CriaBarra(TStringList *campos);
   void     __fastcall FormIniciaPosicao(void);
   void     __fastcall ImportaBarras(AnsiString arq_txt);
   void     __fastcall Inicia(void);

public:		// User declarations
   __fastcall TFormImportaBarras(TComponent* Owner, VTApl *apl_owner);
   __fastcall ~TFormImportaBarras(void);

public: //obj externo
   VTApl *apl;

private:
   double      vnom_kv;
   TList       *lisBARRAS;
   TStringList *linhas, *campos;

};

//---------------------------------------------------------------------------
#endif
