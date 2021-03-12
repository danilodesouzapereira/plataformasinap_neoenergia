//---------------------------------------------------------------------------
#ifndef TFormDiagEqptoH
#define TFormDiagEqptoH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Menus.hpp>
#include <System.ImageList.hpp>
//---------------------------------------------------------------------------
enum modoFORM {
               modoGERAL=-1,
               modoBARRA=0,
               modoTRECHO,
               modoCHAVE,
               modoCAPACITOR,
               modoTRAFO,
               modoREG,
               modoCARGA,
               modoARRANJO,
               modoCABO,
               modoREDE,
               modoBLOCOISOLADO
               };
//---------------------------------------------------------------------------
//class TFormDiagEqptoTrecho;
class TDiagTopo;
class VTDiagEqpto;
class VTApl;
class VTProgresso;
//---------------------------------------------------------------------------
class TFormDiagEqpto : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBarSelecao;
   TToolButton *tbutTrecho;
   TToolButton *tbutGeral;
   TToolButton *separador;
   TPanel *PanelMain;
   TGroupBox *GBoxVerificar;
   TGroupBox *GBoxRelatorio;
   TSplitter *Splitter1;
   TToolBar *ToolBar2;
   TImageList *ImageList1;
   TToolButton *tbConfigLimites;
   TActionList *ActionList1;
   TCheckListBox *CheckListBox1;
   TAction *ActionExibeGeral;
   TAction *ActionExibeTrecho;
   TAction *ActionConfigura;
   TToolBar *ToolBar3;
   TToolButton *butExportRel;
   TAction *ActionExportaRel;
   TBitBtn *butSelNone;
   TBitBtn *butSelAll;
   TToolButton *ToolButtonExecutar;
   TAction *ActionExecutaDiag;
   TPanel *PanelLeft;
   TToolButton *tbutBarra;
   TToolButton *tbutChave;
   TToolButton *tbutCarga;
   TToolButton *tbutRede;
   TToolButton *tbutArranjo;
   TToolButton *tbutCabo;
   TToolButton *tbutCapacitor;
   TToolButton *tbutTrafo;
   TToolButton *tbutReg;
   TToolButton *tbutBIso;
   TToolButton *ToolButton13;
   TAction *ActionExibeChave;
   TAction *ActionExibeBarra;
   TGroupBox *gbInfo;
   TListBox *ListBox1;
   TAction *ActionZoom;
   TToolButton *ToolButton4;
   TAction *ActionExibeCapacitor;
   TAction *ActionExibeArranjo;
   TAction *ActionExibeCabo;
   TAction *ActionExibeTrafo;
   TAction *ActionExibeRede;
   TSaveDialog *SaveDialog;
   TListView *lvLog;
   TAction *ActionExibeCarga;
   TAction *ActionExibeBIso;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TAction *ActionHelp;
   TPanel *PanelCor;
   TLabel *Label1;
   TPanel *PanelTB;
   TPanel *PanelSelCor;
   TColorDialog *ColorDialog;
    TAction *ActionExibeReg;
	void __fastcall ActionConfiguraExecute(TObject *Sender);
   void __fastcall ActionExecutaDiagExecute(TObject *Sender);
   void __fastcall ActionExibeArranjoExecute(TObject *Sender);
   void __fastcall ActionExibeBarraExecute(TObject *Sender);
   void __fastcall ActionExibeBIsoExecute(TObject *Sender);
   void __fastcall ActionExibeCaboExecute(TObject *Sender);
   void __fastcall ActionExibeCapacitorExecute(TObject *Sender);
   void __fastcall ActionExibeCargaExecute(TObject *Sender);
   void __fastcall ActionExibeChaveExecute(TObject *Sender);
   void __fastcall ActionExibeGeralExecute(TObject *Sender);
   void __fastcall ActionExibeRedeExecute(TObject *Sender);
   void __fastcall ActionExibeRegExecute(TObject *Sender);
   void __fastcall ActionExibeTrafoExecute(TObject *Sender);
   void __fastcall ActionExibeTrechoExecute(TObject *Sender);
   void __fastcall ActionExportaRelExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionZoomExecute(TObject *Sender);
   void __fastcall butSelAllClick(TObject *Sender);
   void __fastcall butSelNoneClick(TObject *Sender);
   void __fastcall lvLogClick(TObject *Sender);
   void __fastcall lvLogColumnClick(TObject *Sender, TListColumn *Column);
   void __fastcall lvLogCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);
   void __fastcall lvLogDblClick(TObject *Sender);
   void __fastcall PanelSelCorClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// User declarations
   void __fastcall   AlteraStatusToolButton(TToolButton *toolButton);
   void __fastcall   AlteraStatusConfigButton(int modoFORM);
   void __fastcall   AtualizaForm(VTDiagEqpto *diagEqpto);
   void __fastcall   AtualizaGrafico(void);
   void __fastcall   AutoSizeTListViewColumns(TListView * ListView);
   bool __fastcall   ExecutaDiagEqpto(VTDiagEqpto *diagEqpto);
   void __fastcall   ExportaRel(TStringList *log);
   void __fastcall   IniciaCheckListGeral(void);
   void __fastcall   LeCheckList(void);
   void __fastcall   LeCheckListGeral(void);
   void __fastcall   PreencheLogGeral(void);

private:	// Dados Externos
   VTApl *apl;

private:	// Dados Internos
   int         modoForm;
   TDiagTopo  *diagTopo;
   TList      *emptyList;
   VTProgresso *progresso;
   struct{
         bool  arranjo;
         bool  barra;
         bool  blocoIsolado;
         bool  cabo;
         bool  capacitor;
         bool  carga;
         bool  chave;
         bool  rede;
         bool  reg;
         bool  trafo;
         bool  trecho;
         }dgGeral;
   struct   {
            int    ColumnIndex;
            bool   OrdemCrescente;
            }ListViewSort;

   //TFormDiagEqptoTrecho *formDiagEqptoTrecho;
   //TList  *lisDiagEqptoElem;

public:		// User declarations
   __fastcall TFormDiagEqpto(TComponent* Owner, VTApl *apl, TWinControl *parent);
   __fastcall ~TFormDiagEqpto(void);
};

//---------------------------------------------------------------------------
#endif
//eof

