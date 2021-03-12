//---------------------------------------------------------------------------
#ifndef TFormEdtTrf3EH
#define TFormEdtTrf3EH

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
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRede;
class VTTrafo3E;

//---------------------------------------------------------------------------
class TFormEdtTrf3E : public TForm
{
__published:	// IDE-managed Components
   TPageControl *pgCtrl;
   TTabSheet *tsTrafo;
   TTabSheet *TabSheet2;
   TTabSheet *TabSheet3;
   TTabSheet *TabSheet4;
   TGroupBox *gboxPrimario;
   TLabel *LabelPriVnom;
   TLabel *LabelPriLiga;
   TComboBox *cboxPriLiga;
   TLabel *LabelPriSnom;
   TEdit *edtPriSnom;
   TGroupBox *gBoxSecundario;
   TComboBox *cboxSecLiga;
   TEdit *edtSecSnom;
   TGroupBox *gBoxTerciario;
   TComboBox *cboxTerLiga;
   TEdit *edtTerSnom;
   TLabel *Label7;
   TComboBox *cboxPriBarra;
   TComboBox *cboxSecBarra;
   TComboBox *cboxTerBarra;
   TEdit *edtPriVnom;
   TEdit *edtSecVnom;
   TEdit *edtTerVnom;
   TGroupBox *GroupBox4;
   TStringGrid *sgrid;
   TLabel *LabelPriTap;
   TEdit *edtPriTap;
   TLabel *Label9;
   TLabel *LabelSecVnom;
   TLabel *LabelSecTap;
   TLabel *LabelSecLiga;
   TLabel *LabelSecSnom;
   TEdit *edtSecTap;
   TEdit *edtTerTap;
   TLabel *Label13;
   TLabel *LabelTerVnom;
   TLabel *LabelTerTap;
   TLabel *LabelTerLiga;
   TLabel *LabelTerSnom;
   TGroupBox *gboxPriZterra;
   TLabel *LabelPriRterra;
   TLabel *LabelPriXterra;
   TEdit *edtPriRterra;
   TEdit *edtPriXterra;
   TGroupBox *gboxSecZterra;
   TLabel *LabelSecRterra;
   TLabel *LabelSecXterra;
   TEdit *edtSecRterra;
   TEdit *edtSecXterra;
   TGroupBox *gboxTerZterra;
   TLabel *LabelTerRterra;
   TLabel *LabelTerXterra;
   TEdit *edtTerRterra;
   TEdit *edtTerXterra;
   TToolBar *ToolBar2;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TEdit *EditCodigo;
   TEdit *edtPerdaFe;
   TLabel *Label2;
   TLabel *Label5;
   TComboBox *cboxRede;
   TLabel *Label4;
   TLabel *Label20;
   TComboBox *CBoxSbase;
   TGroupBox *GBoxSecDefasagem;
   TComboBox *CBoxSecDefasagem;
   TGroupBox *GBoxTerDefasagem;
   TComboBox *CBoxTerDefasagem;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TPanel *Panel5;
   TPanel *Panel6;
   TPanel *Panel7;
   TPanel *Panel8;
   TPanel *Panel9;
   TPanel *Panel10;
   TPanel *Panel11;
   TPanel *Panel12;
   TPanel *Panel13;
   TPanel *Panel14;
   TPanel *Panel15;
   TPanel *Panel16;
   TPanel *Panel17;
   TPanel *Panel19;
   TPanel *Panel20;
   TPanel *Panel21;
   TPanel *Panel22;
   TPanel *Panel23;
   TPanel *Panel24;
   TPanel *Panel25;
   TPanel *Panel26;
   TPanel *Panel27;
   TPanel *Panel28;
   TPanel *Panel29;
   TPanel *Panel30;
   TPanel *Panel31;
   TPanel *Panel32;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall cboxPriLigaChange(TObject *Sender);
   void __fastcall cboxPriBarraChange(TObject *Sender);
   void __fastcall cboxSecBarraChange(TObject *Sender);
   void __fastcall cboxTerBarraChange(TObject *Sender);
   void __fastcall cboxSecLigaChange(TObject *Sender);
   void __fastcall cboxTerLigaChange(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall CBoxSbaseChange(TObject *Sender);

public:		// User declarations
        __fastcall TFormEdtTrf3E(TComponent* Owner, VTApl *apl, VTTrafo3E *ptrafo3e);
        __fastcall ~TFormEdtTrf3E(void);
   void __fastcall OpcaoRetira(bool enabled);

private: //métodos
   bool    __fastcall CalculaZseqTrafo(double sbase_old, double sbase_new);
   void    __fastcall DefBarrasPriSecTer(void);
   void    __fastcall DefValoresDefaultTrafo(void);
   void    __fastcall ExibeCamposCondicionais(void);
   void    __fastcall ExibeDadosTrafo(void);
   void    __fastcall ExibeSecDefasagem(void);
   void    __fastcall ExibeTerDefasagem(void);
   void    __fastcall ExibeVnomTrafo(void);
   void    __fastcall ExibeZseqTrafo(void);
   void    __fastcall FormIniciaPosicao(void);
   void    __fastcall IniciaCBoxBarra(void);
   void    __fastcall IniciaCBoxRede(void);
   void    __fastcall IniciaParteFixaTela(void);
   bool    __fastcall LeDadosTrafo(void);
   bool    __fastcall LePriDefasagem(void);
   bool    __fastcall LeSecDefasagem(void);
   bool    __fastcall LeTerDefasagem(void);
   bool    __fastcall LeZseqTrafo(void);
   VTRede* __fastcall SelecionaRede(TList *lisREDE);
   bool    __fastcall ValidaDadosTrafo(void);
   bool    __fastcall ValidaTap(void);

private: //objetos externos
   VTApl     *apl;
   VTTrafo3E *trafo3e;

private: //dados locais
   double  sbase_100mva;
   TList   *lisTRF;
};

#endif
//---------------------------------------------------------------------------
//eof
