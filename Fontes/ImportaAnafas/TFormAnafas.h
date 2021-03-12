//---------------------------------------------------------------------------
#ifndef TFormAnafasH
#define TFormAnafasH

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

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormAnafas : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar3;
   TToolButton *butSelArquivo;
   TToolButton *butImporta;
   TToolButton *butCancela;
   TActionList *ActionList;
   TAction *ActionImporta;
   TAction *ActionCancela;
   TAction *ActionSelArq;
   TImageList *ImageList1;
   TOpenDialog *OpenDialog;
   TRichEdit *redArq;
   TStatusBar *StatusBar;
   TToolButton *butConfirma;
   TAction *ActionConfirma;
   TToolButton *ToolButton1;
   void __fastcall ActionImportaExecute(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionSelArqExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);

public:		// User declarations
   __fastcall TFormAnafas(TComponent* Owner, VTApl *apl_owner);
   __fastcall ~TFormAnafas(void);

private:	// User declarations
   void __fastcall MontaResumo(void);
   void __fastcall FormIniciaPosicao(void);
   
private:	//objetos locais
   VTApl *apl;
   TList *lisEQP;
};

//---------------------------------------------------------------------------
#endif
