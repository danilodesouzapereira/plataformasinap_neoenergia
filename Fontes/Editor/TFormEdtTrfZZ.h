//---------------------------------------------------------------------------
#ifndef TFormEdtTrfZZH
#define TFormEdtTrfZZH

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
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
//class TFormEdtPlan;
class VTApl;
class VTTrafoZZ;

//---------------------------------------------------------------------------
class TFormEdtTrfZZ : public TForm
{
__published:	// IDE-managed Components
   TPageControl *pgCtrl;
   TTabSheet *tsTrafoZZ;
   TGroupBox *gboxChv1;
   TTabSheet *tsPlaneja;
   TEdit *EditCodigo;
   TLabel *Label1;
   TLabel *LabelR0;
        TEdit *edtR0;
   TLabel *LabelX0;
        TEdit *edtX0;
   TLabel *LabelSnom;
   TEdit *edtSnom;
   TLabel *LabelVnom;
   TEdit *edtVnom;
   TToolBar *ToolBar2;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TImageList *ImageList1;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TPanel *Panel5;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);

public:		// métodos
        __fastcall TFormEdtTrfZZ(TComponent* Owner, VTApl *apl, VTTrafoZZ* ptrfZZ);
        __fastcall ~TFormEdtTrfZZ(void);
   void __fastcall OpcaoRetira(bool enabled);

private:    //métodos
   void __fastcall DefValoresDefaultTrafoZZ(void);
   void __fastcall ExibeDadosTrafoZZ(void);
   void __fastcall FormIniciaPosicao(void);
   void __fastcall IniciaParteFixaTela(void);
   bool __fastcall LeDadosTrafoZZ(void);

private: //objetos externos
   //TFormEdtPlan *FormEdtPlan;
   VTApl     *apl;
   VTTrafoZZ     *ptrfZZ;

private: //dados locais
   TList *lisEQP;
};

#endif
//---------------------------------------------------------------------------
//eof
