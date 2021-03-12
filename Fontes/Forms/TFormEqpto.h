//---------------------------------------------------------------------------
#ifndef TFormEqptoH
#define TFormEqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTVetval;
class VTGrupo;
class VTPatamares;

//---------------------------------------------------------------------------
class TFormEqpto : public TForm
{
__published:   // IDE-managed Components
   TPageControl *PageControl;
   TTabSheet *TabSheetAtributo;
   TTabSheet *TabSheetDado;
   TActionList *ActionList;
   TAction *ActionAtributoChanged;
   TAction *ActionAtributoSelAll;
   TAction *ActionAtributoSelNone;
   TToolBar *ToolBar1;
   TCheckListBox *CLBoxAtributo;
   TToolButton *ToolButton1;
   TTabSheet *TabSheetFoto;
   TImage *Image;
   TToolBar *ToolBar4;
   TToolButton *butFirst;
   TToolButton *butLast;
   TToolButton *butPrev;
   TToolButton *butNex;
   TAction *ActionFotoFirst;
   TAction *ActionFotoLast;
   TAction *ActionFotoNext;
   TAction *ActionFotoPrev;
   TImageList *ImageList1;
   TPanel *PanelTView;
   TTreeView *TView;
   TPanel *PanelGrafico;
   TSplitter *Splitter;
   TCheckListBox *CLBoxSerie;
   TChart *chart;
   TFastLineSeries *Series1;
   TFastLineSeries *Series2;
   TFastLineSeries *Series3;
   TFastLineSeries *Series4;
   THeaderControl *TViewHeader;
   TBevel *Bevel1;
   TToolButton *ButPadronizar;
   TAction *ActionAtributoAllEqpto;
   TBitBtn *butSelAll;
   TBitBtn *butSelNone;

   void __fastcall ActionAtributoChangedExecute(TObject *Sender);
   void __fastcall ActionAtributoSelAllExecute(TObject *Sender);
   void __fastcall ActionAtributoSelNoneExecute(TObject *Sender);
   void __fastcall ActionFotoFirstExecute(TObject *Sender);
   void __fastcall ActionFotoLastExecute(TObject *Sender);
   void __fastcall ActionFotoNextExecute(TObject *Sender);
   void __fastcall ActionFotoPrevExecute(TObject *Sender);
   void __fastcall CLBoxSerieClickCheck(TObject *Sender);
   void __fastcall CLBoxSerieDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
   void __fastcall TViewAdvancedCustomDrawItem(
          TCustomTreeView *Sender, TTreeNode *Node, TCustomDrawState State,
          TCustomDrawStage Stage, bool &PaintImages, bool &DefaultDraw);
   void __fastcall TViewHeaderSectionResize(
          THeaderControl *HeaderControl, THeaderSection *Section);
   void __fastcall TViewClick(TObject *Sender);
   void __fastcall ActionAtributoAllEqptoExecute(TObject *Sender);

public:      // User declarations
        __fastcall TFormEqpto(TComponent* Owner, TWinControl *parent, VTApl *apl);
        __fastcall ~TFormEqpto(void);
   void __fastcall Ativo(void);
   void __fastcall Atualiza(void);
   void __fastcall DefineEqpto(VTEqpto *eqpto);
   bool __fastcall ExportaRelatorioMedidor(void);

private:   //métodos
   void     __fastcall AtualizaAtributoAllEqpto(void);
   void     __fastcall ChartInsereCurva(VTVetval *vetval, TChartSeries *serie, TColor cor, bool first);
   TColor   __fastcall DefineCorSerie(int n);
   void     __fastcall EscondeGrafico(void);
   void     __fastcall ExibeGrafico(TList *lisVET);
   void     __fastcall ExportaGrupo(VTGrupo *grupo);
   void     __fastcall ExportaLisDado(TList *lisDADO);
   void     __fastcall ExportaLisVetval(TList *lisVETVAL);
   void     __fastcall HabilitaActionFoto(void);
   void     __fastcall IniciaCLBoxAtributo(TList *lisATR);
   void     __fastcall IniciaStringListFoto(void);
   VTEqpto* __fastcall TViewEqptoSelecionado(void);
   void     __fastcall TViewInicia(void);
   void     __fastcall TViewInsereGrupo(TTreeNode *node_pai, VTGrupo *grupo);
   void     __fastcall TViewInsereLisDado(TTreeNode *node_pai, TList *lisDADO);
   void     __fastcall TViewInsereLisGrupo(TTreeNode *node_pai, TList *lisGRUPO);
   void     __fastcall TViewInsereLisVetval(TTreeNode *node_pai, TList *lisVETVAL);
   bool     __fastcall TViewSelectionChanged(TList *lisVET);

private:   //objetos externos
   VTApl       *apl;
   VTEqpto     *eqpto;
   VTPatamares *patamares;

private:   //dados locais
   VTGrupo     *grupo;
   TList       *lisATR;
   TList       *lisVET;
   TList       *lisEQP;
   TStringList *Lines;

   struct   {
            int         index;
            TStringList *strlis;
            } foto;
   struct strCFG {//atributos pré-selecionados por Eqpto p/ o TeeChart
                 unsigned chart_atrib;
                 } cfg_bar,  cfg_can,   cfg_cap,   cfg_caps, cfg_car,  cfg_chv,
                   cfg_ger,  cfg_med,   cfg_reat,  cfg_redu,  cfg_regu, cfg_sup,
                   cfg_trf,  cfg_trf3,  cfg_tre,   cfg_zzz,  *cfg;
};

//---------------------------------------------------------------------------
#endif
//eof
