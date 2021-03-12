// --------------------------------------------------------------------------------------------- bof
#ifndef TFormImportadorUgridH
#define TFormImportadorUgridH

// ---------------------------------------------------------------------------------------- includes
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Dialogs.hpp>
#include <System.Actions.hpp>
#include <System.Classes.hpp>
#include <System.ImageList.hpp>
#include <map>

// Plataforma include
#include <Fontes\ImportaRedeGis\VTImportaRedeGis.h>

// Project include
#include "..\Ugrid\VTImportador.h"
#include "..\Ugrid\VTConversor.h"

// ------------------------------------------------------------------------------------ declarations

// Class declaration
class VTApl;
class VTCronometro;
class VTCommons;
class VTConversor;
class VTImportador;
struct strOPImporta;
class VTProgresso;
class VTScg;

// ------------------------------------------------------------------------------------------- class
class TFormImportadorUgrid : public TForm
{
__published: // IDE-managed Components
	TToolBar *ToolBar;
	TToolButton *btnReiniciarRede;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TToolButton *ButFecha;
	TPanel *PanelOpcao;
	TRadioGroup *RadioGroupDestino;
	TRadioGroup *RadioGroupRedeSec;
	TRadioGroup *RadioGroupConsPrimario;
	TGroupBox *GBoxCondutor;
	TLabel *Label1;
	TLabel *Label2;
	TComboBox *CBoxCondutorMT;
	TComboBox *CBoxCondutorBT;
	TGroupBox *GBoxLog;
	TCheckBox *CheckBoxLog;
	TCheckBox *CheckBoxCronometro;
	TGroupBox *GBoxBaseDado;
	TPanel *PanelData;
	TLabel *LabelData;
	TDateTimePicker *DateTimePicker;
	TGroupBox *GBoxPastaPublica;
	TEdit *EditPastaPublica;
	TBitBtn *ButPastaPublica;
	TGroupBox *GroupBox2;
	TCheckBox *CheckBoxValidaFase;
	TCheckBox *CheckBoxCorrigeFase;
	TCheckBox *CheckBoxCorrigePerdaFerro;
	TGroupBox *GBoxSubestacaoAereo;
	TCheckListBox *CLBoxSubestacao;
	TToolBar *ToolBar2;
	TBitBtn *ButSubestacaoSelAll;
	TBitBtn *ButSubestacaoSelNone;
	TActionList *ActionList;
	TAction *ActionIniciarImportacao;
	TAction *ActionRedeClear;
	TAction *ActionSelPastaPublica;
	TAction *ActionSubestacaoSelAll;
	TAction *ActionSubestacaoSelNone;
	TAction *ActionPrimarioSelAll;
	TAction *ActionPrimarioSelNone;
	TAction *ActionHelp;
	TAction *ActionFecha;
	TImageList *ImageList;
	TToolButton *btnAbrir;
	TAction *ActionAbrirUgrid;
	TToolButton *btnImportar;
	TOpenDialog *dlgOpen;
	TPanel *pnlRedePri;
	TPanel *pnlRedeSec;
	TSplitter *Splitter2;
	TSplitter *Splitter3;
	TGroupBox *GBoxPrimario;
	TPanel *pnlCaboRamal;
	TLabel *Label3;
	TComboBox *CBoxCondutorRL;
	TPanel *pnlCaboIP;
	TLabel *Label4;
	TComboBox *CBoxCondutorIP;
	TPanel *PanelSelecao;
	TCheckListBox *CLBoxPrimario;
	TToolBar *ToolBar1;
	TBitBtn *ButPrimarioSelAll;
	TBitBtn *ButPrimarioSelNone;
	TGroupBox *GBoxMalha;
	TCheckBox *CheckBoxAbrirChaveExt;
	TCheckBox *CheckBoxAbrirChaveInt;
	TCheckBox *CheckBoxDeslocarTrechoInt;
	TCheckBox *CheckBoxDeslocarTrechoExt;

	void __fastcall ActionAbrirUgridExecute(TObject *Sender);
	void __fastcall ActionRedeClearExecute(TObject *Sender);
	void __fastcall ActionIniciarImportacaoExecute(TObject *Sender);
	void __fastcall ActionFechaExecute(TObject *Sender);
	void __fastcall ActionHelpExecute(TObject *Sender);
	void __fastcall CLBoxSubestacaoClickCheck(TObject *Sender);
	void __fastcall ActionSelPastaPublicaExecute(TObject *Sender);
	void __fastcall ActionSubestacaoSelAllExecute(TObject *Sender);
	void __fastcall ActionSubestacaoSelNoneExecute(TObject *Sender);
	void __fastcall ActionPrimarioSelAllExecute(TObject *Sender);
	void __fastcall ActionPrimarioSelNoneExecute(TObject *Sender);
	void __fastcall CLBoxPrimarioClickCheck(TObject *Sender);


private: // Variáveis locais

	VTImportador* importadorArquivo;
	VTApl *apl;
	VTCronometro *cronometro;
	VTConversor *conversor;
	TList *lisTMP;
	strOPImporta opcao;
	VTCommons *commons;
	VTProgresso *progresso;
	VTScg *scg;
    bool arqEgrid;
	// métodos

	void __fastcall CBoxCondutorInicia(TComboBox *CBox, std::map<int,TArranjoBDGD*>mapArranjo, int arranjo_id);
	void __fastcall CheckAll(TCheckListBox *CLBox, bool checked);
	void __fastcall CLBoxSubestacaoInicia(void);
	void __fastcall FormIniciaPosicao(void);
	void __fastcall FormLeOpcoes(void);
	void __fastcall FormSalvaOpcoes(void);
	bool __fastcall ImportaSubestacao(void);
	void __fastcall CLBoxPrimarioInicia(void);
	int  __fastcall LisObjectChecked(TCheckListBox *CLBox, TList *lisOBJ);
	bool __fastcall ReiniciaRede(bool aviso);
	bool __fastcall ValidaOpcoesDeImportacao(void);


public: // User declarations

	__fastcall TFormImportadorUgrid(TComponent* Owner, VTApl *apl_owner, TWinControl *parent, VTImportador *importador);
	__fastcall ~TFormImportadorUgrid(void);
};

// extern PACKAGE TFormImportadorUgrid *FormImportadorUgrid;

#endif

// --------------------------------------------------------------------------------------------- eof
