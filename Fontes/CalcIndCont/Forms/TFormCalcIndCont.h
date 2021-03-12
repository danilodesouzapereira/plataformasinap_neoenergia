//---------------------------------------------------------------------------
#ifndef TFormCalcIndContH
#define TFormCalcIndContH
//---------------------------------------------------------------------------
#include <stdio.h>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TFormEdt.h"
#include <System.Actions.hpp>
#include <System.Classes.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.ImageList.hpp>
#include <PlataformaSinap\Fontes\Forms\TFormEdt.h>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <Grids.hpp>
#include <Classes.hpp>
#include <Dialogs.hpp>
#include <CheckLst.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCalcIndCont;
class VTBloco;
class VTEdt;
class VTBuscaProt;
class VTDefeito;
class VTEqpto;
class VTChave;
class VTResCont;
class VTRede;
class TVisRede;
class VTBlocoRad;
class VTEstratSoc;

class VTBDado_Externo;

//---------------------------------------------------------------------------
struct ORDENACAO
		{
		int  ColumnIndex;
		bool OrdemCrescente;
		};

//---------------------------------------------------------------------------
//enum para auxiliar   o preenchiiento do lvResult //FKM 03/08/2017
//---------------------------------------------------------------------------
enum col_CaldIndContLVResult
{
	lvcolCIC_Rede = 0, lvcolCIC_Bloco, lvcolCIC_CLIh, lvcolCIC_CLI, lvcolCIC_Consu, lvcolCIC_DEC,
	lvcolCIC_FEC, lvcolCIC_END, lvcolCIC_DECBT, lvcolCIC_FECBT, lvcolCIC_ENDBT, lvcolCIC_FECPISCA,
	lvcolCIC_ConsuVIP, lvcolCIC_FECPISCAVIP, lvcolCIC_Count
};

//---------------------------------------------------------------------------
//struct strParamRede : TObject
//					 {
//					 VTRede *rede;         //rede
//					 double  lambda, trep; //taxa de falha e tempo de reparo
//					 };
//---------------------------------------------------------------------------
class TFormCalcIndCont : public TFormEdt
  {
	__published:	// IDE-managed Components

	TAction *ActionSelEqpto;
	TAction *ActionAvaliaVis;
	TAction *ActionDefListas;
	TToolButton *ToolButton2;
	TAction *ActionFormParam;
	TAction *ActionConfirmaParamGerais;
	TAction *ActionSelChvAuto;
	TAction *ActionRemChvAuto;
	TAction *ActionMostraLbChaves;
	TPageControl *PageControl2;
	TTabSheet *TabSheet7;
	TPageControl *PageControl3;
	TTabSheet *TabSheet8;
	TTabSheet *TabSheet10;
	TPageControl *PageControl4;
	TTabSheet *TabSheet12;
	TTabSheet *TabSheet14;
	TGroupBox *GroupBox3;
	TListBox *lbChaves;
	TSplitter *Splitter3;
	TGroupBox *GroupBox4;
	TButton *Button1;
	TButton *Button2;
	TGroupBox *GroupBox5;
	TListBox *lbChavesAuto;
	TTabSheet *TabSheet15;
	TGroupBox *GroupBox7;
	TGroupBox *GroupBox8;
	TTreeView *tvChavesProt;
	TSplitter *Splitter6;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TListView *lvResultRede;
	TListView *lvResult;
	TTabSheet *TabSheet2;
	TListView *lvResultDefeito;
	TAction *ActionMostraResBloco;
	TToolButton *ToolButton1;
	TAction *ActionReiniciar;
	TToolBar *ToolBar3;
	TToolButton *ToolButton4;
	TAction *ActionExportaRelatorio;
	TAction *ActionExportaDetalhaDefeito;
	TAction *ActionMostraDetalheDefeito;
	TToolBar *ToolBar5;
	TToolButton *ToolButton7;
	TAction *ActionExportaResultadosGeraisResRede;
	TAction *ActionExportaResultadosGeraisRes;
	TToolBar *ToolBar6;
	TToolButton *ToolButton8;
	TOpenDialog *OpenDialog;
	TAction *ActionArqParam;
	TTabSheet *TabSheet4;
	TGroupBox *GroupBox10;
	TAction *ActionAjusteParam;
	TAction *ActionCalcIndDeriv;
	TTabSheet *TabSheet5;
	TListView *lvComp;
	TAction *ActionClusterChavesNA;
	TPageControl *PageControl5;
	TTabSheet *TabSheet6;
	TTreeView *tvChavesNA;
	TAction *ActionDefLisBLocosDiv;
	TAction *ActionLisBlocosCand;
	TTabSheet *TabSheet19;
	TToolBar *ToolBar2;
	TToolButton *ToolButton5;
	TToolButton *ToolButton9;
	TToolButton *ToolButton17;
	TAction *ActionRejeitaParamGerais;
	TToolButton *ToolButton18;
	TAction *ActionArqCircuitoVerificado;
	TAction *ActionArqOcorrVerificado;
	TAction *ActionMostraBlocoParam;
	TMainMenu *MainMenu1;
	TAction *ActionAtualizaParamMedios;
	TAction *ActionExportaRelatVerifAlimBloco;
	TAction *ActionExportaRelatOcorrBloco;
	TAction *ActionExportaRelatOcorrAlim;
	TAction *ActionImprime;
	TSplitter *Splitter13;
	TSaveDialog *SaveDialog;
	TAction *ActionEditaParamJusProt;
	TToolButton *ToolButton27;
	TPageControl *PageControl7;
	TTabSheet *TabSheet23;
	TTabSheet *TabSheet24;
	TTabSheet *TabSheet25;
	TSplitter *Splitter7;
	TGroupBox *GroupBox6;
	TStringGrid *sgridFalhasPat;
	TGroupBox *GroupBox9;
	TCheckListBox *chkLstTipoProt;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TCheckBox *cbRecurso;
	TCheckBox *cbRetorno;
	TRichEdit *edtLambda;
	TRichEdit *edtTLoc;
	TRichEdit *edtTMan;
	TRichEdit *edtPercDefTrans;
	TRichEdit *edtTRep;
	TRichEdit *edtTManAuto;
	TRichEdit *edtPuLim;
	TGroupBox *GroupBox2;
	TStringGrid *sgridParamRede;
	TToolBar *ToolBar12;
	TToolButton *ToolButton10;
	TAction *ActionImprimeRelNA;
	TStringGrid *sgridAjuste;
	TToolBar *ToolBar13;
	TToolButton *ToolButton33;
	TAction *ActionViewDEC;
	TToolButton *ToolButton6;
	TAction *ActionMostraVIP;
	TToolButton *ToolButton11;
	TAction *ActionViewCLI;
	TToolButton *ToolButton13;
	TToolButton *ToolButton14;
	TAction *ActionMostraParamAux;
	TToolButton *ToolButton15;
	TAction *ActionSalvar;
	TTabSheet *TabSheet3;
	TButton *Button3;
	TDateTimePicker *DateTimePickerInicio;
	TDateTimePicker *DateTimePickerTermino;
	TLabel *Label8;
	TLabel *Label9;
	TTabSheet *TabSheet9;
	TListView *lvIndicIndiv;
	TToolBar *ToolBar1;
	TToolButton *ToolButton16;
	TAction *ActionExportaResIndicIndividuais;
	TRadioGroup *rgZonaReliga;
	TAction *ActionVoltaParamOriginais;
	TToolBar *ToolBar4;
	TToolButton *ToolButton22;
	TAction *ActionVoltaParamMdb;
	TToolButton *ToolButton21;
	TCheckBox *chkReiniciaParam;
	TToolButton *ToolButton19;
	TAction *ActionGravaParamMdb;
	TToolButton *ToolButton24;
	TAction *ActionLeArqMdb;
	TToolButton *ToolButton25;
	TToolButton *ToolButton26;
	TEdit *edtMes;
	TToolButton *ToolButton28;
	TAction *ActionExecutaMes;
	TToolBar *ToolBar7;
	TToolButton *ToolButton29;
	TAction *ActionExportaRelatorioCompensacoes;
	TToolBar *ToolBar10;
	TToolButton *ToolButton20;
	TToolButton *butReiniciaParHist;
	TGroupBox *GroupBox16;
	TListView *lvParamRede;
	TSplitter *Splitter1;
	TGroupBox *GroupBox17;
	TListView *lvParamBloco;
	TTabSheet *TabSheet11;
	TToolButton *ToolButton23;
	TGroupBox *GroupBox11;
	TCheckListBox *chkBoxReiniciaParam;
	TToolBar *ToolBar8;
	TToolButton *ToolButton30;
	TToolButton *ToolButton3;
	TAction *ActionGrafGanhosNA;
	TAction *ActionLeTxtIndObserv;
	TTabSheet *TabSheet13;
	TLabel *lblPathParam;
	TToolButton *ToolButton12;
	TAction *ActionImprimeParamSimula;
	TAction *ActionTeste;
	TStatusBar *StatusBarCount;
	TTabSheet *TabSheet16;
	TStringGrid *sgridParamBloco;
	TTabSheet *TabSheet18;
	TGroupBox *GroupBox15;
	TGroupBox *GroupBox12;
	TListBox *lbChavesSemIF;
	TSplitter *Splitter2;
	TGroupBox *GroupBox13;
	TButton *Button4;
	TButton *Button5;
	TGroupBox *GroupBox14;
	TListBox *lbChavesComIF;
TToolButton *btnAdicionaConjunto;
	TAction *ActionInsereConjunto;
	TToolButton *btnDeletaConjunto;
	TToolButton *ToolButton34;
	TAction *ActionDeletaConjunto;	TGroupBox *GroupBox18;
	TSplitter *Splitter4;
	TAction *ActionSelChaveIF;
	TAction *ActionRemChaveIF;
	TAction *ActionPreparaIndicacaoFalta;
	TGroupBox *GroupBox19;
	TTreeView *tvIF;
	TLabel *Label10;
	TRichEdit *edtRedTprepIF;
	TToolBar *ToolBar9;
	TToolButton *btnLeIF;
	TAction *ActionLeIF;
	TRichEdit *edtIndispPadrao;
	TLabel *Label11;
	TTabSheet *TabSheet17;
	TToolBar *ToolBar11;
	TToolButton *ToolButton31;
	TTreeView *tvCircuitosSocorro;
	TAction *ActionImprimeCircuitosSocorro;
	TAction *ActionNAParalelas;
	void __fastcall ActionEdtExecute(TObject *Sender);
	void __fastcall ActionSelEqptoExecute(TObject *Sender);
	void __fastcall tvChavesNAClick(TObject *Sender);
	void __fastcall ActionDefListasExecute(TObject *Sender);
	void __fastcall ActionConfirmaParamGeraisExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ActionSelChvAutoExecute(TObject *Sender);
	void __fastcall ActionRemChvAutoExecute(TObject *Sender);
	void __fastcall lvResultClick(TObject *Sender);
	void __fastcall tvChavesProtMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		int X, int Y);
	void __fastcall ActionMostraResBlocoExecute(TObject *Sender);
	void __fastcall ActionReiniciarExecute(TObject *Sender);
	void __fastcall ActionExportaRelatorioExecute(TObject *Sender);
	void __fastcall lvResultDefeitoDblClick(TObject *Sender);
	void __fastcall ActionExportaResultadosGeraisResRedeExecute(TObject *Sender);
	void __fastcall ActionExportaResultadosGeraisResExecute(TObject *Sender);
	void __fastcall lbChavesAutoClick(TObject *Sender);
	void __fastcall tvChavesProtClick(TObject *Sender);
	void __fastcall ActionArqParamExecute(TObject *Sender);
	void __fastcall ActionAjusteParamExecute(TObject *Sender);
	void __fastcall ActionCalcIndDerivExecute(TObject *Sender);
	void __fastcall ActionRejeitaParamGeraisExecute(TObject *Sender);
	void __fastcall ActionMostraBlocoParamExecute(TObject *Sender);
	void __fastcall ActionExportaRelatVerifAlimBlocoExecute(TObject *Sender);
	void __fastcall ActionEditaParamJusProtExecute(TObject *Sender);
	void __fastcall ActionImprimeRelNAExecute(TObject *Sender);
	void __fastcall ActionViewDECExecute(TObject *Sender);
	void __fastcall ActionMostraLbChavesExecute(TObject *Sender);
	void __fastcall ActionMostraVIPExecute(TObject *Sender);
	void __fastcall ActionViewCLIExecute(TObject *Sender);
	void __fastcall ActionMostraParamAuxExecute(TObject *Sender);
	void __fastcall ActionSalvarExecute(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall ActionExportaResIndicIndividuaisExecute(TObject *Sender);
	void __fastcall lvParamRedeColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall lvParamRedeCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);
	void __fastcall lvParamBlocoColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall lvParamBlocoCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);
	void __fastcall lvResultRedeColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall lvResultRedeCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);
	void __fastcall lvResultColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall lvResultCompare(TObject *Sender, TListItem *Item1, TListItem *Item2, int Data,
		int &Compare);
	void __fastcall lvResultDefeitoColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall lvResultDefeitoCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);
	void __fastcall lvCompColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall lvCompCompare(TObject *Sender, TListItem *Item1, TListItem *Item2, int Data,
		int &Compare);
	void __fastcall lvIndicIndivColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall lvIndicIndivCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
			 int Data, int &Compare);
	void __fastcall lbChavesClick(TObject *Sender);
	void __fastcall ActionVoltaParamOriginaisExecute(TObject *Sender);
	void __fastcall ActionVoltaParamMdbExecute(TObject *Sender);
	void __fastcall ActionGravaParamMdbExecute(TObject *Sender);
	void __fastcall ActionLeArqMdbExecute(TObject *Sender);
	void __fastcall ActionExecutaMesExecute(TObject *Sender);
	void __fastcall ActionExportaRelatorioCompensacoesExecute(TObject *Sender);
	void __fastcall ActionGrafGanhosNAExecute(TObject *Sender);
	void __fastcall ActionLeTxtIndObservExecute(TObject *Sender);
	void __fastcall ActionImprimeParamSimulaExecute(TObject *Sender);
	void __fastcall sgridParamBlocoClick(TObject *Sender);
	void __fastcall ActionInsereConjuntoExecute(TObject *Sender);
	void __fastcall ActionDeletaConjuntoExecute(TObject *Sender);	void __fastcall ActionSelChaveIFExecute(TObject *Sender);
	void __fastcall ActionRemChaveIFExecute(TObject *Sender);
	void __fastcall tvIFClick(TObject *Sender);
	void __fastcall ActionPreparaIndicacaoFaltaExecute(TObject *Sender);
	void __fastcall sgridAjusteKeyPress(TObject *Sender, System::WideChar &Key);
	//gh201806: ação para seleção de arquivo txt com dados de IF
	void __fastcall ActionLeIFExecute(TObject *Sender);
	void __fastcall ActionImprimeCircuitosSocorroExecute(TObject *Sender);
	void __fastcall ActionNAParalelasExecute(TObject *Sender);

	private:	// User declarations
        void __fastcall ImprimeResCalcIndCont();
	public:		// User declarations
		  __fastcall TFormCalcIndCont(TComponent* Owner, VTApl* apl_owner, TWinControl *parent);
		  __fastcall ~TFormCalcIndCont(void);
		  void   __fastcall Inicia(void);

		  void   __fastcall AtualizaInterfaceDadosGerais(void);
		  void   __fastcall ChkLstTipoProtInicia(void);
		  void   __fastcall ConfirmaParametros(void);
		  void   __fastcall ExecutaAjusteCjto(void);
		  //void   __fastcall ExecutaAjusteSE(void);
		  bool   __fastcall ExisteRedeEqvBT(void);  //hk201612: acréscimo de função JCG
		  bool   __fastcall GravaArqParam(void);
		  void   __fastcall LbChavesInicia(void);
		  void   __fastcall LbChavesIFInicia(void); //hk201710IF
		  void   __fastcall LVResultInicia(void);
		  void   __fastcall LvCompInicia(void);
		  void   __fastcall LvParamBlocoInicia(void);
		  void   __fastcall LvParamRedeInicia(void);
		  void __fastcall PreencheListaParametroSe(TList* lisSeParametro);
		  bool   __fastcall SelecionaArquivo(void);
		  void   __fastcall SgridFalhasPatInicia(void);
		  void   __fastcall SgridParamRedeBlocoInicia(void);
		  void   __fastcall SgridAjusteInicia(void);
		  void   __fastcall TVChavesInicia(void);
		  void   __fastcall IniciaLisRedOrdenada(void);
        void   __fastcall OrdenaLisChaves(TList *lisChaves);
		  void   __fastcall IniciaLisChavesFunde(TList *lisChavesFunde);
		  bool  __fastcall TrocaRedeEqvBTporCargaEqv(VTApl* apl); //hk201612: acréscimo de função JCG
		void   __fastcall LeArquivoParametros(AnsiString arq);//hk201702: alterações gerais
        void   __fastcall IdentificaChavesNASerieParalela(void);

	// dados externos
		VTApl 		*apl;
		VTEdt 		*edt_busca;

	// dados locais
		VTCalcIndCont *calcIndCont;

		/*VTBuscaProt  *buscaProt;
		//Variavel Intermediária
		TList       *lisCargaBlocos;
		//Resultados
		TList       *lisDefeitos;
		VTResCont   *resCont;
		//Parametros
		double      pu_lim; //limite de transferencia
		bool        flagRecurso; //sim se utiliza recursos de socorro a jusante
		double *distFalhasPat; //distribuição de falhas por patamar
		TList  *lisParamRede;  //lista de parâmetros por rede
		double tman_manual; //duração de manobras manuais
		double tman_auto;   //duração de manobra com telecomando
		double tlocaliza;   //duração de preparação e localizacao de defeito
		double trep;        //duração do reparo
		double lambda;      //taxa de falha (falhas/km/ano)
		double percDefTrans;//percentual de defeitos transitórios
		bool   flagAutoSoc; //true: aceita socorros com operações telecom (isolamto e socorr); false: caso contrário
		double tlimIC;     //duracao máxima para contar nos indicadores de continuidade
		*/

		void 		  __fastcall IniciaParamOcorrencias(void);
		void 		  __fastcall LeArqVerifCjto(AnsiString codPath);
		void 		  __fastcall ExecutaAjusteSE2(void);
		void 		  __fastcall ExecutaCompensacoes(void);
		//void 		  __fastcall GeraLisParamRede(void);
		AnsiString __fastcall LeDado(FILE* fp, bool &flagEOF);

		VTBDado_Externo *bdado;
		TList *lisBarraOc;
		TList *lisParamEqpto;
		TList *lisParamRede;
		//TList *lisVerifCjto;
		TList *lisRedeOrdenada; //lista auxiliar com redes em ordem alfabética

      TList *lisChavesFunde; //Lista de cheves que permanecem na rede e não são fundidas
	};

//struct strVerifCjto : TObject
//					 {
//					 AnsiString nomeSe, codSe;
//					 double  decVerif, fecVerif;
//					 };

//---------------------------------------------------------------------------
extern PACKAGE TFormCalcIndCont *FormCalcIndCont;
//---------------------------------------------------------------------------
#endif
