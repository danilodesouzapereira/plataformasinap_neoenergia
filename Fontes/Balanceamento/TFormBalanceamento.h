//---------------------------------------------------------------------------
#ifndef TFormBalanceamentoH
#define TFormBalanceamentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <Forms.hpp>
#include "TFormEdt.h"
#include <System.Actions.hpp>
#include <Vcl.Buttons.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <System.ImageList.hpp>
#include <Complex>
#include <Buttons.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include "TFormEdt.h"
using namespace std;

class TBalanceamento;
class VTApl;
class VTArvore;
class VTBarra;
class VTEdt;
class VTEdita;
class VTEqpto;
class VTObra;
class VTRede;
class VTOrdena;
class VTZonas;

//---------------------------------------------------------------------------
class TFormBalanceamento : public TFormEdt
{
__published:   // IDE-managed Components
   TPanel *Panel1;
   TSplitter *Splitter1;
   TGroupBox *GroupBox1;
   TTreeView *TViewRedes;
   TAction *ActionBalancear;
   TAction *ActionSelEqpto;
   TAction *ActionSair;
   TPanel *PanelAntes;
   TPanel *PanelPQa;
   TGroupBox *GroupBox2;
   TListView *listViewPQantes;
   TPanel *PanelPQ;
   TChart *chartPantes;
   TPieSeries *SeriesCarregP;
   TChart *chartQantes;
   TPieSeries *SeriesCarregQ;
   TPanel *PanelSant;
   TChart *chartS3Fantes;
   TLineSeries *SeriesAa;
   TLineSeries *SeriesBa;
   TLineSeries *SeriesCa;
   TLineSeries *SeriesABCa;
	TAction *ActionHelp;
   TToolBar *ToolBarRight;
	TToolButton *ToolButton5;
	TToolButton *ToolButton6;
	TToolButton *ToolButton7;
	TToolButton *ToolButton8;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionBalancearExecute(TObject *Sender);
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall ActionSelEqptoExecute(TObject *Sender);
   void __fastcall ActionSairExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall TViewRedesClick(TObject *Sender);
	void __fastcall ActionHelpExecute(TObject *Sender);

public:      // User declarations
   __fastcall TFormBalanceamento(TComponent* form_owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormBalanceamento(void);

private:
   void        __fastcall AnalisaCarregamento(VTRede *rede);
	void        __fastcall AtualizaDados(VTRede *rede);
	VTRede*		__fastcall ExisteRede(AnsiString cod_rede, TList *lisREDE);
   bool        __fastcall ExisteRedeBalanceada(VTRede* rede);
   void        __fastcall FormIniciaPosicao(void);
   void        __fastcall Inicia(void);
   void        __fastcall PreencheChartsOriginal(void);
   void        __fastcall PreencheLViewOriginal(VTRede *rede);
   void        __fastcall PreencheTreeView(VTRede  *rede);
   void        __fastcall PreencheGraficoPotAparente(VTRede *rede);  //S [kVA]
   void        __fastcall TViewRedesInicia(void);
   VTRede*     __fastcall TViewRedeSelecionada(void);
   bool        __fastcall VerificaPlanejamento(void);

private:   // User declarations
   VTApl          *apl;
   VTArvore       *arvore;
//   VTEdita        *edita;
   VTEdt          *edt_busca;
//   VTObra         *obra_original, *obra;
   VTOrdena       *ordena;
   VTZonas        *zonas;
   TList          *lisRedeBalanc;
   complex<double> sa, sb, sc, s3;  

};
//---------------------------------------------------------------------------
#endif
