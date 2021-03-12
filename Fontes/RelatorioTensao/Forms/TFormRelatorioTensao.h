//---------------------------------------------------------------------------

#ifndef TFormRelatorioTensaoH
#define TFormRelatorioTensaoH
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
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class VTApl;
class TFaixaTensao;
class TRelatorioTensao;

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16082236
class TFormRelatorioTensao : public TFormEdt
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TAction *ActionConfigurar;
	TAction *ActionExportar;
	TAction *ActionFechar;
	TToolButton *ToolButton3;
	TRadioGroup *RadioGroupNivel;
	TGroupBox *GroupBox1;
	TComboBox *CBoxFaixa;
	TGroupBox *GroupBox2;
	TListView *LViewBarra;
	TSaveDialog *SaveDialog;
	void __fastcall ActionConfigurarExecute(TObject *Sender);
	void __fastcall ActionExportarExecute(TObject *Sender);
	void __fastcall ActionFecharExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CBoxFaixaChange(TObject *Sender);
	void __fastcall RadioGroupNivelClick(TObject *Sender);
	void __fastcall LViewBarraSelectItem(TObject *Sender, TListItem *Item, bool Selected);

public:		// User declarations
	__fastcall TFormRelatorioTensao(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
	__fastcall ~TFormRelatorioTensao(void);

private:
	void __fastcall CBoxFaixaInicia(void);
    void __fastcall FormIniciaPosicao(void);
	void __fastcall LViewBarraInicia(TFaixaTensao *faixa);

private: // dados externos
	VTApl *apl;

private: // dados locais
	TRelatorioTensao *relatorio_tensao;

};

//---------------------------------------------------------------------------
#endif
