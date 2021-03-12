//---------------------------------------------------------------------------
#ifndef TFormZdefH
#define TFormZdefH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <System.Actions.hpp>
#include <Vcl.Grids.hpp>
#include "..\RedeCC\ResCurto.h"

//---------------------------------------------------------------------------
class TFormZdef : public TForm
{
__published:	// IDE-managed Components
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TStringGrid *SGridZdef;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);

public:		// User declarations
        __fastcall  TFormZdef(TComponent* Owner);
        __fastcall ~TFormZdef(void);
   void __fastcall  DefineZdef(STR_ZDef &Zdef);
   void __fastcall  LeZdef(STR_ZDef &Zdef);

private:  //métodos
   bool __fastcall LeDados(void);
   void __fastcall SGridInicia(void);

private:  //dados locais
   STR_ZDef Zdef;
};

//---------------------------------------------------------------------------
#endif
//eof
