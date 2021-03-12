//---------------------------------------------------------------------------
#ifndef TFormEdtTrfH
#define TFormEdtTrfH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Grids.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <CheckLst.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTGeral;
class VTBarra;
class VTTrafo;
class VTRede;
struct strIMP;

//---------------------------------------------------------------------------
class TFormEdtTrf : public TForm
{
__published:	// IDE-managed Components
   TPageControl *pgCtrl;
   TTabSheet *TabSheetTrafo;
   TTabSheet *TabSheetPrimario;
   TTabSheet *TabSheetSecundario;
   TLabel *Label1;
   TLabel *LabelSnom;
   TLabel *Label9;
   TLabel *Label14;
   TLabel *Label13;
   TLabel *Label15;
   TEdit *EditCodigo;
   TEdit *edtSnom;
   TEdit *EditR0;
   TEdit *EditX0;
   TEdit *EditR1;
   TEdit *EditX1;
   TGroupBox *gboxPrimario;
   TComboBox *CBoxPriLiga;
   TLabel *Label20;
   TTabSheet *TabSheetAtributo;
   TComboBox *CBoxPriBar;
   TEdit *EditPriVnom;
   TEdit *EditPriVtap;
   TLabel *Label7;
   TLabel *LabelPriVnom;
   TLabel *LabelPriTap;
   TLabel *LabelPriLiga;
   TGroupBox *gboxPriZterra;
   TLabel *LabelPriRterra;
   TLabel *LabelPriXterra;
   TEdit *EditPriRat;
   TEdit *EditPriXat;
   TGroupBox *gboxSecDefasagem;
   TCheckBox *checkAutoTrafo;
   TLabel *Label3;
   TComboBox *CBoxLtcTipo;
   TToolBar *ToolBar;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TLabel *Label4;
   TComboBox *CBoxRede;
   TLabel *Label5;
   TEdit *edtPerdaFe;
   TAction *ActionSelFoto;
   TAction *ActionSelPadrao;
   TComboBox *CBoxSbase;
   TPanel *PanelTrafoLeft;
   TPanel *PanelTrafoRight;
   TPanel *PanelCodigo;
   TPanel *PanelLtcBarra;
   TPanel *PanelLtcTensao;
   TPanel *PanelAutoTrafo;
   TPanel *PanelLtc;
   TPanel *PanelPerdaFerro;
   TPanel *PanelSnom;
   TPanel *PanelRede;
   TPanel *PanelZ0Z1;
   TPanel *PanelPrimarioRight;
   TPanel *PanelPrimarioLeft;
   TPanel *PanelPriTensao;
   TPanel *PanelPriLigacao;
   TPanel *PanelPriBarra;
   TEdit *EditLtcVpu;
   TLabel *Label6;
   TPanel *PanelTrafoTop;
   TComboBox *CBoxLtcBar;
   TLabel *Label8;
   TPanel *PanelSecLeft;
   TGroupBox *gboxSecundario;
   TPanel *PanelSecBarra;
   TLabel *Label11;
   TComboBox *CBoxSecBar;
   TPanel *PanelSecLigacao;
   TLabel *LabelSecLiga;
   TComboBox *CBoxSecLiga;
   TPanel *PanelSecTensao;
   TLabel *LabelSecTap;
   TLabel *LabelSecVnom;
   TEdit *EditSecVtap;
   TEdit *EditSecVnom;
   TPanel *PanelSecRight;
   TGroupBox *gboxSecZterra;
   TLabel *Label21;
   TLabel *Label22;
   TEdit *EditSecRat;
   TEdit *EditSecXat;
   TCheckListBox *CLBoxAtributo;
   TGroupBox *GBoxLTC;
   TComboBox *CBoxSecDefasagem;
   TPanel *PanelR0;
   TPanel *PanelX0;
   TPanel *PanelR1;
   TPanel *PanelX1;
   TPanel *PanelSbase;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TPanel *Panel5;
   TPanel *Panel6;
   TPanel *Panel7;
   TPanel *Panel8;

   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall CBoxPriBarChange(TObject *Sender);
   void __fastcall CBoxSecBarChange(TObject *Sender);
   void __fastcall CBoxPriLigaChange(TObject *Sender);
   void __fastcall CBoxSecLigaChange(TObject *Sender);
   void __fastcall checkAutoTrafoClick(TObject *Sender);
   void __fastcall CBoxLtcTipoChange(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall ActionSelFotoExecute(TObject *Sender);
   void __fastcall ActionSelPadraoExecute(TObject *Sender);
   void __fastcall CBoxSbaseChange(TObject *Sender);
   void __fastcall CLBoxAtributoClickCheck(TObject *Sender);
   void __fastcall CBoxLtcBarChange(TObject *Sender);
   void __fastcall pgCtrlChange(TObject *Sender);
   void __fastcall CBoxSecDefasagemChange(TObject *Sender);
   void __fastcall CBoxRedeChange(TObject *Sender);

public:		// User declarations
        __fastcall  TFormEdtTrf(TComponent* Owner, VTApl *apl, VTTrafo *trafo, bool criar_copia);
        __fastcall  TFormEdtTrf(TComponent* Owner, VTApl *apl, TList *lisEXT, bool criar_copia);
        __fastcall ~TFormEdtTrf(void);
   void __fastcall  OpcaoRetira(bool enabled);
   
private: //métodos  AtualizaTrafoOriginal
   void     __fastcall AtualizaAutoTrafo(VTTrafo *trafo);
   void     __fastcall AtualizaCodigo(VTTrafo *trafo);
   void     __fastcall AtualizaDados(void);
   void     __fastcall AtualizaLtcBar(VTTrafo *trafo);
   void     __fastcall AtualizaLtcTipo(VTTrafo *trafo);
   void     __fastcall AtualizaLtcVpu(VTTrafo *trafo);
   void     __fastcall AtualizaPotenciaNominal(VTTrafo *trafo);
   void     __fastcall AtualizaPerdaFerro(VTTrafo *trafo);
   void     __fastcall AtualizaPriBarra(VTTrafo *trafo);
   void     __fastcall AtualizaPriLigacao(VTTrafo *trafo);
   void     __fastcall AtualizaPriTensao(VTTrafo *trafo);
   void     __fastcall AtualizaPriZat(VTTrafo *trafo);
   void     __fastcall AtualizaSecBarra(VTTrafo *trafo);
   void     __fastcall AtualizaSecLigacao(VTTrafo *trafo);
   void     __fastcall AtualizaSecTensao(VTTrafo *trafo);
   void     __fastcall AtualizaSecZat(VTTrafo *trafo);
   void     __fastcall AtualizaSecDefasagem(VTTrafo *trafo);
   void     __fastcall AtualizaRede(VTTrafo *trafo);
   void     __fastcall AtualizaZ0Z1(VTTrafo *trafo);
   void     __fastcall CBoxLtcBarInicia(void);
   void     __fastcall CBoxPriBarInicia(void);
   void     __fastcall CBoxSecBarInicia(void);
   void     __fastcall CBoxRedeInicia(void);
   //bool     __fastcall LeZseqTrafo(void);
   void     __fastcall CLBoxAtributoInicia(void);
   bool     __fastcall ConverteZ0Z1(strIMP &z0, strIMP &z1, double sbase_old, double sbase_new);
   void     __fastcall DefValoresDefaultTrafo(void);
   //void     __fastcall ExibeVnomTrafo(void);
   //void     __fastcall ExibeZseqTrafo(void);
   void     __fastcall ExibeAutoTrafo(void);
   void     __fastcall ExibeCodigo(void);
   void     __fastcall ExibeDados(void);
   void     __fastcall ExibeDadosCondicionais(void);
   void     __fastcall ExibeLtcBar(void);
   void     __fastcall ExibeLtcTipo(void);
   void     __fastcall ExibeLtcVpu(void);
   void     __fastcall ExibePerdaFerro(void);
   void     __fastcall ExibePotenciaNominal(void);
   void     __fastcall ExibePriBarra(void);
   void     __fastcall ExibePriLigacao(void);
   void     __fastcall ExibePriTensao(void);
   void     __fastcall ExibePriZat(void);
   void     __fastcall ExibeSecBarra(void);
   void     __fastcall ExibeSecDefasagem(void);
   void     __fastcall ExibeSecLigacao(void);
   void     __fastcall ExibeSecTensao(void);
   void     __fastcall ExibeSecZat(void);
   void     __fastcall ExibeRede(void);
   void     __fastcall ExibeZ0Z1(void);
   VTRede*  __fastcall ExisteRede(VTBarra *barra);
   void     __fastcall FormIniciaPosicao(void);
   bool     __fastcall LeAutoTrafo(void);
   bool     __fastcall LeCodigo(void);
   bool     __fastcall LeDadosTrafo(void);
   bool     __fastcall LeLtcBar(void);
   bool     __fastcall LeLtcTipo(void);
   bool     __fastcall LeLtcVpu(void);
   bool     __fastcall LePerdaFerro(void);
   bool     __fastcall LePotenciaNominal(void);
   bool     __fastcall LePriBarra(void);
   bool     __fastcall LePriDefasagem(void);
   bool     __fastcall LePriLigacao(void);
   bool     __fastcall LePriTensao(void);
   bool     __fastcall LePriZat(void);
   bool     __fastcall LeSecBarra(void);
   bool     __fastcall LeSecDefasagem(void);
   bool     __fastcall LeSecLigacao(void);
   bool     __fastcall LeSecTensao(void);
   bool     __fastcall LeSecZat(void);
   bool     __fastcall LeRede(void);
   bool     __fastcall LeZ0Z1(void);
   bool     __fastcall LeZ0Z1(strIMP &z0, strIMP &z1);
   void     __fastcall Moldura(VTEqpto *eqpto=NULL);
   void     __fastcall MolduraBarraLtc(void);
   void     __fastcall MolduraBarraSec(void);
   void     __fastcall MolduraBarraPri(void);

private: //dados externos
   VTApl   *apl;
   VTGeral *geral;

private: //dados locais
   bool    aviso_enabled;
   bool    criar_copia;
   double  sbase_100mva;
   TList   *lisTRF;
   TList   *lisTMP;
   VTTrafo *clone;
};

#endif
//---------------------------------------------------------------------------
//eof

