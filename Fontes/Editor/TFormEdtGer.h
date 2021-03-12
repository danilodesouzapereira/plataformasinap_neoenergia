//---------------------------------------------------------------------------
#ifndef TformEdtGerH
#define TformEdtGerH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <System.Actions.hpp>
#include <Vcl.Dialogs.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTCurva;
class VTGerador;
class VTApl;
class TFormEditaCurva;
class TFormExibeCurva;

//---------------------------------------------------------------------------
class TFormEdtGer : public TForm
{
__published:   // IDE-managed Components
	TPageControl *PageControl;
   TTabSheet *TabSheetGerador;
   TGroupBox *gboxGeral;
   TLabel *Label1;
   TLabel *LabelGerVnom;
   TLabel *LabelGerSnom;
   TEdit *EditCodigo;
   TEdit *edtVnom;
   TEdit *edtSnom;
   TEdit *EditFatPotLim;
   TTabSheet *TabSheetCurva;
   TLabel *Label6;
   TComboBox *cboxLigacao;
   TTabSheet *tsImpedancia;
   TTabSheet *TabSheetGrafico;
   TGroupBox *GroupBox1;
   TStringGrid *sgZeq;
   TLabel *Label2;
   TComboBox *cboxTipoSup;
   TCheckBox *CheckBoxFatPotLim;
   TImageList *ImageList1;
   TToolBar *ToolBar2;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TLabel *Label3;
   TComboBox *CBoxFonte;
   TGroupBox *GBoxCurvaTipica;
   TComboBox *CBoxCurvaTipica;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TRadioGroup *RadioGroupTipoCurva;
   TAction *ActionImporta;
   TToolButton *ToolButton1;
   TOpenDialog *OpenDialog;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TPanel *Panel5;
   TPanel *Panel6;
   TPanel *Panel7;
   TPanel *Panel8;
   TPanel *Panel9;
	TPanel *PanelFases;
	TComboBox *CBoxFases;
	TGroupBox *GBoxFases;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall CBoxFonteChange(TObject *Sender);
   void __fastcall CBoxTipoSupChange(TObject *Sender);
   void __fastcall CheckBoxFatPotLimClick(TObject *Sender);
   void __fastcall PageControlChange(TObject *Sender);
   void __fastcall PageControlChanging(TObject *Sender, bool &AllowChange);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall CBoxCurvaTipicaChange(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall RadioGroupTipoCurvaClick(TObject *Sender);
	void __fastcall ActionImportaExecute(TObject *Sender);
	void __fastcall CBoxFasesChange(TObject *Sender);
	void __fastcall  CBoxLigacaoChange(TObject *Sender);

public:      // métodos
        __fastcall TFormEdtGer(TComponent* Owner, VTApl *apl, VTGerador *pger);
        __fastcall ~TFormEdtGer(void);
   void __fastcall OpcaoRetira(bool enabled);

private:
    void       __fastcall AtualizaForm(void);
   void       __fastcall CBoxCurvaTipicaInicia(void);
   VTCurva*   __fastcall CriaCurva(void);
   void       __fastcall ExibeDadosGerador(void);
   void       __fastcall ExibeDadosImpedancia(void);
   void       __fastcall ExibeDadosVPQ(void);
   void       __fastcall FormIniciaPosicao(void);
   bool       __fastcall SGridExistePeriodo(int nr);
   void       __fastcall IniciaDadosGeradorNovo(void);
   void       __fastcall IniciaParteFixaTela(void);
   bool       __fastcall LeDadosGerador(void);
   bool       __fastcall LeDadosImpedancia(void);
   bool       __fastcall LeDadosCurva(void);
	bool       __fastcall LeDadosPatamarFixo(void);
   bool       __fastcall LeDadosPatamarVariavel(void);
   bool       __fastcall SelecionaCurva(void);

private: //objetos externos
   VTApl        *apl;
   VTGerador    *pger;

private: //dados locais
   TFormEditaCurva *FormEditaCurva;
   TFormExibeCurva *FormExibeCurva;
   TList           *lisTMP;
   struct   {
            VTCurva *propria;
            }curva;
};

#endif
//---------------------------------------------------------------------------
//eof

