//---------------------------------------------------------------------------
#ifndef TFormLoginH
#define TFormLoginH

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
class TFormLogin : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar2;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TGroupBox *GroupBox1;
   TEdit *EditLogin;
   TGroupBox *GroupBox2;
   TEdit *EditPassword;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall EditKeyPress(TObject *Sender, char &Key);

public:		// User declarations
              __fastcall TFormLogin(TComponent* Owner);
              __fastcall ~TFormLogin(void);
   AnsiString __fastcall GetLogin(void);
   AnsiString __fastcall GetPassword(void);

private:	// User declarations
};

//---------------------------------------------------------------------------
#endif
//eof
