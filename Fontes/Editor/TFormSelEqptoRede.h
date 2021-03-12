//---------------------------------------------------------------------------
#ifndef TFormSelEqptoRedeH
#define TFormSelEqptoRedeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

class VTApl;

//---------------------------------------------------------------------------
class TFormSelEqptoRede : public TForm
{
__published:	// IDE-managed Components
	TActionList *ActionList;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TImageList *ImageList;
	TToolBar *ToolBar;
	TToolButton *ButConfirma;
	TToolButton *ButCancela;
	TGroupBox *GBoxTipoRede;
	TToolBar *ToolBarTipoRede;
	TBitBtn *ButTipoRedeAll;
	TBitBtn *ButTipoRedeNone;
	TAction *ActionTipoRedeAll;
	TAction *ActionTipoRedeNone;
	TAction *ActionRedeAll;
	TAction *ActionRedeNone;
	TAction *ActionEqptoAll;
	TAction *ActionEqptoNone;
	TGroupBox *GBoxRede;
	TCheckListBox *CLBoxTipoRede;
	TToolBar *ToolBarRede;
	TBitBtn *ButRedeAll;
	TBitBtn *ButRedeNone;
	TCheckListBox *CLBoxRede;
	TGroupBox *GBoxEqpto;
	TToolBar *ToolBarEqpto;
	TBitBtn *ButEqptoAll;
	TBitBtn *ButEqptoNone;
	TCheckListBox *CLBoxEqpto;
	TSplitter *Splitter1;
	TSplitter *Splitter2;
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall ActionEqptoAllExecute(TObject *Sender);
	void __fastcall ActionEqptoNoneExecute(TObject *Sender);
	void __fastcall ActionRedeAllExecute(TObject *Sender);
	void __fastcall ActionRedeNoneExecute(TObject *Sender);
	void __fastcall ActionTipoRedeAllExecute(TObject *Sender);
	void __fastcall ActionTipoRedeNoneExecute(TObject *Sender);
	void __fastcall CLBoxTipoRedeClickCheck(TObject *Sender);

public:		// User declarations
			 __fastcall  TFormSelEqptoRede(TComponent* Owner, VTApl *apl);
			 __fastcall ~TFormSelEqptoRede(void);
	TList* __fastcall  LisEqpto(void);

private:	//métodos
	void __fastcall CLBoxEqptoInicia(void);
	void __fastcall CLBoxRedeInicia(TList *lisEXT);
	void __fastcall CLBoxTipoRedeInicia(void);
	int  __fastcall DeterminaEqptosSelecionados(TList *lisEXT);
	int  __fastcall DeterminaRedesSelecionadas(TList *lisEXT);
   void __fastcall FormIniciaPosicao(void);

private:	//objetos externos
	VTApl *apl;

private:	//dados locais
	TList *lisEQP;
	TList *lisREDE;
};
//---------------------------------------------------------------------------
#endif
