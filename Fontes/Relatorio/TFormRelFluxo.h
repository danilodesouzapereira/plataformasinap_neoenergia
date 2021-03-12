//---------------------------------------------------------------------------
#ifndef TFormRelFluxoH
#define TFormRelFluxoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <System.Actions.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Buttons.hpp>
#include <System.ImageList.hpp>
#include <complex>
//---------------------------------------------------------------------------
enum modoFORM {
               modoBARRAS=0,
               modoCARGAS,
               modoDEMANDA,
               modoTRAFOS,
               modoTRECHOS,
               modoERRO,
               modoCOUNT
               };
#define strSEM_FLUXO "Sem Fluxo"
//---------------------------------------------------------------------------
class VTApl;
class VTPatamar;
class TFormSelArranjo;
class TFormSelClasse;
class TFormSelRedePorHierarquia;
class TFormSelRedePorTipo;

//---------------------------------------------------------------------------
class TFormRelFluxo : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList;
   TToolBar *ToolBar;
   TImageList *ImageList;
   TToolButton *ToolButton1;
   TAction *ActionExporta;
   TSaveDialog *SaveDialog;
   TGroupBox *GBoxPlanilha;
   TGroupBox *GBoxGrafico24pat;
   TToolButton *ToolButton2;
   TListView *LView;
   TChart *Chart24pat;
   TFastLineSeries *SeriesAdequada;
   TFastLineSeries *SeriesPrecaria;
   TFastLineSeries *SeriesCritica;
   TToolButton *ButRelBarra;
   TToolButton *ButRelCarga;
   TAction *ActionRelBarra;
   TAction *ActionRelCarga;
   TToolButton *ButRelDemanda;
   TAction *ActionRelDemanda;
   TAction *ActionSelLisRede;
   TGroupBox *GBoxFiltroRede;
   TSplitter *SplitterFiltro;
   TSplitter *SplitterPlanilha;
   TPanel *PanelFiltro;
   TGroupBox *GBoxFiltroClasse;
   TAction *ActionSelLisClasse;
   TSplitter *Splitter1;
   TPanel *Panel1;
   TCheckBox *CheckBoxPercentual;
   TToolButton *ButRelTrecho;
   TAction *ActionRelTrecho;
   TAction *ActionSelLisArranjo;
   TGroupBox *GBoxFiltroArranjo;
   TToolButton *ButRelTrafo;
   TAction *ActionRelTrafo;
	TToolButton *ButDestacaEqpto;
	TAction *ActionDestacaEqptoCritico;
	TPopupMenu *PopupMenuDestaca;
	TMenuItem *MenuItemFaixaCritica;
	TAction *ActionDestacaEqptoAdequado;
	TAction *ActionDestacaEqptoPrecario;
	TMenuItem *MenuItemFaixaPrecaria;
	TMenuItem *MenuItemFaixaAdequada;
	TMenuItem *N1;
	TMenuItem *DefinirCor1;
	TColorDialog *ColorDialog;
	TAction *ActionDefineCor;
   TToolButton *ToolButton3;
   TToolButton *ToolButton5;
   TAction *ActionHelp;
   TRadioGroup *RadioGroupModo;
   TPanel *PanelGrafico;
   TGroupBox *GBoxGrafico01pat;
   TGroupBox *GBoxAnimacao;
   TBitBtn *ButTimer;
   TTimer *Timer;
   TAction *ActionAnimacao;
   TComboBox *CBoxPatamar;
   TChart *Chart01pat;
   TPieSeries *PieSeriesComObra;
   TSplitter *SplitterCharts;
   TCheckBox *CheckBoxSemFluxo;
   TFastLineSeries *SeriesSemFluxo;
   TFastLineSeries *SeriesTotal;

   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall LViewColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall LViewCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
   void __fastcall ActionExportaExecute(TObject *Sender);
   void __fastcall ActionRelBarraExecute(TObject *Sender);
   void __fastcall ActionRelCargaExecute(TObject *Sender);
   void __fastcall ActionRelDemandaExecute(TObject *Sender);
   void __fastcall ActionRelErroExecute(TObject *Sender);
   void __fastcall ActionSelLisRedeExecute(TObject *Sender);
   void __fastcall ActionSelLisClasseExecute(TObject *Sender);
   void __fastcall CheckBoxPercentualClick(TObject *Sender);
   void __fastcall ActionRelTrechoExecute(TObject *Sender);
   void __fastcall ActionSelLisArranjoExecute(TObject *Sender);
   void __fastcall ActionRelTrafoExecute(TObject *Sender);
	void __fastcall ActionDestacaEqptoCriticoExecute(TObject *Sender);
	void __fastcall ActionDestacaEqptoAdequadoExecute(TObject *Sender);
	void __fastcall ActionDestacaEqptoPrecarioExecute(TObject *Sender);
	void __fastcall ActionDefineCorExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall RadioGroupModoClick(TObject *Sender);
   void __fastcall ActionAnimacaoExecute(TObject *Sender);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall CBoxPatamarChange(TObject *Sender);
   void __fastcall CheckBoxSemFluxoClick(TObject *Sender);


public:		// User declarations
		  __fastcall  TFormRelFluxo(TForm *Owner, VTApl *apl_owner, TWinControl *parent);
		  __fastcall ~TFormRelFluxo(void);
	void __fastcall  Atualiza(void);

private:	//objetos externos
   VTApl     *apl;

private:	//dados locais
   int             modoSelecionado;
   bool            check_semFluxo;
	TColor			           cor_eqpto;
   TFormSelArranjo           *FormSelArranjo;
   TFormSelClasse            *FormSelClasse;
   TFormSelRedePorHierarquia *FormSelRedePorHierarquia;
	TFormSelRedePorTipo       *FormSelRedePorTipo;
   TList           *lisARRANJO;
   TList           *lisCLASSE;
   TList           *lisREDE;
	TList           *lisEQP;
   TList           *lisAUX; //FKM
	TList           *lisADEQUADA;
	TList           *lisPRECARIA;
	TList       	 *lisCRITICA;
   TList       	 *lisISOLADA; //FKM
   struct{
         int    ColumnIndex;
         bool   OrdemCrescente;
         }ListViewSort;
   struct StructDouble  {
								VTPatamar *patamar;
								double     adequada, precaria, critica, semfluxo, total;
								};
   struct StructRel  {
                     AnsiString                 caption;
                     bool                       val_inteiro;
                     DynamicArray<StructDouble> array;
                     }rel_qtde, rel_perc;

private:	//métodos
   void       __fastcall CBoxPatamarInicia(void);
   void       __fastcall Chart01patInicia(void);
   void       __fastcall Chart24patInicia(void);
   void       __fastcall ClassificaBarra(TList *lisBARRA, int ind_pat);
   void       __fastcall ClassificaCarga(TList *lisCARGA, int ind_pat);
   void       __fastcall ClassificaLigacao(TList *lisLIGACAO, int ind_pat);
   double     __fastcall ComprimentoTrecho_km(TList *lisTRECHO);
   int     	   __fastcall ConverteIndexPatHabilitado(VTPatamar *patamar);
   double     __fastcall Demanda_kw(TList *lisCARGA, int ind_pat);
	void       __fastcall ExecutaFluxoPotencia(void);
   void       __fastcall ExibeInfoEqptoSemFluxo(bool exibe);
	int        __fastcall FiltraLisCarga(TList *lisCARGA);
	int        __fastcall FiltraLisTrecho(TList *lisTRECHO);
	void       __fastcall FormIniciaPosicao(void);
	void       __fastcall GraficoDestacaEqpto(TList *lisEQP=NULL);
	void       __fastcall IniciaArrayValorPercentual(void);
   void       __fastcall IniciaArrayValorTotal(void);
	int        __fastcall IniciaLisEqpto(TList *lisEQP, int eqpto_tipo);
   void       __fastcall ListaEqptoDestaque(TList *lisEXT); //FKM
   void       __fastcall LViewInicia(void);
   bool       __fastcall SelecionaArquivo(void);
};

//---------------------------------------------------------------------------
#endif
//eof
