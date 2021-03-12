//---------------------------------------------------------------------------
#ifndef TFormCfgCurtoH
#define TFormCfgCurtoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include "..\Geral\VTGeral.h"


//---------------------------------------------------------------------------
class VTBDado;
class VTGeral;
class TformCfgCurto : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *gbImpDef;
   TLabel *Label17;
   TLabel *Label18;
   TEdit *edtRdef;
   TEdit *edtXdef;
   TToolBar *ToolBar1;
   TToolButton *butDefault;
   TToolButton *butInclui;
   TToolButton *butSair;
   TActionList *ActionList1;
   TAction *ActionConfirma;
   TAction *ActionCancela;
   TAction *ActionDefault;
   TImageList *ImageList;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionDefaultExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
public:		// User declarations
   __fastcall TformCfgCurto(TComponent* Owner, AnsiString dirBase, AnsiString dirRede);
   __fastcall ~TformCfgCurto(void);

private:	// métodos
   void __fastcall ExibeDados(void);
   bool __fastcall LeDados(void);

private:	// dados
   VTBDado *bdado;
   VTGeral *geral;
   strCURTO CURTO;
};

#endif
//---------------------------------------------------------------------------
//eof
