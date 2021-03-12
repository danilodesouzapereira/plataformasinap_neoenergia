//---------------------------------------------------------------------------
#ifndef TFormMT_DCH
#define TFormMT_DCH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <CheckLst.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTTema_DC;
class TFormRel_DC;

//---------------------------------------------------------------------------
class TFormMT_DC : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *butFecha;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionFecha;
   TGroupBox *GroupBox1;
   TBitBtn *ButSelAll;
   TBitBtn *ButSelNone;
   TAction *ActionSelAll;
   TAction *ActionSelNone;
   TColorDialog *ColorDialog;
   TGroupBox *GroupBox2;
   TComboBox *CBoxPatamar;
   TTimer *Timer;
   TBitBtn *ButTimer;
   TAction *ActionAnimacao;
   TGroupBox *GroupBox3;
   TCheckListBox *CLBoxClasse;
   TComboBox *CBoxAno;
   TListView *LViewRede;
   TAction *ActionCalculo;
   TGroupBox *GroupBox4;
   TCheckBox *CheckBoxAno;
   TCheckBox *CheckBoxPatamar;
   TPanel *PanelColor;
   TComboBox *CBoxArea;
   TLabel *Label1;
   TToolButton *ToolButton1;
   TAction *ActionRelatorio;
   TRadioGroup *RadioGroupGrafico;
   TGroupBox *GBoxRedeVisible;
   TCheckBox *CheckBoxRedeVisible;
   TAction *ActionRemontaMapaPadrao;
   TAction *ActionRemontaMapaDegrade;
	TCheckBox *CheckBoxTransparente;
	TAction *ActionRemontaPorDemandaMax;
   TPanel *Panel2;
   TGroupBox *GroupBox5;
   TPanel *PanelAIS;
   TToolButton *ToolButton2;
   TAction *ActionHelp;
   TToolButton *ToolButton3;
   TPanel *Panel1;
   TBitBtn *BitBtn1;
   TBitBtn *BitBtn2;
   TAction *ActionSelAllClasse;
   TAction *ActionSelNoneClasse;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall ActionSelAllExecute(TObject *Sender);
   void __fastcall ActionSelNoneExecute(TObject *Sender);
   void __fastcall ActionAnimacaoExecute(TObject *Sender);
   void __fastcall TimerTimer(TObject *Sender);
	void __fastcall ActionCalculoExecute(TObject *Sender);
   void __fastcall LViewColumnClick(TObject *Sender, TListColumn *Column);
   void __fastcall LViewCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
   void __fastcall LViewRedeClick(TObject *Sender);
   void __fastcall PanelColorClick(TObject *Sender);
   void __fastcall CBoxAreaChange(TObject *Sender);
   void __fastcall ActionRelatorioExecute(TObject *Sender);
   void __fastcall RadioGroupGraficoClick(TObject *Sender);
   void __fastcall CheckBoxRedeVisibleClick(TObject *Sender);
   void __fastcall ActionRemontaMapaPadraoExecute(TObject *Sender);
   void __fastcall ActionRemontaMapaDegradeExecute(TObject *Sender);
	void __fastcall CheckBoxTransparenteClick(TObject *Sender);
	void __fastcall ActionRemontaPorDemandaMaxExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionSelAllClasseExecute(TObject *Sender);
   void __fastcall ActionSelNoneClasseExecute(TObject *Sender);

public:      // User declarations
        __fastcall  TFormMT_DC(TComponent* Owner, VTApl *apl_owner, TWinControl *parent, TAction *ActionOnClose);
        __fastcall ~TFormMT_DC(void);
   void __fastcall  Atualiza(void);

private:   //métodos
   void __fastcall CalculaDensidadeCargaDefault(void);
	void __fastcall CancelaMapaTematicoAtual(void);
	void __fastcall CarregaCBoxRede(void);
	void __fastcall CBoxAnoInicia(void);
	void __fastcall CBoxAreaInicia(void);
   void __fastcall CBoxPatamarInicia(void);
   void __fastcall CLBoxClasseInicia(void);
   void __fastcall ExibeRedes(bool visible);
   void __fastcall FormIniciaPosicao(void);
	void __fastcall HabilitaMapaTematico(bool enabled);
	int  __fastcall IndicePatamarSelecionado(void);	
   void __fastcall LViewRedeInicia(void);
	void __fastcall RecalculaDensidadeCarga(void);
	void __fastcall SalvaCBoxRede(void);
   void __fastcall SelecionaModoGrafico(int modo);

private:   //objetos externos
   TAction *ActionOnClose;
   VTApl   *apl;

private:   //dados locais
   TFormRel_DC *FormRel_DC;
   VTTema_DC   *tema_DC;
   TList       *lisEQP;
	TList       *lisCLASSE;
	struct   {
            int ano_mercado;
            int modo_grafico;
				}original;
   struct   {//estrutura p/ controle da ordenação do LView
            TListColumn *Column;
            bool        OrdemCrescente;
            }LViewSort;				
};

//---------------------------------------------------------------------------
#endif
//eof

