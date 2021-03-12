// ---------------------------------------------------------------------------
#ifndef TFormTesteBateriaH
#define TFormTesteBateriaH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

// ---------------------------------------------------------------------------
class VTApl;
class VTBusca;
class VTArea;
class VTEdt;
class VTCurva;

// ---------------------------------------------------------------------------
class TFormTesteBateria : public TForm
{
__published: // IDE-managed Components

	TToolBar *ToolBarLeft;
	TPanel *panelMouse;
	TImage *ImageMouseOff;
	TImage *ImageMouseOn;
	TToolButton *butAlign;
	TToolButton *butEdt;
	TActionList *ActionList;
	TAction *ActionAlignDown;
	TAction *ActionAlignLeft;
	TAction *ActionAlignNone;
	TAction *ActionAlignRight;
	TAction *ActionAlignUp;
	TAction *ActionEdt;
	TAction *ActionMouseOff;
	TAction *ActionMouseOn;
	TImageList *ImageList;
	TPopupMenu *PopupMenu;
	TMenuItem *mnuAlignLeft;
	TMenuItem *mnuAlignRight;
	TMenuItem *mnuAlignDown;
	TMenuItem *mnuAlignUp;
	TMenuItem *mnuAlignNode;
	TPanel *PanelToolBar;
	TPanel *PanelLeft;
	TPanel *PanelRight;
	TAction *ActionSelEqpto;
	TPanel *Panel1;

	void __fastcall ActionAlignDownExecute(TObject *Sender);
	void __fastcall ActionAlignLeftExecute(TObject *Sender);
	void __fastcall ActionAlignNoneExecute(TObject *Sender);
	void __fastcall ActionAlignRightExecute(TObject *Sender);
	void __fastcall ActionAlignUpExecute(TObject *Sender);
	void __fastcall ActionEdtExecute(TObject *Sender);
	void __fastcall ActionMouseOffExecute(TObject *Sender);
	void __fastcall ActionMouseOnExecute(TObject *Sender);
	void __fastcall ActionSelEqptoExecute(TObject *Sender);


public: // User declarations
	__fastcall TFormTesteBateria(TComponent* Owner, TWinControl *parent, VTApl *apl);
	__fastcall ~TFormTesteBateria(void);
	virtual void __fastcall Ativo(void);
	virtual void __fastcall Atualiza(void);
	void __fastcall DefParent(TWinControl *parent);
	VTCurva* __fastcall InsereCurvaBateria(void);
	VTCurva* __fastcall InsereValoresCurvaBateria(VTCurva *curva);

protected: // métodos
	void __fastcall AtualizaIndicacaoEditorOff(void);
	void __fastcall AtualizaIndicacaoEditorOn(TToolButton *button);
	void __fastcall Centraliza(void);

	// dados locais
	TToolButton *button_down;
	VTApl *apl;
	VTBusca *busca;
	VTArea *area;
	VTEdt *edt;

	struct
	{ // coordenadas p/ janela flutuante
		int top;
		int left;
		int width;
		int height;
	}

	janela;
};

// ---------------------------------------------------------------------------
#endif
// eof
