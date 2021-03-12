//---------------------------------------------------------------------------
#ifndef TFormMT_SinalH
#define TFormMT_SinalH

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
#include <VCLTee.TeCanvas.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTTema_Sinal;
//class TFormRel_Sinal;
class TFormLimites_Sinal;
class VTProgresso;

//---------------------------------------------------------------------------
class TFormMT_Sinal : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *butFecha;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionFecha;
   TAction *ActionSelAll;
   TAction *ActionSelNone;
   TColorDialog *ColorDialog;
   TAction *ActionAnimacao;
   TAction *ActionCalculo;
   TPanel *PanelColor;
   TAction *ActionRelatorio;
   TGroupBox *GBoxRedeVisible;
   TCheckBox *CheckBoxRedeVisible;
   TAction *ActionRemontaMapaDegrade;
   TCheckBox *cboxAutoRange;
   TAction *ActionRemontaPorSinal;
   TToolButton *ToolButton2;
   TAction *ActionHelp;
   TToolButton *ToolButton3;
   TAction *ActionSelAllClasse;
   TAction *ActionSelNoneClasse;
   TToolButton *ToolButton4;
   TOpenDialog *OpenDialog1;
   TGroupBox *gbRanges;
   TListView *LView;
   TPanel *Panel1;
   TButtonColor *ButCorConsMenor;
   TButtonColor *ButCorConsMaior;
   TLabel *LabelSignalLevel;
   TToolButton *tbEditRange;
   TAction *ActionEditaFaixas;
   TPanel *Panel2;
   TPanel *Panel3;
   TLabel *Label2;
   TComboBox *CBoxArea;
   TAction *ActionRemontaMapaPadrao;
   TPanel *panelData;
   TLabel *Label3;
   TComboBox *cboxData;
   TGroupBox *GroupBox1;
   TPanel *panelLatOff;
   TPanel *panelLonOff;
   TLabel *labelLatOff;
   TLabel *Label1;
   TEdit *editLatOff;
   TEdit *editLonOff;
	void __fastcall ActionCalculoExecute(TObject *Sender);
	//void __fastcall ActionRemontaPorSinalExecute(TObject *Sender);
	void __fastcall cboxAutoRangeClick(TObject *Sender);
   //void __fastcall ActionRelatorioExecute(TObject *Sender);
   void __fastcall ActionEditaFaixasExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionImportarExecute(TObject *Sender);
   void __fastcall ActionRelatorioExecute(TObject *Sender);
   void __fastcall ActionRemontaMapaDegradeExecute(TObject *Sender);
   void __fastcall ActionRemontaMapaPadraoExecute(TObject *Sender);
   void __fastcall ButCorConsMaiorClick(TObject *Sender);
   void __fastcall ButCorConsMenorClick(TObject *Sender);
   void __fastcall CBoxAreaChange(TObject *Sender);
   void __fastcall cboxDataChange(TObject *Sender);
   //void __fastcall ActionRemontaMapaPadraoExecute(TObject *Sender);
   void __fastcall CheckBoxRedeVisibleClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall LViewColumnClick(TObject *Sender, TListColumn *Column);
   void __fastcall LViewCompare(TObject *Sender, TListItem *Item1, TListItem *Item2, int Data, int &Compare);
//   void __fastcall LViewCustomDraw(TCustomListView *Sender, const TRect &ARect, bool &DefaultDraw);
//   void __fastcall PanelColorClick(TObject *Sender);
   //void __fastcall RadioGroupGraficoClick(TObject *Sender);
   void __fastcall LViewAdvancedCustomDrawSubItem(TCustomListView *Sender, TListItem *Item,
          int SubItem, TCustomDrawState State, TCustomDrawStage Stage, bool &DefaultDraw);
   void __fastcall LViewDblClick(TObject *Sender);
   void __fastcall panelLatOffClick(TObject *Sender);
   void __fastcall panelLonOffClick(TObject *Sender);
   void __fastcall editLatOffClick(TObject *Sender);
   void __fastcall editLonOffClick(TObject *Sender);
   void __fastcall editLatOffExit(TObject *Sender);
   void __fastcall editLonOffExit(TObject *Sender);
   void __fastcall editLonOffKeyPress(TObject *Sender, System::WideChar &Key);
   void __fastcall editLatOffKeyPress(TObject *Sender, System::WideChar &Key);





public:      // User declarations
        __fastcall  TFormMT_Sinal(TComponent* Owner, VTApl *apl_owner, TWinControl *parent, TAction *ActionOnClose);
        __fastcall ~TFormMT_Sinal(void);
   void __fastcall  Atualiza(void);

private:   //métodos
   void __fastcall AtualizaDegrade(void);
   void __fastcall AlteraOffset(void);
   void __fastcall AutoRange(bool enabled);
   void __fastcall CalculaDensidadeMTSinal(void);
   void __fastcall CalculaDimMinCelula(void);
	void __fastcall CancelaMapaTematicoAtual(void);
	void __fastcall CBoxAreaInicia(void);
   void __fastcall ExibeRedes(bool visible);
   int  __fastcall ExtraiCampos(AnsiString linha, AnsiString separadores);
   void __fastcall FormIniciaPosicao(void);
	void __fastcall HabilitaMapaTematico(bool enabled);
   void __fastcall HabilitaForm(bool enabled);
   void __fastcall ImportaTxt(AnsiString filename);
   void __fastcall LViewIniciaValor(void);
   void __fastcall OffsetSegundoToGMS(double offset_s, int &graus, int &minutos, double &segundos);
   bool __fastcall VerificaOffset(void);
   void __fastcall SelecionaModoGrafico(int modo);

private:   //objetos externos
   TAction *ActionOnClose;
   VTApl   *apl;

private:   //dados locais
   //TFormRel_Sinal *FormRel_Sinal;
   TFormLimites_Sinal *FormLimites_Sinal;
   VTTema_Sinal   *tema_Sinal;
   TStringList    *lines;
   TStringList    *campos;
   TList          *listSTR;
   TList          *listData;
   double         resolucao;
   VTProgresso    *progresso;
   AnsiString     tipoDado;
   double         latOff, lonOff;
   double         latOff_anterior, lonOff_anterior;
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

