//---------------------------------------------------------------------------
#ifndef TFormResultadoBalancH
#define TFormResultadoBalancH

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
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <System.Actions.hpp>
#include <complex>
using namespace std;

class VTBalanceamento;
class VTApl;
class VTEdita;
class VTObra;
class VTRede;
//---------------------------------------------------------------------------
class TFormResultadoBalanc : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *butEdt;
   TToolButton *ToolButton5;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TPopupMenu *PopupMenu;
   TMenuItem *mnuAlignLeft;
   TMenuItem *mnuAlignRight;
   TMenuItem *mnuAlignDown;
   TMenuItem *mnuAlignUp;
   TMenuItem *mnuAlignNode;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionAlignDown;
   TAction *ActionAlignLeft;
   TAction *ActionAlignNone;
   TAction *ActionAlignRight;
   TAction *ActionAlignUp;
   TAction *ActionEdt;
   TAction *ActionMouseOff;
   TAction *ActionMouseOn;
   TAction *ActionSelEqpto;
   TAction *ActionCancelar;
   TAction *ActionConfirmar;
   TAction *ActionBalancear;
   TAction *ActionRelCargas;
   TPanel *PanelPQa;
   TGroupBox *GroupBox2;
   TListView *LViewPQantes;
   TPanel *PanelPQantes;
   TChart *chartPantes;
   TPieSeries *SeriesCarregPantes;
   TChart *chartQantes;
   TPieSeries *SeriesCarregQantes;
   TPanel *PanelSant;
   TChart *chartS3Fantes;
   TLineSeries *SeriesAa;
   TLineSeries *SeriesBa;
   TLineSeries *SeriesCa;
   TLineSeries *SeriesABCa;
   TPanel *PanelAntes;
   TPanel *PanelDepois;
   TPanel *PanelPQd;
   TPanel *PanelPQdepois;
   TPanel *PanelSdep;
   TChart *chartS3Fdepois;
   TLineSeries *SeriesAd;
   TLineSeries *SeriesBd;
   TLineSeries *SeriesCd;
   TLineSeries *SeriesABCd;
   TChart *chartPdepois;
   TPieSeries *SeriesCarregPdepois;
   TChart *chartQdepois;
   TPieSeries *SeriesCarregQdepois;
   TGroupBox *GroupBox1;
   TListView *LViewPQdepois;
   TPanel *Panel;
   TLabel *Label1;
   TComboBox *CBoxPatamar;

   void __fastcall ActionCancelarExecute(TObject *Sender);
   void __fastcall ActionConfirmarExecute(TObject *Sender);
   void __fastcall ActionRelCargasExecute(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public:		// User declarations
   __fastcall  TFormResultadoBalanc(TComponent* Owner, VTApl* apl_owner, VTRede* rede);
	__fastcall ~TFormResultadoBalanc(void);

public:		// variável que indica se houve balanceamento
	bool sucesso;

private:
	void 			__fastcall AnalisaRede(void);
	void 			__fastcall CancelaBalanceamento(void);
	void 			__fastcall CBoxPatamarInicia(void);
	void 			__fastcall ConfirmaBalanceamento(void);
	void 			__fastcall FormIniciaPosicao(void);
	bool 			__fastcall Inicia(void);
	void 			__fastcall PreencheChartsBalanceado(void);
	void 			__fastcall PreencheChartsOriginal(void);
	void 			__fastcall PreencheLViewBalanceado(void);
	void 			__fastcall PreencheLViewOriginal(void);
	void 			__fastcall PreencheGraficoPotAparenteBalanc(void);
	void 			__fastcall PreencheGraficoPotAparenteOrig(void);
	void 			__fastcall RestauraRedeOriginal(void);
	AnsiString 	__fastcall TraduzZona(int tipo_zona);

private:   // User declarations
   VTApl           *apl;
   VTEdita         *edita;
   VTObra          *obra_original, *obra;
   VTRede          *rede;
   VTBalanceamento *balanceamento;
   complex<double> sa, sb, sc, s3;


};
//---------------------------------------------------------------------------
#endif
