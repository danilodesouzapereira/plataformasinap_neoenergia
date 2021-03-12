// ---------------------------------------------------------------------------
#ifndef TFormEdtBateriaH
#define TFormEdtBateriaH

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
#include <Vcl.Dialogs.hpp>
#include <System.ImageList.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>

// ---------------------------------------------------------------------------
class VTApl;
class VTBateria;
class VTCarga;
class VTCurva;
class TFormEditaCurvaBateria3F;
class TFormExibeCurvaBateria;

// ---------------------------------------------------------------------------
class TFormEdtBateria : public TForm
{
__published: // IDE-managed Components

	TToolBar *ToolBarLeft;
	TPanel *panelMouse;
	TImage *ImageMouseOff;
	TImage *ImageMouseOn;
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
	TGroupBox *GBoxInfoArmazenamento;
	TPanel *Panel2;
	TLabel *Label1;
	TPanel *Panel1;
	TLabel *Label2;
	TEdit *editCapkWh;
	TComboBox *CBoxMBT;
	TComboBox *CBoxFase;
	TPageControl *PageControlBateria;
	TTabSheet *TabSheetBateria;
	TTabSheet *TabSheetCurvaEst;
	TGroupBox *GBoxPeakShaving;
	TPanel *Panel6;
	TLabel *Label6;
	TComboBox *CBoxPeakShavingCarga;
	TPanel *Panel4;
	TLabel *Label4;
	TPanel *Panel11;
	TLabel *Label11;
	TTabSheet *TabSheetCurvaCargaDescarga;
	TGroupBox *GBoxCurvaCargaDescarga;
	TPanel *pnlCurvaPropria;
	TButton *butImportarCurva;
	TTabSheet *TabSheetModoOper;
	TAction *ActionSelModoOperacao;
	TToolButton *ToolButton2;
	TAction *ActionConfirma;
	TPanel *Panel9;
	TLabel *Label9;
	TEdit *editCodigo;
	TPanel *Panel15;
	TLabel *Label15;
	TComboBox *CBoxModCargaDescarga;
	TPanel *Panel18;
	TLabel *Label18;
	TEdit *editPotNomkW;
	TTabSheet *TabSheetAvancado;
	TGroupBox *GroupBox1;
	TPanel *Panel21;
	TLabel *Label21;
	TEdit *editReserva;
	TPanel *Panel22;
	TLabel *Label22;
	TEdit *editPerdasReativasInatividade;
	TPanel *Panel23;
	TLabel *Label23;
	TEdit *editEficienciaCarga;
	TPanel *Panel24;
	TLabel *Label24;
	TEdit *editEficienciaDescarga;
	TPanel *Panel28;
	TLabel *Label28;
	TEdit *editCapkVA;
	TPanel *Panel29;
	TLabel *Label29;
	TEdit *editEArmazIni;
	TPanel *Panel3;
	TLabel *Label3;
	TEdit *editPerdasAtivasInatividade;
	TTabSheet *TabSheetGraficoCargaDescarga;
	TAction *ActionCancela;
	TAction *ActionImporta;
	TOpenDialog *OpenDialog;
	TRadioGroup *RGroupModoOperacao;
	TComboBox *CBoxPeakShavingHoraIni;
	TComboBox *CBoxPeakShavingDuracao;
	TChart *Chart;
	TFastLineSeries *Series1;
	TFastLineSeries *Series2;
	TFastLineSeries *Series3;
	TFastLineSeries *Series4;
	TFastLineSeries *Series5;
	TFastLineSeries *Series6;
	TRadioGroup *RGroupPeakShaving;
	TAction *ActionSelPeakShaving;

	void __fastcall ActionAlignDownExecute(TObject *Sender);
	void __fastcall ActionAlignLeftExecute(TObject *Sender);
	void __fastcall ActionAlignNoneExecute(TObject *Sender);
	void __fastcall ActionAlignRightExecute(TObject *Sender);
	void __fastcall ActionAlignUpExecute(TObject *Sender);
	void __fastcall ActionEdtExecute(TObject *Sender);
	void __fastcall ActionMouseOffExecute(TObject *Sender);
	void __fastcall ActionMouseOnExecute(TObject *Sender);
	void __fastcall ActionSelModoOperacaoExecute(TObject *Sender);
	void __fastcall PageControlBateriaChange(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall CBoxMBTChange(TObject *Sender);
	void __fastcall CBoxFaseChange(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall PageControlBateriaChanging(TObject *Sender, bool &AllowChange);
	void __fastcall ActionImportaExecute(TObject *Sender);
	void __fastcall editEArmazIniChange(TObject *Sender);
	void __fastcall ActionSelPeakShavingExecute(TObject *Sender);

public: // User declarations
	__fastcall TFormEdtBateria(TComponent* Owner, VTApl *apl, VTBateria *pbateria, bool op_retira);
	__fastcall ~TFormEdtBateria(void);
	virtual void __fastcall Ativo(void);
	virtual void __fastcall Atualiza(void);
	void __fastcall DefParent(TWinControl *parent);

private: // objetos externos

	VTApl *apl;
	VTBateria *pbateria;

	struct
	{
		VTCurva *propria;
	}curva;

protected: // métodos

	void __fastcall AtualizaIndicacaoEditorOff(void);
	void __fastcall AtualizaIndicacaoEditorOn(TToolButton *button);
    void __fastcall CalculaEnergiaInicialPeakShaving(void);
	bool __fastcall CalculaEnergiaPeakShaving(void);
    void __fastcall CalculaPotênciaCarregamentoPeakShaving(void);
	void __fastcall Centraliza(void);
	VTCurva* __fastcall CriaCurva(void);
	void __fastcall DefineModOperOciosa(void);
	void __fastcall DefineTipOperAuto(void);
	void __fastcall DefineTipOperManual(void);
    void __fastcall DefineModoOper(int np, double valorP);
	void __fastcall EscondeAbas(void);
	void __fastcall EscondeSeries(void);
	void __fastcall EstadoBateriaInicia(void);
	void __fastcall ExibeDadosAvancado(void);
	void __fastcall ExibeDadosBateria(void);
	void __fastcall ExibeDadosFases(void);
	void __fastcall ExibeDadosInformacoesGerais(void);
	void __fastcall ExibeDadosModCargaDescarga(void);
	void __fastcall IniciaCBoxFase(void);
	void __fastcall IniciaCBoxPeakShaving(void);
	void __fastcall IniciaCBoxPeakShavingCarga(void);
	void __fastcall IniciaCBoxPeakShavingDuracao(void);
    void __fastcall IniciaCBoxPeakShavingHoraIni(void);
	void __fastcall IniciaDadosBateriaNova(void);
	void __fastcall IniciaMododeOperacao(void);
	void __fastcall IniciaSGrid(void);
	VTCurva* __fastcall InsereValoresCurvaBateria(VTCurva *curva);
	bool __fastcall LeAvancado(void);
	bool __fastcall LeDadosBateria(void);
	bool __fastcall LeDadosCurva(void);
	bool __fastcall LeFases(void);
	bool __fastcall LeInformacaoGerais(void);
    bool __fastcall LePeakShaving(void);
	bool __fastcall LeValorModCargaDescarga(void);
	int  __fastcall MBTfasica(int Fases);
	void __fastcall MontaListaCargas(void);
    void __fastcall MontaListaEqbar(void);
	void __fastcall PeakShavingInicia(void);
	void __fastcall PreencheCurvaEstado(void);
	bool __fastcall PreencheCurvaPeakShaving(void);
	bool __fastcall PreencheSGrid(void);
    bool __fastcall PreencheVetorPeakShaving(void);
	bool __fastcall PreencheVetoresPotencia(void);
    void __fastcall PreencheVetorEnergia(void);
	bool __fastcall RealizaCalculosPeakShaving(void);
	bool __fastcall RealizaPeakShaving(void);
    void __fastcall RedimensionaSerie(TChartSeries* Serie);
	bool __fastcall SelecionaCurva(void);
	bool __fastcall ValidaDadosNumericos(void);
	bool __fastcall ValidaEnergiaPeakShaving(double energia_kWh);
	bool __fastcall ValidaTaxaCarregamentoPeakShaving(void);
	bool __fastcall VerificaTaxaCarregamentoPeakShaving(void);
	void __fastcall VerificaTaxaDescarregamentoPeakShaving(void);
	bool __fastcall VerificaPorcentagem(double valor);


	// dados locais
	TToolButton *button_down;

	TFormEditaCurvaBateria3F *FormEditaCurvaBateria3F;
	TFormExibeCurvaBateria *FormExibeCurvaBateria;

	struct
	{ // coordenadas p/ janela flutuante
		int top;
		int left;
		int width;
		int height;
	}janela;
	struct
	{
		//bool correcaoTotal;
		double taxaCarregamento_kW;
		double *vet_pot_kw;
		double energiaPeakShaving_kWh;
		int inicioDescarga_hr;
		int duracaoDescarga_hr;
		VTCarga *carga;
		TList *listaEqbar;
		TList *listaCargas;
	}peakShaving;
};

// ---------------------------------------------------------------------------
#endif
// eof
