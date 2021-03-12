//---------------------------------------------------------------------------

#ifndef TFormAnaliseRTH
#define TFormAnaliseRTH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Fontes\Constante\Fases.h>

//---------------------------------------------------------------------------
class VTApl;
class VTAjusta;
class VTAlternativa;
class VTCarga;
class VTMedicao;
class VTMulta;
class VTRede;
class VTRegulador;
class VTSuprimento;
//---------------------------------------------------------------------------
class TFormAnaliseRT : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TChart *chartTensao;
	TToolBar *ToolBar1;
	TToolButton *ToolButtonRelatorio;
	TImageList *ImageList1;
	TActionList *ActionList1;
   TAction *ActionExportaTensao;
	TLineSeries *SeriesV1;
	TLineSeries *SeriesV2;
	TLineSeries *SeriesV3;
	TLineSeries *SeriesVA;
	TLineSeries *SeriesVB;
	TLineSeries *SeriesVC;
	TPageControl *PageControlAlternativas;
	TTabSheet *TabSheet1;
	TAreaSeries *SeriesAd;
	TAreaSeries *SeriesPr_sup;
	TAreaSeries *SeriesPr_inf;
	TAreaSeries *SeriesCr_sup;
	TAreaSeries *SeriesCr_inf;
	TComboBox *cboxData;
	TGroupBox *GroupBox2;
	TChart *ChartIndiceComObra;
	TRadioGroup *rgFases;
	TPieSeries *PieSeriesComObra;
	TListView *LViewDadosCons;
	TToolButton *ToolButtonGeral;
	TAction *ActionAnaliseGeral;
	TTreeView *TViewIndices;
	TSaveDialog *SaveDialogAlternativa;
	TToolButton *ToolButtonCfgGraf;
	TAction *ActionCfgGraf;
	TAction *ActionCfgTensao;
	TAction *ActionGrafRegulador;
	TAction *ActionGrafTensao;
	TAction *ActionGrafCarregamento;
	TToolButton *ToolButtonCfgTensao;
	TToolButton *ToolButton1;
	TPanel *Panel1;
	TGroupBox *GroupBox3;
	TSplitter *Splitter1;
   TPanel *Panel2;
   TChart *chartCorrente;
   TAreaSeries *SerieAdCorr;
   TLineSeries *SeriesIa;
   TLineSeries *SeriesIb;
   TLineSeries *SeriesIc;
   TSplitter *Splitter2;
   TPanel *PanelReg;
   TGroupBox *GBoxReg;
   TAction *ActionChartCorrentes;
   TListView *ListViewReg;
   TToolButton *ToolButton2;
   TAction *ActionExportaCorrente;

	void __fastcall ActionExportaTensaoExecute(TObject *Sender);
	void __fastcall PageControlAlternativasChange(TObject *Sender);
	void __fastcall cboxDataChange(TObject *Sender);
	void __fastcall rgFasesClick(TObject *Sender);
	void __fastcall ActionAnaliseGeralExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ActionCfgGrafExecute(TObject *Sender);
	void __fastcall ActionCfgTensaoExecute(TObject *Sender);
	void __fastcall ActionGrafReguladorExecute(TObject *Sender);
	void __fastcall ActionGrafTensaoExecute(TObject *Sender);
	void __fastcall ActionGrafCarregamentoExecute(TObject *Sender);
   void __fastcall ActionChartCorrentesExecute(TObject *Sender);
   void __fastcall ActionExportaCorrenteExecute(TObject *Sender);

public:		// User declarations
			__fastcall TFormAnaliseRT(TComponent* Owner, VTApl *apl);
			__fastcall ~TFormAnaliseRT(void);

private:	//objetos externos
	VTApl 			*apl;

private:	//dados locais
	#define   	   LIMITE_AMOSTRA 1008
	VTAjusta		   *ajusta;
   VTAlternativa  *ult_alternativa;
	VTMedicao	   *medicao;
	VTMulta		   *multa;
	TList			   *lisEQP;


private:	//métodos
	void 			__fastcall ComboBoxDataInicia(void);
	void 			__fastcall CriaAbasComAlternativas(void);
	VTRegulador*__fastcall DeterminaRegulador(void);
	bool 			__fastcall ExisteAlternativaSalva(AnsiString cod_alt);
	bool 			__fastcall ExportaRelatorio(AnsiString nome_arq);
	void 			__fastcall Inicia(void);
	void 			__fastcall IniciaGraficos(void);
	void 			__fastcall PreencheChartIndices(void);
	void 			__fastcall PreencheChartMedicao(void);
	void 			__fastcall PreencheChartResultados(void);
	void 			__fastcall PreencheChartResultadosCorrReg(VTRegulador *reg);
	void 			__fastcall PreencheGraficos(void);
	void 			__fastcall PreencheLViewDadosCons(void);
	void        __fastcall PreencheLViewReg(void);
	void 			__fastcall PreencheTViewIndices(void);
	void 			__fastcall SalvaAlternativa(AnsiString cod_alt);

};

//---------------------------------------------------------------------------
#endif
//eof

