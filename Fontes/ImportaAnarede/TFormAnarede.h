//---------------------------------------------------------------------------
#ifndef TFormAnaredeH
#define TFormAnaredeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormAnarede : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar3;
   TActionList *ActionList;
   TAction *ActionConfirma;
   TAction *ActionCancela;
   TAction *ActionSelArq;
   TImageList *ImageList1;
   TOpenDialog *OpenDialog;
   TRichEdit *redArq;
   TStatusBar *StatusBar;
   TToolButton *ButSelArquivo;
   TToolButton *ButConfirma;
   TToolButton *ButCancela;
   TCheckBox *CheckBoxArea;
   TToolButton *ToolButton1;
   TEdit *EditArea;
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionSelArqExecute(TObject *Sender);
   void __fastcall CheckBoxAreaClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);

public:		// User declarations
   __fastcall TFormAnarede(TComponent* Owner, VTApl *apl_owner);
   __fastcall ~TFormAnarede(void);

private:	// User declarations
   void __fastcall ImportaCoordenadasEsquematico(void);
   void __fastcall FormIniciaPosicao(void);

private:	// User declarations
   VTApl *apl;
};

//---------------------------------------------------------------------------
#endif
