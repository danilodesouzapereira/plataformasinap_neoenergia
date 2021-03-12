//---------------------------------------------------------------------------
#ifndef TFormAlocaCapacitorH
#define TFormAlocaCapacitorH

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

//---------------------------------------------------------------------------
class VTApl;
class VTAlocaBC;
class VTVetval;
class VTEqpto;
class VTObra;
class VTRede;           //DVK 2013.07.24

//---------------------------------------------------------------------------
class TFormAlocaCapacitor : public TFormEdt
{
__published:   // IDE-managed Components
   TAction *ActionAdd;
   TAction *ActionAlocaBC;
   TAction *ActionCancelar;
   TAction *ActionConfirmar;
   TAction *ActionDelete;
   TAction *ActionExporta;
   TAction *ActionHelp;
   TAction *ActionRefresh;
   TAction *ActionRemoveCap;
   TAction *ActionSelAll;
   TAction *ActionSelNone;
   TBitBtn *butSelAll;
   TBitBtn *butSelNone;
   TChart *chart;
   TCheckBox *CheckBoxDistancia;
   TCheckBox *CheckBoxFatpot;
   TCheckBox *checkDeleteLisCap;
   TCheckBox *checkLigado;
   TCheckBox *checkQLimite;
   TCheckBox *checkTronco;
   TComboBox *CBoxPatamar;
   TEdit *editCusto;
   TFastLineSeries *Series1;
   TGroupBox *GBoxCustoEnergia;
   TGroupBox *GBoxEstoque;
   TGroupBox *GBoxPatamar;
   TGroupBox *GBoxRestricao;
   TGroupBox *GroupBox1;
   TGroupBox *GroupBox3;
   TGroupBox *GroupBox4;
   TLabel *Label3;
   TListBox *ListBoxCap;
   TListView *ListViewCap;
   TPageControl *PageControl;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TPanel *Panel5;
   TRichEdit *RichEditDistancia;
   TRichEdit *RichEditFatpot;
   TRichEdit *RichEditMaxMvar;
   TSaveDialog *SaveDialog;
   TSplitter *Splitter1;
   TStringGrid *gridLisCap;
   TTabSheet *TabCapAlocado;
   TTabSheet *TabCapOriginal;
   TTabSheet *TabConfiguracao;
   TToolBar *ToolBar1;
   TToolBar *ToolBarRight;
   TToolBar *ToolBar3;
   TToolButton *ToolButton12;
   TToolButton *ToolButton13;
   TToolButton *ToolButton14;
   TToolButton *ToolButton15;
   TToolButton *ToolButton16;
   TToolButton *ToolButton17;
   TToolButton *ToolButton2;
   TToolButton *ToolButton4;
   TToolButton *ToolButton5;
   TToolButton *ToolButton7;
   TToolButton *ToolButton1;

   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
	void __fastcall ActionAlocaBCExecute(TObject *Sender);
	void __fastcall ActionSelAllExecute(TObject *Sender);
   void __fastcall ActionSelNoneExecute(TObject *Sender);
   void __fastcall ActionAddExecute(TObject *Sender);
   void __fastcall ActionDeleteExecute(TObject *Sender);
   void __fastcall ActionRefreshExecute(TObject *Sender);
   void __fastcall ActionRemoveCapExecute(TObject *Sender);
   void __fastcall ActionCancelarExecute(TObject *Sender);
	void __fastcall ActionConfirmarExecute(TObject *Sender);
	void __fastcall ListBoxCapClick(TObject *Sender);
   void __fastcall ListViewCapClick(TObject *Sender);
   void __fastcall ListViewCapDblClick(TObject *Sender);
   void __fastcall ListViewCapSelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
   void __fastcall ListBoxCapDblClick(TObject *Sender);
   void __fastcall CheckBoxOpcaoClick(TObject *Sender);
   void __fastcall ListViewCapColumnClick(TObject *Sender, TListColumn *Column);
   void __fastcall ListViewCapCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall checkLigadoClick(TObject *Sender);
	void __fastcall ActionExportaExecute(TObject *Sender);
	void __fastcall ActionHelpExecute(TObject *Sender);

public:      // User declarations
   __fastcall  TFormAlocaCapacitor(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormAlocaCapacitor(void);

private:   //métodos
   void __fastcall CancelaAlocacao(void);
   void __fastcall ConfirmaAlocacao(void);
	void __fastcall CriaUmCapacitor(void);
	void __fastcall FormIniciaPosicao(void);
   void __fastcall HabilitaOpcoes(void);
   void __fastcall IniciaGrid(void);
   void __fastcall IniciaListaCapacitoresOriginais(void);
   bool __fastcall LeParametros(void);
   bool __fastcall LeListaCapacitores(void);
   bool __fastcall Executa(void);
	void __fastcall ExibeListaAlocada(void);
	void __fastcall SalvaDados(void);
   //exibição do gráfico
   void __fastcall AjustaCurvas(TChart *chart);
   void __fastcall ExibeGrafico(TList *lisCAP);
	void __fastcall ExibeGrafico(VTEqpto *eqpto);
	void __fastcall ExportaAlocados(void);
	void __fastcall ExportaExistentes(void);
   void __fastcall IniciaListView(VTRede *rede); //DVK 2013.07.24
	void __fastcall Moldura(VTEqpto *eqpto=NULL);
	void __fastcall RestauraRedeOriginal(void);
   void __fastcall VerificaPrimariosIsolados(void);
   bool __fastcall VerificaPlanejamento(void); //DVK 2013.07.24
   void __fastcall Zoom(VTEqpto *eqpto);

private:   //objetos externos
   VTApl     *apl;
   VTObra    *obra_original;

private:   //dados locais
   int       num_alocacao;
   VTAlocaBC *alocaBC;
   VTObra    *obra;
   TList     *lisTabCap;
   TList     *lisEQP;
   TList     *lisCAP_ORIG;
   //TList     *lisCAP_Removidos; //FKM - 2015.09.22
   struct   {//estrutura p/ controle da ordenação do LView
            TListColumn *Column;
            bool        OrdemCrescente;
            }LViewSort;
};

//---------------------------------------------------------------------------
#endif
//eof

