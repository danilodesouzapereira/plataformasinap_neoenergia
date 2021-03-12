//---------------------------------------------------------------------------

#ifndef TFormCfgEdtTrafoH
#define TFormCfgEdtTrafoH
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
class TFormCfgEdtTrafo : public TForm
{
__published:	// IDE-managed Components
	TImageList *ImageList;
	TActionList *ActionList;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TStringGrid *SGridTrafo;
	TToolBar *ToolBar1;
	TToolButton *butConfirma;
	TToolButton *butCancela;
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall SGridTrafoSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
	void __fastcall SGridTrafoSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);

private:	// dados
	enum nomeCOLUNA {ncTENSAO=0, ncSNOM, ncPERDAVAZIO, ncPERDATOTAL };
	UnicodeString	valorAntigo;
	bool				salvo;

private:	// dados externos
	TList	 *LisPerdasTrafo;
	TList  *LisPerdasTrafoTipo;
	TList	 *LisPerdasTrafoTipoDIFF;
	int	 tipo_trf;

private:	// User declarations
	void  __fastcall ConcatenaListas(void);
	void	__fastcall DestroiItemsList(TList *listaPerda);
	bool  __fastcall FaltaParam(int row);
	bool  __fastcall IsRowEmpty(int row);
	void	__fastcall SalvaCfg(void);
	bool  __fastcall ValidaValor(AnsiString valor, int col);

public:		// User declarations
			__fastcall TFormCfgEdtTrafo(TComponent* Owner);
			__fastcall ~TFormCfgEdtTrafo(void);
	void 	__fastcall Inicia(TList *LisPerdasTrafo, int tipo_trf);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCfgEdtTrafo *FormCfgEdtTrafo;
//---------------------------------------------------------------------------
#endif
