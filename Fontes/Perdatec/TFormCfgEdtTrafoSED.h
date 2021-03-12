//---------------------------------------------------------------------------

#ifndef TFormCfgEdtTrafoSEDH
#define TFormCfgEdtTrafoSEDH
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
class TFormCfgEdtTrafoSED : public TForm
{
__published:	// IDE-managed Components
   TStringGrid *SGridTrafo;
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall SGridTrafoSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
   void __fastcall SGridTrafoSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);



private:	// dados
	enum nomeCOLUNA {ncSNOM=0, ncPERDAVAZIO, ncPERDATOTAL };
	UnicodeString	valorAntigo;
	bool				salvo;

private:	// dados externos
	TList	 *LisPerdasTrafo;
	TList  *LisPerdasTrafoTipo;

private:	// User declarations
	void  __fastcall ConcatenaListas(void);
	void	__fastcall DestroiItemsList(TList *listaPerda);
	bool  __fastcall FaltaParam(int row);
	bool  __fastcall IsRowEmpty(int row);
	void	__fastcall SalvaCfg(void);
	bool  __fastcall ValidaValor(AnsiString valor, int col);

public:		// User declarations
			__fastcall TFormCfgEdtTrafoSED(TComponent* Owner);
			__fastcall ~TFormCfgEdtTrafoSED(void);
	void 	__fastcall Inicia(TList *LisPerdasTrafo);

};

//---------------------------------------------------------------------------
#endif
