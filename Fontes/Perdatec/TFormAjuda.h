//---------------------------------------------------------------------------
#ifndef TFormAjudaH
#define TFormAjudaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormAjuda : public TForm
{
__published:	// IDE-managed Components
   TRichEdit *RichEditHelp;
   TActionList *ActionList;
   TAction *ActionFechar;
   TImageList *ImageList;
   TToolBar *ToolBar;
   TToolButton *ToolButton1;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionFecharExecute(TObject *Sender);

public:		// User declarations
        __fastcall  TFormAjuda(TComponent* Owner, VTApl *apl);
        __fastcall ~TFormAjuda(void);
   bool __fastcall  CarregaArquivoAjuda(void);

private:	//métodos
   void __fastcall FormIniciaPosicao(void);

private:	//objetos externos
   VTApl *apl;
};

//---------------------------------------------------------------------------
#endif
//eof

