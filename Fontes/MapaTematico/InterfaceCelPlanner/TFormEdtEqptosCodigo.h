//---------------------------------------------------------------------------

#ifndef TFormEdtEqptosCodigoH
#define TFormEdtEqptosCodigoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TFormEdt.h"
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
//---------------------------------------------------------------------------
class TFormEdtEqptosCodigo : public TFormEdt
{
__published:	// IDE-managed Components
	TGroupBox *gboxListaEqptos;
	TListView *lviewEqptos;
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TAction *ActionOnDone;
	void __fastcall ToolButton1Click(TObject *Sender);
	void __fastcall ActionOnDoneExecute(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormEdtEqptosCodigo(TComponent* Owner, TWinControl *parent, TList *lisEqptos);
	__fastcall ~TFormEdtEqptosCodigo(void);

private:
	void __fastcall PreencheListView(void);

	TList *mListEqptos;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEdtEqptosCodigo *FormEdtEqptosCodigo;
//---------------------------------------------------------------------------
#endif
