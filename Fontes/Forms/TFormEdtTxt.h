//---------------------------------------------------------------------------
#ifndef TFormEdtTxtH
#define TFormEdtTxtH

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
class TFormEdtTxt : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TEdit *Edit;
   TLabel *Label;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall EditKeyPress(TObject *Sender, char &Key);

public:   //property
   __property AnsiString Texto = {read=PM_GetTexto, write=PM_SetTexto};

public:      // User declarations
   __fastcall  TFormEdtTxt(TComponent* Owner, AnsiString caption);
   __fastcall ~TFormEdtTxt(void);

private: // User declarations
   AnsiString __fastcall PM_GetTexto(void);
   void       __fastcall PM_SetTexto(AnsiString texto);
};

//---------------------------------------------------------------------------
#endif
//eof

