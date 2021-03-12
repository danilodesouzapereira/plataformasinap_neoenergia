//---------------------------------------------------------------------------
#ifndef TFormEdtCapsH
#define TFormEdtCapsH

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
class VTApl;
class VTCapserie;
class VTRede;
//class TFormEdtPlan;

//---------------------------------------------------------------------------
class TFormEdtCaps : public TForm
{
__published:	// IDE-managed Components
   TPageControl *pgCtrl;
   TTabSheet *tsCapserie;
   TGroupBox *gboxChv1;
   TTabSheet *TabSheetPlaneja;
   TEdit *EditCodigo;
   TLabel *Label1;
   TLabel *lbReatancia;
   TEdit *EditX;
   TLabel *LabelCapsCapacidade;
   TEdit *EditSnom;
   TToolBar *ToolBar2;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TToolButton *ToolButton3;
   TActionList *ActionList2;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TImageList *ImageList1;
   TLabel *Label2;
   TEdit *EditR;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
	TLabel *Label4;
	TComboBox *cboxRede;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);

public:		// métodos
        __fastcall TFormEdtCaps(TComponent* Owner, VTApl *apl, VTCapserie* pcaps);
        __fastcall ~TFormEdtCaps(void);
   void __fastcall OpcaoRetira(bool enabled);

private:    //métodos
   void __fastcall DefineValoresDefault(void);
   void __fastcall ExibeDadosCapserie(void);
   void __fastcall FormIniciaPosicao(void);
   void __fastcall IniciaCBoxRede(void);
   void __fastcall IniciaParteFixaTela(void);
   bool __fastcall LeDadosCapserie(void);
   VTRede* __fastcall SelecionaRede(TList *lisREDE);

private: //objetos externos
   VTApl      *apl;
   VTCapserie *pcaps;

private: //dados locais
   //TFormEdtPlan *FormEdtPlan;
   TList       *lisEQP;
};

#endif
//---------------------------------------------------------------------------
//eof
