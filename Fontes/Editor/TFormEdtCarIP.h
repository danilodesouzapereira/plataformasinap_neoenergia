//---------------------------------------------------------------------------
#ifndef TFormEdtCarIPH
#define TFormEdtCarIPH

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
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTCarga;
class VTApl;
class VTGeral;
//class TFormEdtPlan;
class VTLegenda;
class VTPatamares;
class VTVetval;

//---------------------------------------------------------------------------
class TFormEdtCarIP : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl;
	TTabSheet *TabSheetCarga;
   TGroupBox *GroupBox1;
   TLabel *LabelCodigo;
   TEdit *EditCodigo;
   TTabSheet *tsPlaneja;
   TLabel *LabelQCap;
   TEdit *EditPot;
   TComboBox *CBoxCurva;
   TLabel *Label3;
   TTabSheet *TabSheetCurva;
   TChart *chart;
   TFastLineSeries *Series1;
   TToolBar *ToolBar2;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TImageList *ImageList1;
   TLabel *Label1;
   TLabel *Label5;
   TComboBox *CBoxMB;
   TComboBox *CBoxFase;
   TLabel *Label6;
   TComboBox *CBoxModCarga;
   TEdit *EditFatPot;
	TLabel *Label2;
	TEdit *EditNumLamp;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TPanel *Panel5;
   TPanel *Panel6;
   TPanel *Panel7;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall PageControlChange(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall CBoxMBChange(TObject *Sender);
	void __fastcall PageControlChanging(TObject *Sender, bool &AllowChange);

public:		// métodos
        __fastcall  TFormEdtCarIP(TComponent* Owner, VTApl *apl, VTCarga *carga);
        __fastcall ~TFormEdtCarIP(void);
   void __fastcall  OpcaoRetira(bool enabled);

private:
   void     __fastcall DefValoresDefaultCarga(void);
   void     __fastcall ExibeCurva(void);
   void     __fastcall ExibeDadosCarga(void);
   VTCarga* __fastcall ExisteCargaIP(TList *lisEQP);
   void     __fastcall FormIniciaPosicao(void);
   void     __fastcall IniciaCBoxCurva(void);
   void     __fastcall IniciaCBoxFase(void);
   void     __fastcall IniciaParteFixaTela(void);
   bool     __fastcall LeDadosCarga(void);
   bool     __fastcall LeFases(void);
   bool     __fastcall LeValorModCarga(void);

private: //objetos externos
   VTApl   *apl;
   VTCarga *carga;

private: //atributos locais
   //TFormEdtPlan *FormEdtPlan;
   VTLegenda   *legenda;
   VTPatamares *patamares;
   TList       *lisVETVAL;
   TList       *lisEQP;
};

#endif
//---------------------------------------------------------------------------
//eof
