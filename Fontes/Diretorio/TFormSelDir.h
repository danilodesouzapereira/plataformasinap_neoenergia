//---------------------------------------------------------------------------
#ifndef TFormSelDirH
#define TFormSelDirH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>
//---------------------------------------------------------------------------
class TformSelDir : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList1;
   TAction *ActionConfirma;
   TAction *ActionCancela;
   TToolBar *ToolBar3;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TImageList *ImageList1;
   TPanel *panelDir;
   TDirectoryListBox *dlbDir;
   TFileListBox *flbFiles;
   TPanel *Panel1;
   TLabel *Label1;
   TLabel *Label2;
   TDriveComboBox *dcbDrive;
   TFilterComboBox *fcbFilter;
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TformSelDir(TComponent* Owner, AnsiString dir);
   AnsiString __fastcall Dir(void);
};

#endif
//---------------------------------------------------------------------------
//eof

