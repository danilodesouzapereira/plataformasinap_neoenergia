//---------------------------------------------------------------------------

#ifndef TFormRelTensoesH
#define TFormRelTensoesH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Fontes\Constante\Fases.h>
//---------------------------------------------------------------------------
class VTApl;
//---------------------------------------------------------------------------
class TFormRelTensoes : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TActionList *ActionList1;
	TAction *ActionFechar;
	TImageList *ImageList1;
	TListView *LViewTensoes;
	TAction *ActionExporta;
	TSaveDialog *SaveDialogAnalise;
	TToolButton *ToolButton2;
	void __fastcall ActionFecharExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ActionExportaExecute(TObject *Sender);

private:	// User declarations
	void __fastcall PreencheLView(void);

public:		// User declarations
	__fastcall TFormRelTensoes(TComponent* Owner, VTApl *apl, TLineSeries* SeriesVA,
									 TLineSeries* SeriesVB, TLineSeries* SeriesVC, bool tensao);
	__fastcall ~TFormRelTensoes(void);

private: //objs externos
	VTApl 		*apl;
	TLineSeries *SeriesVA;
	TLineSeries *SeriesVB;
	TLineSeries *SeriesVC;

};

//---------------------------------------------------------------------------
#endif
