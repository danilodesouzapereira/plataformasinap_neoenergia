//---------------------------------------------------------------------------

#ifndef TFormCfgEdtMedidorH
#define TFormCfgEdtMedidorH
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
class TFormCfgEdtMedidor : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *SGridMedidor;
	TActionList *ActionList;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TImageList *ImageList;
	TToolBar *ToolBar1;
	TToolButton *butConfirma;
	TToolButton *butCancela;
	void __fastcall SGridMedidorSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall SGridMedidorSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);


private:	// dados
	enum nomeCOLUNA {ncCODIGO=0, ncPERDA };
	UnicodeString	valorAntigo;
	bool				salvo;

private:	// dados externos
	TList	 *LisPerdasMedidor;

private:	// User declarations
	bool  __fastcall IsRowEmpty(int row);
	bool  __fastcall FaltaParam(int row);
	bool  __fastcall ValidaValor(AnsiString valor, int col);
	void	__fastcall SalvaCfg(void);
	void	__fastcall DestroiItemsList(TList *listaPerda);

public:		// User declarations
			__fastcall TFormCfgEdtMedidor(TComponent* Owner);
			__fastcall ~TFormCfgEdtMedidor(void);
	void 	__fastcall Inicia(TList *LisPerdasMedidor);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormCfgEdtMedidor *FormCfgEdtMedidor;
//---------------------------------------------------------------------------
#endif
