// ---------------------------------------------------------------------------
#ifndef TFormCfgPerdatecH
#define TFormCfgPerdatecH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

// ---------------------------------------------------------------------------
class VTApl;
class VTCfgPerdaTec;

// ---------------------------------------------------------------------------
class TFormCfgPerdatec : public TForm
{
__published: // IDE-managed Components

	TLabel *Label4;
	TEdit *EditPerdaAdicional;
	TButton *Button3;
	TButton *Button4;
	TGroupBox *GBoxPerdaPorDentro;
	TCheckBox *CheckBoxPerdaFerroPorDentro;
	TCheckBox *CheckBoxPerdaRamalPorDentro;
	TCheckBox *CheckBoxPerdaMedidorPorDentro;
	TPanel *PanelLeft;
	TSplitter *Splitter1;
	TGroupBox *GBoxPerdaAdicional;
	TPanel *PanelRight;
	TSplitter *Splitter4;
	TRadioGroup *RadioGroupMedidor;
	TRadioGroup *RadioGroupRamal;
	TRadioGroup *RadioGroupTrafo;
	TPanel *PanelButtons;
	TGroupBox *GBoxFluxo;
	TCheckBox *CheckBoxFluxo;
	TPanel *PanelMedidor;
	TPanel *PanelRamal;
	TPanel *PanelTrafo;
	TComboBox *CBoxMedidor;
	TComboBox *CBoxRamal;
	TComboBox *CBoxTrafo;
	TRadioGroup *RadioGroupEdicao;
	TGroupBox *GBoxDados;
	TImageList *ImageList;
	TActionList *ActionList;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TAction *ActionEditaMedidor;
	TAction *ActionEditaRamal;
	TAction *ActionEditaTrafoMonoProdist;
	TPanel *PanelEdicao;
	TButton *ButEditaMedidor;
	TAction *ActionEditaTrafoTrifProdist;
	TAction *ActionEditaTrafoMonoEmpresa;
	TAction *ActionEditaTrafoTrifEmpresa;
	TButton *Button5;
	TButton *Button6;
	TButton *Button7;
	TButton *Button8;
	TButton *Button9;
	TCheckBox *CheckBoxNeutro;
	TEdit *EditZat;
	TListView *LViewMedidor;
	TListView *LViewRamal;
	TListView *LViewTrafo;
	TAction *ActionCfgProdist;
	TAction *ActionCfgGis1;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TCheckBox *CheckBoxZat;
	TComboBox *CBoxModeloCarga;
	TLabel *Label1;
	TCheckBox *CheckBoxCapacitor;
	TButton *Button1;
	TAction *ActionEditaTrafoSED;
	TRadioGroup *RadioGroupTrafoSED;
	TPanel *PanelTrafoSED;
	TButton *Button2;
	TAction *ActionCfgGis2;
	TButton *Button10;
	TAction *ActionCfgAnterior;

	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ActionAtualizaFormExecute(TObject *Sender);
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionEditaMedidorExecute(TObject *Sender);
	void __fastcall ActionEditaRamalExecute(TObject *Sender);
	void __fastcall ActionEditaTrafoMonoProdistExecute(TObject *Sender);
	void __fastcall ActionEditaTrafoTrifProdistExecute(TObject *Sender);
	void __fastcall ActionEditaTrafoMonoEmpresaExecute(TObject *Sender);
	void __fastcall ActionEditaTrafoTrifEmpresaExecute(TObject *Sender);
	void __fastcall RadioGroupEdicaoClick(TObject *Sender);
	void __fastcall ActionCfgAnteriorExecute(TObject *Sender);
	void __fastcall ActionCfgProdistExecute(TObject *Sender);
	void __fastcall ActionCfgGis1Execute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall RadioGroupMedidorClick(TObject *Sender);
	void __fastcall RadioGroupRamalClick(TObject *Sender);
	void __fastcall RadioGroupTrafoClick(TObject *Sender);
	void __fastcall CheckBoxZatClick(TObject *Sender);
	void __fastcall ActionEditaTrafoSEDExecute(TObject *Sender);
	void __fastcall ActionCfgGis2Execute(TObject *Sender);

public: // User declarations
	__fastcall TFormCfgPerdatec(TComponent* Owner, VTApl *apl_owner);
	__fastcall ~TFormCfgPerdatec(void);

private: // métodos
	void __fastcall CarregaParametros(void);
	void __fastcall CBoxMedidorInicia(void);
	void __fastcall CBoxRamalInicia(void);
	void __fastcall CBoxTrafoInicia(void);
	void __fastcall ExibeDados(void);
	void __fastcall FormIniciaPosicao(void);
	bool __fastcall LeDados(void);
	void __fastcall LViewIniciaMedidor(TList *LisPerdasMedidor);
	void __fastcall LViewIniciaRamal(TList *LisPerdasRamal);
	void __fastcall LViewIniciaTrafo(TList *PerdasTrafoProdist, int tipo_trf);
	void __fastcall LViewIniciaTrafoSED(TList *LisPerdasTrafoSED);
	void __fastcall SalvaParametros(void);
	int __fastcall ShowEdita(TListView *listView);
	bool __fastcall StrToDoubleLocal(AnsiString text, double &valor);

	// dados locais
	VTApl *apl;
	VTCfgPerdaTec *CfgPerdaTec;
};

// ---------------------------------------------------------------------------
#endif
// eof
