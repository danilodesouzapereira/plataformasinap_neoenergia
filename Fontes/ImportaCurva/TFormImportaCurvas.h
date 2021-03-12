//---------------------------------------------------------------------------
#ifndef TFormImportaCurvasH
#define TFormImportaCurvasH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Grids.hpp>
#include <CheckLst.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
class VTCurva;

//---------------------------------------------------------------------------
class TFormImportaCurvas : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TGroupBox *GroupBox6;
   TPanel *Panel1;
   TGroupBox *GroupBox1;
   TDriveComboBox *dcbDrive;
   TGroupBox *GroupBox2;
   TDirectoryListBox *DirectoryListBox;
   TPanel *Panel2;
   TGroupBox *GroupBox3;
   TFilterComboBox *FilterComboBox;
   TGroupBox *GroupBox4;
   TFileListBox *FileListBox;
   TGroupBox *GroupBox5;
   TComboBox *CBoxFatPot;
   TSplitter *Splitter1;
   TSplitter *Splitter2;
   TRichEdit *RichEdit;
   TPanel *Panel3;
   TButton *butImporta;
   TCheckListBox *CheckListBox;
   TAction *ActionImporta;
   TStringGrid *SGrid;
   TAction *ActionSGrid;
   TGroupBox *GroupBox7;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall DirectoryListBoxChange(TObject *Sender);
   void __fastcall ActionImportaExecute(TObject *Sender);
   void __fastcall ActionSGridExecute(TObject *Sender);

public:		// User declarations
   __fastcall TFormImportaCurvas(TComponent* Owner, VTApl *apl_owner);
   __fastcall ~TFormImportaCurvas(void);

private:	//métodos
   int  __fastcall AchaIndex(AnsiString arquivo);
   void __fastcall ExtraiCampos(AnsiString txt, TStringList *campos);
   void __fastcall IniciaLisCarga(void);
   void __fastcall IniciaLisCurva(void);
   bool __fastcall InserePontoNaCurva(int hora, int minuto, double i_amp);
   void __fastcall ReiniciaCurva(void);
   void __fastcall ReiniciaSGrid(void);

private:	//objeto externos
   VTApl   *apl;
   VTCarga *carga;
   VTCurva *curva;
private:	//dados locais
   TList       *lisCARGA;
   TList       *lisCURVA;
   TStringList *campos;
};

//---------------------------------------------------------------------------
#endif
//eof

