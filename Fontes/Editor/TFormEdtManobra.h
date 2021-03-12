//---------------------------------------------------------------------------
#ifndef TFormEdtManobraH
#define TFormEdtManobraH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TFormEdt.h"
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTChave;
class VTEdt;
class VTEqpto;
class VTGrafico;
class VTLocked;
class VTManobra;
class VTManobras;

//---------------------------------------------------------------------------
class TFormEdtManobra : public TFormEdt
{
__published:	// IDE-managed Components
	TImageList *ImageList1;
	TActionList *ActionList1;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TAction *ActionInsereManobra;
	TAction *ActionRetiraManobra;
	TAction *ActionRetChave;
	TAction *ActionExecutaManobra;
	TAction *ActionAlteraEstadoChave;
	TAction *ActionExecutaManobraComplementar;
	TPopupMenu *PopupMenu1;
	TMenuItem *MenuManobraComplementar;
	TPanel *Panel1;
	TImage *ImageTeste;
	TLabel *Label1;
	TGroupBox *GBoxManobra;
	TListBox *LBox;
	TToolBar *ToolBar1;
	TToolButton *ButInsereManobra;
	TToolButton *ButRetiraManobra;
	TToolButton *ButExecutaManobra;
	TGroupBox *GBoxChave;
	TListView *LView;
	TToolBar *ToolBar2;
	TToolButton *ButRetiraChave;
	TToolButton *ButAlteraChave;
	TSplitter *Splitter2;
	TAction *ActionSelChave;
	void __fastcall ActionInsereManobraExecute(TObject *Sender);
	void __fastcall ActionRetiraManobraExecute(TObject *Sender);
	void __fastcall ActionRetChaveExecute(TObject *Sender);
	void __fastcall ActionExecutaManobraExecute(TObject *Sender);
	void __fastcall ActionAlteraEstadoChaveExecute(TObject *Sender);
	void __fastcall ActionExecutaManobraComplementarExecute(TObject *Sender);
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall ActionEdtExecute(TObject *Sender);
	void __fastcall LBoxClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ImageTesteMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ImageTesteMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall LBoxDblClick(TObject *Sender);
	void __fastcall LViewSelectItem(TObject *Sender, TListItem *Item, bool Selected);
	void __fastcall ActionSelChaveExecute(TObject *Sender);


public:		// User declarations
		   __fastcall TFormEdtManobra(TComponent* Owner, VTApl *apl, TAction *ActionOnDone, TWinControl *parent);
		   __fastcall ~TFormEdtManobra(void);
   void    __fastcall ChaveSelecionada(VTChave *chave);

private:	//métodos
   void       __fastcall ExibeMoldura(VTLocked *locked);
   VTManobra* __fastcall ExisteManobraSelecionada(void);
   VTLocked*  __fastcall ExisteLockedSelecionado(void);
   void       __fastcall FormIniciaPosicao(void);
   void       __fastcall LBoxInicia(VTManobra *manobra_sel=NULL);
   void       __fastcall LViewInicia(TList *lisLOCKED, VTLocked *locked_sel=NULL);

private:	//dados externos
   TAction    *ActionOnDone;
   VTApl      *apl;
   VTGrafico  *grafico;
   VTEqpto    *eqpto;
   VTManobras *manobras;

private:	//dados locais
   TList  *lisCHV;
   VTEdt  *edt;
   struct   {
            bool undo;
			} teste;
};

//---------------------------------------------------------------------------
#endif
