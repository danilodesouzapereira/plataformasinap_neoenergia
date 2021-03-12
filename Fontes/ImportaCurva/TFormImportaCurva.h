//---------------------------------------------------------------------------
#ifndef TFormImportaCurvaH
#define TFormImportaCurvaH

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
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
class VTCurva;

//---------------------------------------------------------------------------
class TFormImportaCurva : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TPanel *Panel1;
   TPanel *Panel2;
   TGroupBox *GroupBox1;
   TGroupBox *GroupBox2;
   TGroupBox *GroupBox3;
   TGroupBox *GroupBox4;
   TPageControl *PageControl;
   TTabSheet *TabSheetPonto;
   TTabSheet *TabSheetGrafio;
   TFilterComboBox *FilterComboBox;
   TDriveComboBox *dcbDrive;
   TDirectoryListBox *DirectoryListBox;
   TFileListBox *FileListBox;
   TTabSheet *TabSheetArquivo;
   TRichEdit *RichEdit;
   TStringGrid *SGrid;
   TTabSheet *TabSheetDefault;
   TGroupBox *GroupBox5;
   TComboBox *CBoxFatPot;
   TSplitter *Splitter1;
   TSplitter *Splitter2;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FileListBoxChange(TObject *Sender);

public:		// User declarations
   __fastcall TFormImportaCurva(TComponent* Owner, VTApl *apl_owner,
                                VTCarga *carga_orig, VTCurva *curva_orig);
   __fastcall ~TFormImportaCurva(void);

private:	//métodos
   void __fastcall ExtraiCampos(AnsiString txt, TStringList *campos);
   void __fastcall ReiniciaChart(void);
   void __fastcall ReiniciaCurva(void);
   void __fastcall ReiniciaSGrid(void);
   bool __fastcall InserePontoNaCurva(int hora, int minuto, double i_amp);

private:	//objeto externos
   VTApl   *apl;
   VTCarga *carga_orig;
   VTCurva *curva_orig;

private:	//dados locais
   TStringList *campos;
   VTCurva     *curva;
};

//---------------------------------------------------------------------------
#endif
//eof

