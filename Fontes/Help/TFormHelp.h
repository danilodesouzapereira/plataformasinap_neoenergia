//---------------------------------------------------------------------------
#ifndef TFormHelpH
#define TFormHelpH

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
#include <Vcl.FileCtrl.hpp>
#include <SHDocVw.hpp>
#include <Vcl.OleCtrls.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTGeral;
class VTPath;

//---------------------------------------------------------------------------
class TFormHelp : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
   TImageList *ImageList;
   TActionList *ActionList;
   TToolButton *ButClose;
   TAction *ActionClose;
   TListBox *LBoxSinap;
   TFileListBox *FileListBoxSinap;
   TWebBrowser *WebBrowser;
   TGroupBox *GBoxHelp;
   TPageControl *PageControl;
   TTabSheet *TabSheetSinap;
   TTabSheet *TabSheetStk;
   TListBox *LBoxStk;
   TFileListBox *FileListBoxStk;
   void __fastcall ActionCloseExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall LBoxSinapClick(TObject *Sender);
   void __fastcall LBoxStkClick(TObject *Sender);
   void __fastcall PageControlChange(TObject *Sender);

public:		// User declarations
   __fastcall  TFormHelp(TComponent *Owner, VTApl *apl_owner, AnsiString topico);
   __fastcall ~TFormHelp(void);

private:	//métodos
   void       __fastcall ExibeArquivo(TListBox *LBox, TFileListBox *FileLBox);
   void       __fastcall FormIniciaPosicao(void);
   void       __fastcall LBoxInicia(TListBox *LBox, TFileListBox *FileLBox, AnsiString topico);
   AnsiString __fastcall MontaNomeModulo(AnsiString topico);

private:	//objetos externos
   //VTApl   *apl;
   VTGeral *geral;
   VTPath  *path;

private:	//dados locais
   struct{//coordenadas p/ janela flutuante
         int top;
         int left;
         int width;
         int height;
         } janela;
};

//---------------------------------------------------------------------------
#endif
//eof
