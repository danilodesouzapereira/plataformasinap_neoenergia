//---------------------------------------------------------------------------
#ifndef TFormTermoUsoH
#define TFormTermoUsoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <System.Actions.hpp>

class VTApl;
//---------------------------------------------------------------------------
class TFormTermoUso : public TForm
{
__published:	// IDE-managed Components
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionConfirma;
   TToolBar *ToolBar;
   TToolButton *butConfirma;
   TRichEdit *RichEdit;
   TCheckBox *CheckBox;
   TPanel *PanelToolBar;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall CheckBoxClick(TObject *Sender);

public:		// User declarations
   __fastcall  TFormTermoUso(TComponent* Owner, VTApl *apl, int tipo_licenca);
   __fastcall ~TFormTermoUso(void);

private:	// User declarations
};

//---------------------------------------------------------------------------
#endif
//eof
