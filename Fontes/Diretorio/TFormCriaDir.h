//---------------------------------------------------------------------------
#ifndef TFormCriaDirH
#define TFormCriaDirH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include <System.Actions.hpp>
//---------------------------------------------------------------------------
class TformCriaDir : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TGroupBox *gboxDir;
   TActionList *ActionList1;
   TAction *ActionConfirma;
   TAction *ActionCancela;
   TGroupBox *gbox;
   TEdit *edtDir;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TImageList *ImageList1;
   TPanel *Panel1;
   TLabel *Label1;
   TDriveComboBox *dcbDrive;
   TLabel *Label2;
   TFilterComboBox *fcbFilter;
   TFileListBox *flbFiles;
   TDirectoryListBox *dlbDir;
   TLabel *DirLabel;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   //void __fastcall dcbDriveChange(TObject *Sender);
   void __fastcall edtDirKeyPress(TObject *Sender, char &Key);
   void __fastcall dlbDirClick(TObject *Sender);

public:		// User declarations
              __fastcall  TformCriaDir(TComponent* Owner, AnsiString dir, AnsiString subdir);
              __fastcall ~TformCriaDir(void);
   AnsiString __fastcall  Dir(void);

private:	// User declarations
};

#endif
//---------------------------------------------------------------------------
//eof

