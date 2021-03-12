//---------------------------------------------------------------------------

#ifndef TformParCaboH
#define TformParCaboH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>

class TCabo;
//---------------------------------------------------------------------------
class TformParCabo : public TForm
{
__published:	// IDE-managed Components
   TEdit *edtCabo;
   TEdit *edtIadm;
   TEdit *edtR0;
        TColorDialog *dlgSelCor;
   TEdit *edtX0;
   TEdit *edtC0;
   TEdit *edtR1;
   TEdit *edtC1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
   TImageList *ImageList;
   TToolBar *ToolBar1;
   TToolButton *butDefault;
   TPanel *pnlCor;
   TLabel *Label8;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionDefault;
   TToolButton *ToolButton1;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall edtIadmKeyPress(TObject *Sender, char &Key);
   void __fastcall pnlCorClick(TObject *Sender);

private:	// User declarations
   TCabo *pCabo;
   TCabo *oldCabo;

private:	// User declarations
   void __fastcall loadCabo(void);
   bool __fastcall saveCabo(void);

public:		// User declarations
   __fastcall TformParCabo(TComponent* Owner, TCabo *pCabo);
 };
//---------------------------------------------------------------------------
//extern PACKAGE TformParCabo *formParCabo;
//---------------------------------------------------------------------------
#endif
