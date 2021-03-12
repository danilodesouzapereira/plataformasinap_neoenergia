//---------------------------------------------------------------------------
#ifndef TFormRelCargaH
#define TFormRelCargaH
using namespace std;

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
#include <System.ImageList.hpp>
#include <complex>

//---------------------------------------------------------------------------
class TFormPlanilha;
class VTApl;
class VTArvore;
class VTProgresso;
//---------------------------------------------------------------------------
class TFormRelCarga : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList;
   TToolBar *ToolBar1;
   TListView *LView;
   TImageList *ImageList;
   TToolButton *ToolButton1;
   TAction *ActionExporta;
   TSaveDialog *SaveDialog;
	TPanel *PanelUnidade;
   TLabel *Label1;
   TComboBox *CBoxUnidade;
   TPageControl *PageControl;
   TTabSheet *TabSheetPlanilha;
   TTabSheet *TabSheetGrafico;
   TGroupBox *GBoxHieraquia;
   TGroupBox *GBoxDemPropria;
   TGroupBox *GBoxGrafico;
   TTreeView *TView;
   TStringGrid *SGridDemPropria;
	TPanel *PanelTipoRel;
   TLabel *Label2;
   TComboBox *CBoxRelatorio;
   TToolButton *ToolButton2;
   TChart *chart;
   //TChartAxis *chartAxis;
   TFastLineSeries *Series1;
   TFastLineSeries *Series2;
   TFastLineSeries *Series3;
	TComboBox *CBoxClasses;
	TToolButton *ToolButton3;
   TToolButton *ToolButton4;
   TToolButton *ToolButton5;
   TAction *ActionHelp;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall LViewColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall LViewCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
   void __fastcall ActionExportaExecute(TObject *Sender);
	void __fastcall TViewChange(TObject *Sender, TTreeNode *treenode);
	void __fastcall PageControlChange(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
	void __fastcall CBoxRelatorioChange(TObject *Sender);
	void __fastcall CBoxUnidadeChange(TObject *Sender);
	void __fastcall CBoxClassesChange(TObject *Sender);



public:		// User declarations
		  __fastcall  TFormRelCarga(TForm *Owner, VTApl *apl_owner, TWinControl *parent);
		  __fastcall ~TFormRelCarga(void);
	void __fastcall  Atualiza(void);

private:	//métodos
	AnsiString __fastcall Carga_Modelo(int modelo);
	void 		  __fastcall ChartInicia(void);
   void       __fastcall FormIniciaPosicao(void);
	void       __fastcall IniciaArvore(void);
	void 		  __fastcall IniciaCBoxClasses(void);
	int        __fastcall IniciaLisCarga(TList *lisREDE, TList *lisCARGA);
	void       __fastcall LViewInicia(void);
	bool       __fastcall SelecionaArquivo(void);
   void       __fastcall SGridInicia(void);
   void       __fastcall SomaDemandaCarga(TList *lisCAR, int ind_pat, complex<double> &s_kva);
   void       __fastcall TViewInicia(void);
   void       __fastcall TViewInsereNodesChild(TTreeNode *treenode);

private:	//objetos externos
   VTApl     *apl;

private:	//dados locais
   VTProgresso *progresso;
	VTArvore 	*arvore;
   TList       *lisREDE, *lisREDE_MON;
	TList    	*lisEQP;
   TList    	*lisAUX;
   //TTreeNode *node_changing;
   //TTreeNode *node_sel;    //???nó fixo p/ exibir todas as redes selecionadas
   struct   {
            int    ColumnIndex;
            bool   OrdemCrescente;
            }ListViewSort;
};

//---------------------------------------------------------------------------
#endif
//eof
