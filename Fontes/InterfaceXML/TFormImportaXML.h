//---------------------------------------------------------------------------
#ifndef TFormImportaXMLH
#define TFormImportaXMLH

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
class VTCtrlPanel;
class VTImportaXML;

//---------------------------------------------------------------------------
class TFormImportaXML : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar3;
   TActionList *ActionList;
   TAction *ActionImporta;
   TAction *ActionCancela;
   TAction *ActionSelArq;
   TImageList *ImageList1;
   TOpenDialog *OpenDialog;
   TRichEdit *redArq;
   TStatusBar *StatusBar;
   TToolButton *ButSelArquivo;
   TToolButton *ButConfirma;
   TToolButton *ButFecha;
   TToolButton *ButImporta;
   TAction *ActionAvalia;
   TAction *ActionConfirma;
   TToolButton *ToolButton1;
   void __fastcall ActionImportaExecute(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionSelArqExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionAvaliaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);

public:      // User declarations
   __fastcall TFormImportaXML(TComponent* Owner, VTApl *apl_owner, AnsiString arq_xml);
   __fastcall ~TFormImportaXML(void);

private:   // User declarations
   void __fastcall AlteraSeparadorDecimal(AnsiString arq_xml);
   void __fastcall ExibeArquivo(void);
   void __fastcall FormIniciaPosicao(void);
   bool __fastcall SelecionaArquivo(void);

private:   //objetos externos
   VTApl *apl;

private:   //dados locais
   VTImportaXML *ImportaXML;
   VTCtrlPanel  *ctrlpanel;
};

//---------------------------------------------------------------------------
#endif
