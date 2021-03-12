//---------------------------------------------------------------------------
#ifndef TFormEdtAlternativaH
#define TFormEdtAlternativaH

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
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTAlternativa;

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422298
class TFormEdtAlternativa : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TEdit *EditCodigo;
   TGroupBox *GroupBox1;
   TRichEdit *RichEditJustif;
   TGroupBox *GroupBox2;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);


public:      // User declarations
   __fastcall  TFormEdtAlternativa(TComponent* Owner, VTAlternativa *alternativa);
   __fastcall ~TFormEdtAlternativa(void);

private:
	VTAlternativa *alternativa;

};

//---------------------------------------------------------------------------
#endif
//eof

