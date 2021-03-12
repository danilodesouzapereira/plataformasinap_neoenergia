//---------------------------------------------------------------------------

#ifndef TFormCfgEdtRamalH
#define TFormCfgEdtRamalH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
//---------------------------------------------------------------------------
class TFormCfgEdtRamal : public TForm
{
__published:	// IDE-managed Components
	TImageList *ImageList;
	TActionList *ActionList;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TStringGrid *SGridRamal;
	TToolBar *ToolBar1;
	TToolButton *butConfirma;
	TToolButton *butCancela;
	void __fastcall SGridRamalSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
	void __fastcall SGridRamalSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);


private:	// dados
	enum nomeCOLUNA {ncTENSAO=0, ncFASES, ncCOMPRI, ncRESIST };
	UnicodeString	valorAntigo;
   bool				salvo;
private:	// dados externos
	TList	 *LisPerdasRamal;

private:	// User declarations
	bool  __fastcall IsRowEmpty(int row);
	bool  __fastcall FaltaParam(int row);
	bool  __fastcall ValidaValor(AnsiString valor, int col);
	void	__fastcall SalvaCfg(void);
	void	__fastcall DestroiItemsList(TList *listaPerda);

public:		// User declarations
			__fastcall TFormCfgEdtRamal(TComponent* Owner);
			__fastcall ~TFormCfgEdtRamal(void);
	void 	__fastcall Inicia(TList *LisPerdasRamal);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCfgEdtRamal *FormCfgEdtRamal;
//---------------------------------------------------------------------------
#endif
