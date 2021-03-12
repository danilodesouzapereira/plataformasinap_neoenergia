//---------------------------------------------------------------------------
#ifndef TFormDiagFlowH
#define TFormDiagFlowH

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
#include "..\Forms\TFormEdt.h"
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTConsulta;
class VTEqpto;
class VTFlow;
class VTGrafico;
class VTLigacao;
class VTPatamar;
class VTPatamares;
class VTRede;
class VTRedes;
class VTSuprimento;

//---------------------------------------------------------------------------
enum enumNIVEL
   {nivelNORMAL = 0, nivelREGULAR, nivelCRITICO, nivelPROIBIDO};
enum enumORG
   {orgRede = 0, orgPatamar, orgCarreg};

//---------------------------------------------------------------------------
struct strDIAG
   {
   VTEqpto   *eqpto;
   int        nivel;
   TTreeNode *node;
   int        patamar;
   VTRede    *rede;
   int        tipo;
   };

//---------------------------------------------------------------------------
class TFormDiagFlow : public TFormEdt
{
__published:	// IDE-managed Components
   TPanel *Panel1;
   TPopupMenu *popOrganizar;
   TMenuItem *optRede;
   TMenuItem *optPatamar;
   TMenuItem *optCarreg;
   TImageList *ImageList1;
   TGroupBox *GroupBox3;
   TGroupBox *GroupBox4;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TTrackBar *TrackBar1;
   TEdit *Edit1;
   TTrackBar *TrackBar3;
   TEdit *Edit3;
   TEdit *Edit2;
   TGroupBox *GroupBox5;
   TLabel *Label4;
   TLabel *Label5;
   TLabel *Label6;
   TTrackBar *TrackBar4;
   TTrackBar *TrackBar5;
   TEdit *Edit4;
   TEdit *Edit5;
   TEdit *Edit6;
   TTrackBar *TrackBar6;
   TGroupBox *GroupBox1;
   TGroupBox *GroupBox2;
   TTreeView *TViewDiag;
   TChart *Chart;
   TBarSeries *Series1;
   TRadioGroup *RadioGroup;
   TAction *ActionAtualiza;
   TAction *ActionFecha;
   TAction *ActionTChart;
   TTrackBar *TrackBar2;
   TToolButton *butAtualizar;
   TToolButton *butOrganizar;
   void __fastcall ActionAtualizaExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall ActionTChartExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall TViewDiagClick(TObject *Sender);
   void __fastcall TrackBar1Change(TObject *Sender);
   void __fastcall TrackBar2Change(TObject *Sender);
   void __fastcall TrackBar3Change(TObject *Sender);
   void __fastcall TrackBar4Change(TObject *Sender);
   void __fastcall TrackBar5Change(TObject *Sender);
   void __fastcall TrackBar6Change(TObject *Sender);
   void __fastcall optRedeClick(TObject *Sender);
   void __fastcall optPatamarClick(TObject *Sender);
   void __fastcall optCarregClick(TObject *Sender);
   void __fastcall TViewDiagDblClick(TObject *Sender);

public:		// User declarations
        __fastcall TFormDiagFlow(TForm* Owner, VTApl* apl, TWinControl *parent);
        __fastcall ~TFormDiagFlow(void);
   void __fastcall Ativo(void);
   void __fastcall Atualiza(void);

private:
   int      __fastcall AchaNivelBarra(double valor);
   int      __fastcall AchaNivelCarreg(double valor);
   VTRede*  __fastcall AchaRedeBarra(VTBarra *barra);
   bool     __fastcall DiagBarra(VTBarra* barra);
   bool     __fastcall DiagLigacao(VTLigacao* ligacao);
   void     __fastcall Diagnostico(void);
   bool     __fastcall DiagSuprimento(VTSuprimento* suprimento);
   void     __fastcall LimpaLisDiag(TList *list);
   void     __fastcall LimpaTreeView(void);
   void     __fastcall MarcaTreeView(void);
   void     __fastcall Moldura(VTEqpto *eqpto);
   void     __fastcall PreenchePorCarreg(void);
   void     __fastcall PreenchePorPatamar(void);
   void     __fastcall PreenchePorRede(void);
   void     __fastcall PreencheTChart(void);
   void     __fastcall PreencheTreeView(void);
   TList*   __fastcall SeparaPorCarreg(TList *list, int nivel);
   TList*   __fastcall SeparaPorPatamar(TList *list, int patamar);
   TList*   __fastcall SeparaPorRede(TList *list, VTRede *rede);

private:	// User declarations
   double      barra_critico;
   double      barra_regular;
   double      barra_normal;
   double      valor_critico;
   double      valor_regular;
   double      valor_normal;
   int         organizar;
   TList       *lisEQP;
   TList       *lisDIAG;
   TList       *lisPorRede;
   TList       *lisPorPatamar;
   TList       *lisPorCarreg;
   VTApl       *apl;
   VTConsulta  *consulta;
   VTFlow      *flow;
   VTGrafico   *grafico;
   VTRedes     *redes;
   VTPatamares *patamares;
};

//---------------------------------------------------------------------------
#endif
//eof
