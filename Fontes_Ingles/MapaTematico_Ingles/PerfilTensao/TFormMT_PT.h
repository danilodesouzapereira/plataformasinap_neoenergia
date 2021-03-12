//---------------------------------------------------------------------------
#ifndef TFormMT_PTH
#define TFormMT_PTH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TFormEdt.h"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTArvore;
class VTCaminho;
class VTEdt;
class VTBarra;
class VTEqpto;
class VTLigacao;
class VTRede;
class VTCfgCor;
class VTTema;
class VTRadial;
class VTTronco;

//---------------------------------------------------------------------------
class TFormMT_PT : public TFormEdt
{
__published:   // IDE-managed Components
   TAction *ActionSelEqpto;
   TListView *LView;
	TGroupBox *GroupBoxRedes;
   TGroupBox *GroupBoxBarras;
   TSplitter *Splitter1;
   TSplitter *Splitter2;
   TTimer *Timer;
   TAction *ActionAnimacao;
   TGroupBox *GroupBox4;
   TGroupBox *GroupBox5;
   TCheckBox *CheckBoxDistancia;
   TToolBar *ToolBarRight;
   TToolButton *ButShowRedes;
   TToolButton *ButShowBarras;
   TAction *ActionShowRedes;
   TAction *ActionShowBarras;
   TGroupBox *GroupBox2;
   TComboBox *CBoxPatamar;
   TComboBox *CBoxAno;
   TGroupBox *GroupBox1;
   TBitBtn *ButTimer;
   TCheckBox *CheckBoxAno;
   TCheckBox *CheckBoxPatamar;
   TPanel *Panel2;
   TGroupBox *GroupBox3;
   TAction *ActionCalculo;
   TToolButton *ButMapaTematico;
   TAction *ActionMapaTematico;
	TTreeView *TView;
   TChart *Chart;
   TFastLineSeries *SeriesVan;
   TFastLineSeries *SeriesVbn;
   TFastLineSeries *SeriesVcn;
   TAction *ActionSelTronco;
   TToolButton *ToolButton1;
   TAction *ActionExporta;
   TSaveDialog *SaveDialog;
   TToolButton *ToolButton2;
   TAction *ActionHelp;
   TToolButton *ToolButton4;
   TToolButton *ToolButton5;



   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall ActionSelEqptoExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall LViewDblClick(TObject *Sender);
   void __fastcall LViewColumnClick(TObject *Sender, TListColumn *Column);
   void __fastcall LViewCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall ActionAnimacaoExecute(TObject *Sender);
   void __fastcall CBoxPatamarChange(TObject *Sender);
   void __fastcall CheckBoxDistanciaClick(TObject *Sender);
   void __fastcall ActionShowBarrasExecute(TObject *Sender);
   void __fastcall ActionShowRedesExecute(TObject *Sender);
   void __fastcall CBoxAnoChange(TObject *Sender);
   void __fastcall ActionMapaTematicoExecute(TObject *Sender);
   void __fastcall LViewSelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
   void __fastcall ActionSelTroncoExecute(TObject *Sender);
   void __fastcall ActionExportaExecute(TObject *Sender);
   void __fastcall CheckBoxAnoClick(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);

public:      // User declarations
           __fastcall  TFormMT_PT(TComponent *owner, VTApl *apl_owner, TWinControl *parent);
           __fastcall ~TFormMT_PT(void);
   void    __fastcall Atualiza(void);

private:  //método da classe usada na ordenação da lista de Ptocarga
   //static int __fastcall ComparaTensaoBarra(void *Item1, void *Item2);

private:   //métodos
   void    __fastcall AtualizaTela(void);
   double  __fastcall CalculaDistancia_m(VTLigacao *ligacao);
   void    __fastcall CBoxAnoInicia(void);
   void    __fastcall CBoxPatamarInicia(void);
   void    __fastcall ChartInicia(void);
   void    __fastcall ChartInsereBarra(VTBarra *barra, int index, int ind_pat);
   void    __fastcall DefineBarraFinal(VTBarra *barra);
   void    __fastcall DefineBarraInicial(VTBarra *barra, VTRede *rede);
   void    __fastcall DestacaEqpto(TList *lisEQP);
   void    __fastcall FormIniciaPosicao(void);
   void    __fastcall HabilitaMapaTematico(bool enabled);
   int     __fastcall IndicePatamarSelecionado(void);
   TList*  __fastcall LisBarra(TList *lisLIG);
   void    __fastcall LViewBarraInicia(TList *lisLIG);
   void    __fastcall LViewBarraInsereBarra(VTBarra *barra, double dist_m, int ind_pat);
   void    __fastcall Moldura(TList *lisEQP);
   bool    __fastcall RecalculaFluxo(bool obrigatorio=true);
   void    __fastcall RecalculaMapaTematico(void);
   //void    __fastcall SelecionaModoGrafico(int modo);
   VTRede* __fastcall RedeDaBarra(VTBarra *barra);
   VTRede* __fastcall RedeDaBarra(TList *lisLIG, VTBarra *barra);
   void    __fastcall Zoom(VTEqpto *eqpto);
	void 	  __fastcall TViewInicia(void);
   VTRede* __fastcall TViewRedeSelecionada(void);
   VTRede* __fastcall TViewVerificaNodeRedes(AnsiString cod_rede, TList *lisREDE); //fkm-2015.09.10

private:   //objetos externos

private:   //dados locais
   int       ano_mercado;
   VTApl     *apl;
	VTArvore  *arvore;
	VTCaminho *caminho;
   VTRadial  *radial;
   VTTronco  *tronco;
   VTEdt     *edt_busca;
   VTCfgCor  *cfg_cor;
   VTTema    *tema;
   TList     *lisEQP;
   TList     *lisLIG;
 
	struct   {//estrutura c/ Eqptos inicial e final do caminho elétrico
            VTRede  *rede;
            VTBarra *bar1, *bar2;
            VTBarra *barra_ini, *barra_fim;
            } dist;
   struct   {//estrutura p/ controle da ordenação do LView
            TListColumn *Column;
            bool        OrdemCrescente;
            }LViewSort;
};

//---------------------------------------------------------------------------
#endif
//eof
