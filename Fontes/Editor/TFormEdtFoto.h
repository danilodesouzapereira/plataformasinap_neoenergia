//---------------------------------------------------------------------------
#ifndef TFormEdtFotoH
#define TFormEdtFotoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;

//---------------------------------------------------------------------------
class TFormEdtFoto : public TForm
{
__published:	// IDE-managed Components
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TToolBar *ToolBar2;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TGroupBox *GBoxImagem;
   TCheckListBox *CheckListBox1;
   TImage *Image;
   TFileListBox *FileListBox;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);

public:		// User declarations
   __fastcall TFormEdtFoto(TComponent* Owner, VTApl *apl, VTEqpto *eqpto);
   __fastcall ~TFormEdtFoto(void);

private:	// User declarations
   void  __fastcall FormIniciaPosicao(void);

private: //objetos externos
   VTApl     *apl;
};

//---------------------------------------------------------------------------
#endif
//eof
