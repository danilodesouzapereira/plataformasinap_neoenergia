//---------------------------------------------------------------------------
#ifndef TFormCustoNodalH
#define TFormCustoNodalH

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
#include <CheckLst.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include "..\Forms\TFormEdt.h"
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCustoNodal;

//---------------------------------------------------------------------------
class TFormCustoNodal : public TFormEdt
{
__published:	// IDE-managed Components
   TSplitter *Splitter1;
   TCheckListBox *CLBoxSerie;
   TAction *ActionRedeChanged;
   TPageControl *PageControl;
   TTabSheet *TabSheetGrafico;
   TTabSheet *TabSheetPlanilha;
   TChart *chart;
   TFastLineSeries *Series1;
   TFastLineSeries *Series2;
   TFastLineSeries *Series3;
   TFastLineSeries *Series4;
   TListView *LView;
   void __fastcall CLBoxSerieClickCheck(TObject *Sender);
   void __fastcall CLBoxSerieDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionRedeChangedExecute(TObject *Sender);

public:		// User declarations
        __fastcall TFormCustoNodal(TForm* Owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormCustoNodal(void);
   void __fastcall Atualiza(void);
   void __fastcall DefinePatamar(int ind_pat);

private:	//métodos
   TColor __fastcall ChartCorSerie(int n);
   void   __fastcall ChartInicia(void);
   void   __fastcall ChartInsereCurva(VTCustoNodal *custo_nodal, TChartSeries *serie, TColor cor, bool first);
   void   __fastcall LViewInicia(void);

private:	//objetos externos
   //VTApl *apl;

private:	//dados locais
   VTApl *apl;
};

//---------------------------------------------------------------------------
#endif
//eof

