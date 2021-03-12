//---------------------------------------------------------------------------
#ifndef TFormEdtCapH
#define TFormEdtCapH

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
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTCapacitor;
class VTApl;
class VTCurva;
class VTGeral;
class VTLegenda;
class VTPatamares;
class VTVetval;
class TFormEditaCurva;
class TFormExibeCurva;

//---------------------------------------------------------------------------
class TFormEdtCap : public TForm
{
__published:	// IDE-managed Components
   TPageControl *pgCtrl;
	TTabSheet *tsCapacitor;
   TLabel *LabelCodigo;
   TLabel *LabelCapVnom;
   TEdit *EditCodigo;
   TEdit *EditVnom;
   TLabel *LabelQCap;
   TEdit *EditQcap;
	TTabSheet *TabSheetEdicao;
   TToolBar *ToolBar2;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TImageList *ImageList1;
   TLabel *LabelLigPri;
   TComboBox *cboxLigacao;
   TGroupBox *GBoxTipica;
   TComboBox *CBoxCurva;
   TRadioGroup *RadioGroupTipoCurva;
	TTabSheet *TabSheetGrafico;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall CBoxCurvaChange(TObject *Sender);
   void __fastcall pgCtrlChange(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall pgCtrlChanging(TObject *Sender, bool &AllowChange);
   void __fastcall RadioGroupTipoCurvaClick(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);

public:		// métodos
        __fastcall TFormEdtCap(TComponent* Owner, VTApl *apl, VTCapacitor *pcap);
        __fastcall ~TFormEdtCap(void);
   void __fastcall OpcaoRetira(bool enabled);

private:
   VTCurva* __fastcall CriaCurva(void);
   void     __fastcall DefValoresDefaultCapacitor(void);
   void     __fastcall ExibeDadosCapacitor(void);
   VTCurva* __fastcall ExisteCurvaTipica(void);
   void     __fastcall FormIniciaPosicao(void);
   void     __fastcall IniciaCBoxCurva(void);
   void     __fastcall IniciaCurvas(void);
   bool     __fastcall LeDadosCapacitor(void);
   bool     __fastcall LePontosCurvaPropria(void);
   double   __fastcall ValorMaximoCurvaPropria(void);

private: //objetos externos
   VTApl       *apl;
   VTCapacitor *pcap;

private: //atributos locais
   TFormEditaCurva *FormEditaCurva;
   TFormExibeCurva *FormExibeCurva;
   VTLegenda       *legenda;
   VTPatamares     *patamares;
   TList           *lisVETVAL;
   TList           *lisEQP;
   struct   {
            VTCurva *propria;
            VTCurva *tipica;
            }curva;
};

#endif
//---------------------------------------------------------------------------
//eof
