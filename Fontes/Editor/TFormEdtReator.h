//---------------------------------------------------------------------------
#ifndef TFormEdtReatorH
#define TFormEdtReatorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
//class TFormEdtPlan;
class VTApl;
class VTLegenda;
class VTPatamares;
class VTReator;

//---------------------------------------------------------------------------
class TFormEdtReator : public TForm
{
__published:	// IDE-managed Components
   TPageControl *pgCtrl;
   TTabSheet *tsPlaneja;
   TTabSheet *tsReator;
   TGroupBox *GroupBox1;
   TEdit *EditCodigo;
   TLabel *Label1;
   TLabel *LabelSnom;
   TEdit *edtSnom;
   TLabel *LabelVnom;
   TEdit *edtVnom;
   TLabel *LabelLigPri;
   TComboBox *cboxLigacao;
   TToolBar *ToolBar2;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TLabel *Label3;
   TComboBox *CBoxCurva;
   TTabSheet *TabSheetCurva;
   TChart *chart;
   TFastLineSeries *Series1;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TPanel *Panel5;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall pgCtrlChanging(TObject *Sender, bool &AllowChange);
   void __fastcall pgCtrlChange(TObject *Sender);

public:		// métodos
        __fastcall TFormEdtReator(TComponent* Owner, VTApl *apl, VTReator* reator);
        __fastcall ~TFormEdtReator(void);
   void __fastcall OpcaoRetira(bool enabled);

private:    //métodos
   void __fastcall DefValoresDefaultReator(void);
   void __fastcall ExibeCurva(void);
   void __fastcall ExibeDadosReator(void);
   void __fastcall FormIniciaPosicao(void);
   void __fastcall IniciaCBoxCurva(void);
   void __fastcall IniciaParteFixaTela(void);
   bool __fastcall LeDadosReator(void);

private: //objetos externos
   VTApl    *apl;
   VTReator *reator;

private:    // atributos
   //TFormEdtPlan *FormEdtPlan;
   VTLegenda   *legenda;
   VTPatamares *patamares;
   TList       *lisVETVAL;
   TList       *lisEQP;
};

#endif
//---------------------------------------------------------------------------
//eof
