//---------------------------------------------------------------------------
#ifndef TFormCfgDemandaH
#define TFormCfgDemandaH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <System.ImageList.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.CheckLst.hpp>

//---------------------------------------------------------------------------
class VTAjusta;

//---------------------------------------------------------------------------
class TFormCfgDemanda : public TForm
	{
	__published:	// IDE-managed Components
		TToolBar *ToolBar;
		TToolButton *ButCancela;
		TToolButton *ButConfirma;
		TActionList *ActionList1;
		TAction *ActionCancelar;
		TAction *ActionConfirmar;
		TImageList *ImageList1;
		TGroupBox *GBoxOpcao;
		TCheckBox *CheckBoxConsA4;
		TCheckBox *CheckBoxRedeEquilibrada;
		TCheckBox *CheckBoxLimite;
		TGroupBox *GBoxLimite;
		TLabel *Label4;
		TLabel *Label5;
		TComboBox *CBoxAjusteMax;
		TComboBox *CBoxErroMax;
		TCheckBox *CheckBoxPerdaFe;
		TGroupBox *GBoxTempo;
		TLabel *Label1;
		TComboBox *CboxTempo;
		TCheckBox *CheckBoxTempo;
		TGroupBox *GroupBox1;
		TPanel *Panel1;
		TToolBar *ToolBar2;
		TBitBtn *ButSubestacaoSelAll;
		TBitBtn *ButSubestacaoSelNone;
		TCheckListBox *CLBoxPatamares;
	TAction *ActionSelAll;
	TAction *ActionSelNone;
	void __fastcall ActionCancelarExecute(TObject *Sender);
	void __fastcall ActionConfirmarExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CheckBoxLimiteClick(TObject *Sender);
	void __fastcall CheckBoxTempoClick(TObject *Sender);
	void __fastcall ActionSelAllExecute(TObject *Sender);
	void __fastcall ActionSelNoneExecute(TObject *Sender);

	private:	// User declarations
		bool __fastcall PatamaresSelecionados(void);

	public:		// User declarations
		__fastcall TFormCfgDemanda(TComponent* Owner, VTAjusta *ajusta_demanda);
		__fastcall ~TFormCfgDemanda(void);

	public:
		bool opFluxoAlterado;

	private: //obj externo
		VTAjusta *ajusta;

	private:
		bool incluir_perda_orig;
		int  tipo_fluxo_orig;
	};

//---------------------------------------------------------------------------
#endif
