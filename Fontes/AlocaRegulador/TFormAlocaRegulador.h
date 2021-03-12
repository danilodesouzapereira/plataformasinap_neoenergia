//---------------------------------------------------------------------------
#ifndef TFormAlocaReguladorH
#define TFormAlocaReguladorH

//---------------------------------------------------------------------------
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
#include <Grids.hpp>
#include <Buttons.hpp>
#include <VCLTee.Chart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <System.Actions.hpp>
#include <VCLTee.Series.hpp>
#include "TFormEdt.h"
#include <Vcl.Dialogs.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAlocaRT;
class VTEqpto;
class VTBarra;
class VTRede;
class VTRegulador;
class VTLigacao;
class VTObra;

//---------------------------------------------------------------------------
class TFormAlocaRegulador : public TFormEdt
{
__published:   // IDE-managed Components
   TAction *ActionAlocaRT;
   TPageControl *PageControl;
   TTabSheet *TabConfiguracao;
   TTabSheet *TabReguOriginal;
   TStringGrid *gridRT;
   TGroupBox *GBoxConfiguracao;
   TTabSheet *TabLisAlocada;
   TGroupBox *GroupBox3;
   TGroupBox *GroupBox4;
   TListBox *ListBoxReg;
   TToolBar *ToolBar1;
   TToolButton *ToolButton2;
   TToolButton *ToolButton4;
   TAction *ActionAdd;
   TAction *ActionDelete;
   TAction *ActionSelAll;
   TAction *ActionSelNone;
   TToolBar *ToolBar3;
   TBitBtn *butSelAll;
   TBitBtn *butSelNone;
   TChart *chart;
   TFastLineSeries *Series1;
   TAction *ActionRefresh;
   TAction *ActionRemoveRT;
   TAction *ActionConfirmar;
   TAction *ActionCancelar;
   TLabel *Label4;
	TLabel *LabelValvo;
   TToolButton *ToolButton7;
   TAction *ActionEditar;
   TFastLineSeries *Series2;
   TCheckBox *checkTronco;
   TAction *ActionViewTronco;
   TToolButton *ToolButton5;
   TSplitter *Splitter1;
   TGroupBox *GBoxEstoque;
   TListView *ListViewReg;
   TGroupBox *GBoxOpcoes;
   TCheckBox *checkDeleteLisReg;
   TRichEdit *RichEditVim_pu;
   TRichEdit *RichEditValvo_pu;
   TLabel *Label1;
   TRichEdit *RichEditMaxReg;
   TPanel *Panel1;
	TComboBox *CBoxLigacao;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TComboBox *CBoxAjuste;
	TAction *ActionExporta;
	TSaveDialog *SaveDialog;
	TCheckBox *CheckBoxDistancia;
	TRichEdit *RichEditDistancia;
   TToolBar *ToolBarRight;
	TToolButton *ToolButton11;
	TToolButton *ToolButton12;
	TToolButton *ToolButton13;
	TToolButton *ToolButton14;
	TToolButton *ToolButton15;
	TToolButton *ToolButton16;
	TAction *ActionHelp;
   TToolButton *ButMedidor;
   TAction *ActionMedidor;
   TPanel *Panel2;
   TPanel *PanelTapFixo;
   TLabel *Label3;
   TRichEdit *RichEditTapFixo;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionAlocaRTExecute(TObject *Sender);
   void __fastcall ActionSelAllExecute(TObject *Sender);
   void __fastcall ActionSelNoneExecute(TObject *Sender);
   void __fastcall ActionAddExecute(TObject *Sender);
   void __fastcall ActionDeleteExecute(TObject *Sender);
   void __fastcall ActionRemoveRTExecute(TObject *Sender);
   void __fastcall ActionCancelarExecute(TObject *Sender);
   void __fastcall ActionConfirmarExecute(TObject *Sender);
   void __fastcall ListBoxRegClick(TObject *Sender);
   void __fastcall ActionEditarExecute(TObject *Sender);
   void __fastcall ListBoxRegDblClick(TObject *Sender);
   void __fastcall ActionViewTroncoExecute(TObject *Sender);
   void __fastcall ListViewRegClick(TObject *Sender);
   void __fastcall ListViewRegColumnClick(TObject *Sender, TListColumn *Column);
   void __fastcall ListViewRegCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
   void __fastcall ListViewRegDblClick(TObject *Sender);
   void __fastcall ListViewRegSelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall ActionExportaExecute(TObject *Sender);
	void __fastcall CheckBoxDistanciaClick(TObject *Sender);
	void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionMedidorExecute(TObject *Sender);
   void __fastcall CBoxAjusteChange(TObject *Sender);

public:      // User declarations
   __fastcall  TFormAlocaRegulador(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormAlocaRegulador(void);

private:   //métodos
   void       __fastcall AjustaCurvas(TChart *chart);
	//void       __fastcall AlteraRegAlocado(void);
	bool       __fastcall AlteraRegEstoque(void);
   void       __fastcall CancelaAlocacao(void);
   void       __fastcall ConfirmaAlocacao(void);
   void       __fastcall CriaEqptosFicticios(void);
   void       __fastcall CriaModelosReg(void);
   void       __fastcall CriaUmRegulador(double snom = 1., int unid = 1);
   void       __fastcall ConfiguraReguladoresOriginais(void);
   AnsiString __fastcall DeterminaModeloReg(int tipo);
   int        __fastcall DeterminaReguladorSelecionado(void);
   void       __fastcall EliminaEqptosFicticios(void);
   bool       __fastcall Executa(void);
   void       __fastcall ExibeGrafico(VTEqpto *eqpto);
   VTLigacao* __fastcall ExisteLigacaoMovida(int id);
	void       __fastcall ExibeListaAlocada(TList *lisREG);
	void       __fastcall ExportaAlocados(void);
	void       __fastcall ExportaExistentes(void);
	void       __fastcall FormIniciaPosicao(void);
   void       __fastcall HabilitaReguladoresOriginais(void);
   void       __fastcall IniciaGrid(void);
   void       __fastcall IniciaListaReguladoresOriginais(void);
   void       __fastcall IniciaParametros(void);
   void       __fastcall InsereReguladores(TList *lisReg);
   void       __fastcall InsereXY(double X, double Y, TChart *chart, int indexSerie);
   bool       __fastcall LeParametros(void);
   void       __fastcall ListViewRegInicia(void);
   void       __fastcall Moldura(VTEqpto *eqpto=NULL);
   bool       __fastcall RedefineLigacao(TList *lisLigacao, VTBarra *barra1, VTBarra *barra2);
   void       __fastcall RemoveLigacoes(TList *lisLig);
   void       __fastcall RemoveReguladores(TList *lisReg);
   void       __fastcall RemoveReguladoresOriginais(void);
   void       __fastcall SalvaDados(void);
   double     __fastcall SnomToSpassante(double Snom, double dv_perc);
   double     __fastcall SpassanteToSnom(double Spas, double dv_perc);
   double     __fastcall TensaoMinima(VTBarra *barra, int ind_pat);
   //double     __fastcall TensaoRegulada(VTRegulador *reg, double v_pu);
   bool       __fastcall VerificaPlanejamento(void);
   void       __fastcall Zoom(VTEqpto *eqpto);

private:   //objetos externos
   VTApl  *apl;
   VTObra *obra_original;

private:   //dados locais
   VTAlocaRT *alocaRT;
   VTObra    *obra;
   TList     *lisTabReg;
   TList     *lisEQP;
   TList     *lisREG_ALOCADO;
   TList     *lisREG_ORIG_TODOS;
   TList     *lisREG_ORIG_ATIVO;
   //eqptos fictícios necessários p/ configurar os modelos de reguladores
   VTBarra *barra1, *barra2;
   VTRede  *rede;
	struct   {//estrutura p/ controle da ordenação do LView
			TListColumn *Column;
			bool        OrdemCrescente;
			}LViewSort;
};

//---------------------------------------------------------------------------
#endif
//eof

