//---------------------------------------------------------------------------
#ifndef TFormSelRedeEgridH
#define TFormSelRedeEgridH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTSub;
class TArqRede;

//---------------------------------------------------------------------------
class TFormSelRedeEgrid : public TForm
{
__published:	// IDE-managed Components
	TActionList *ActionList;
	TAction *ActionConfirmar;
	TAction *ActionCancelar;
	TToolBar *ToolBar;
	TToolButton *ButFecha;
	TToolButton *ToolButton5;
	TToolButton *ToolButton1;
	TImageList *ImageList;
	TGroupBox *GBoxSubestacaoAereo;
	TCheckListBox *CLBoxSubestacao;
	TToolBar *ToolBar2;
	TBitBtn *ButSubestacaoSelAll;
	TBitBtn *ButSubestacaoSelNone;
	TGroupBox *GBoxPrimario;
	TToolBar *ToolBar1;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TCheckListBox *CLBoxPrimario;
	TSplitter *Splitter2;
	TAction *ActionSubestacaoSelAll;
	TAction *ActionSubestacaoSelNone;
	TAction *ActionPrimarioSelAll;
	TAction *ActionPrimarioSelNone;
	void __fastcall ActionCancelarExecute(TObject *Sender);
	void __fastcall ActionConfirmarExecute(TObject *Sender);
	void __fastcall ActionPrimarioSelAllExecute(TObject *Sender);
	void __fastcall ActionPrimarioSelNoneExecute(TObject *Sender);
	void __fastcall ActionSubestacaoSelAllExecute(TObject *Sender);
	void __fastcall ActionSubestacaoSelNoneExecute(TObject *Sender);
	void __fastcall CLBoxPrimarioClickCheck(TObject *Sender);
	void __fastcall CLBoxSubestacaoClickCheck(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public:		// User declarations
	__fastcall TFormSelRedeEgrid(TComponent* Owner, VTApl *apl_owner, TArqRede *arq);
	__fastcall ~TFormSelRedeEgrid(void);

private:   // métodos
	void   __fastcall CheckAll(TCheckListBox *CLBox, bool checked);
	void   __fastcall CLBoxFilhasInicia(TList *lisFilhas);
	void   __fastcall CLBoxPrimarioInicia(void);
	void   __fastcall CLBoxSubestacaoInicia(void);
	bool   __fastcall ExistePriSel(TList *lisSub);
	VTSub* __fastcall ExisteSub(TList *lisSub, AnsiString cod_sub);
	void   __fastcall Inicia(void);
	void   __fastcall LeSubPriSelecionados(void);
	int    __fastcall LisObjectChecked(TCheckListBox *CLBox, TList *lisOBJ);

private:	// obj externos
	VTApl    *apl;
	TArqRede *arq;
	TList    *lisAllSub;

private: // obj locais
	TList *lisTMP;

};

//---------------------------------------------------------------------------
#endif
