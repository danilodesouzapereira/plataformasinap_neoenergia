//---------------------------------------------------------------------------
#ifndef TFormEdtTrfMonoH
#define TFormEdtTrfMonoH

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
#include "..\Rede\VTLigacao.h"
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTGeral;
class VTBarra;
class VTTrafo;
class VTRede;
//class TFormEdtPlan;

//---------------------------------------------------------------------------
class TFormEdtTrfMono : public TForm
{
__published:   // IDE-managed Components
   TPageControl *pgCtrl;
   TTabSheet *TabSheetTrafo;
   TTabSheet *TabSheetPriSec;
   TGroupBox *gboxCodigo;
   TLabel *Label1;
   TEdit *EditCodigo;
   TGroupBox *gboxPrimario;
   TComboBox *CBoxBarPri;
   TEdit *EditVnomPri;
   TEdit *EditTapPri;
   TLabel *Label7;
   TLabel *LabelPriVnom;
   TLabel *LabelTapPri;
   TToolBar *ToolBar2;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TLabel *Label4;
   TComboBox *CBoxRede;
   TToolButton *ToolButton1;
   TToolButton *ButImagem;
   TAction *ActionSelFoto;
   TLabel *Label6;
   TComboBox *CBoxModelo;
   TGroupBox *GroupBox2;
   TLabel *Label10;
   TLabel *LabelSecVnom;
   TLabel *LabelTapSec;
   TComboBox *CBoxBarSec;
   TEdit *EditVnomSec;
   TEdit *EditTapSec;
   TGroupBox *gboxParmEletric;
   TLabel *Label5;
   TEdit *EditPerdaFe;
   TEdit *EditSnom;
   TLabel *LabelSnom;
   TGroupBox *GroupBox1;
   TLabel *Label9;
   TLabel *Label14;
   TEdit *EditR;
   TEdit *EditX;
   TGroupBox *GroupBox3;
   TLabel *Label8;
   TLabel *Label11;
   TComboBox *CBoxFasePri;
   TComboBox *CBoxFaseSec;
   TGroupBox *GBoxZatPri;
   TLabel *LabelSecRterra;
   TLabel *LabelSecXterra;
   TEdit *EditRatPri;
   TEdit *EditXatPri;
   TGroupBox *GBoxZatSec;
   TLabel *Label2;
   TLabel *Label3;
   TEdit *EditRatSec;
   TEdit *EditXatSec;
   TCheckBox *CheckAutoTrafo;
   TTabSheet *TabSheet1;
   TComboBox *CBoxLtcTipo;
   TGroupBox *GBoxLtcBar;
   TComboBox *CBoxLtcBar;
   TGroupBox *GBoxLtcVpu;
   TEdit *EditLtcVpu;
   TGroupBox *gboxTap;
	TUpDown *UpDownSec;
	TUpDown *UpDownPri;
	TLabel *LabelTap1;
	TLabel *LabelTap2;
	TLabel *LabelNumTap2;
	TLabel *LabelNumTap1;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TPanel *Panel5;
   TPanel *Panel6;
   TPanel *Panel11;
   TPanel *Panel12;
   TPanel *Panel13;
   TPanel *Panel7;
   TPanel *Panel8;
   TPanel *Panel9;
   TPanel *Panel10;
   TPanel *Panel14;
   TPanel *Panel15;
   TPanel *Panel16;
   TPanel *Panel17;
   TPanel *Panel18;
   TPanel *Panel19;
   TPanel *Panel20;
   TPanel *Panel21;
   TPanel *Panel22;
   TPanel *Panel23;
   TPanel *Panel24;
   TPanel *Panel25;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall CBoxBarPriChange(TObject *Sender);
   void __fastcall CBoxLtcTipoChange(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall ActionSelFotoExecute(TObject *Sender);
   void __fastcall CBoxModeloChange(TObject *Sender);
   void __fastcall CBoxFasePriChange(TObject *Sender);
   void __fastcall CBoxFaseSecChange(TObject *Sender);
   void __fastcall CBoxBarSecChange(TObject *Sender);
	void __fastcall UpDownPriClick(TObject *Sender, TUDBtnType Button);
	void __fastcall UpDownSecClick(TObject *Sender, TUDBtnType Button);

public:      // User declarations
        __fastcall  TFormEdtTrfMono(TComponent* Owner, VTApl *apl, VTTrafo *ptrafo, bool criar_copia);
        __fastcall  TFormEdtTrfMono(TComponent* Owner, VTApl *apl, TList *lisEXT, bool criar_copia);
        __fastcall ~TFormEdtTrfMono(void);
   void __fastcall  OpcaoRetira(bool enabled);
   
private: //métodos
   void    __fastcall AtualizaDados(void);
   void    __fastcall DefineValoresDefault(void);
   void    __fastcall DefineVnomVtap(void);
   void    __fastcall DeterminaFasesDisponiveis(void);
   double  __fastcall DeterminaVnomFFouFN(int fase, double vnom_bar, double fat_mult);
   void    __fastcall ExibeCamposCondicionais(void);
   void    __fastcall ExibeDadosTrafo(void);
   void    __fastcall ExibeVnomVtap(void);
   void    __fastcall FormIniciaPosicao(void);
   void    __fastcall IniciaCBoxBarraPriSec(void);
   void    __fastcall IniciaCBoxBarraControlada(void);
   void    __fastcall IniciaCBoxFasePri(void);
   void    __fastcall IniciaCBoxFaseSec(void);
   void    __fastcall IniciaCBoxModelo(void);
   void    __fastcall IniciaCBoxRede(void);
   void    __fastcall IniciaParteFixaTela(void);
   bool    __fastcall LeDadosTrafo(void);
   VTRede* __fastcall SelecionaRede(TList *lisREDE);
   bool    __fastcall ValidaDadosTrafo(void);
   bool    __fastcall ValidaTap(void);

private: //dados externos
   VTApl   *apl;
   VTGeral *geral;

private: //dados locais
   bool     aviso_enabled;
   bool     criar_copia;
   int      fases_disp[NBAR_LIG];
   TList   *lisTRF;
   TList   *lisTMP;
   VTTrafo *clone;
};

#endif
//---------------------------------------------------------------------------
//eof

