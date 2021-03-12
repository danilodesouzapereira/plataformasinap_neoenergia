//---------------------------------------------------------------------------
#ifndef TFormEdtFiltroH
#define TFormEdtFiltroH

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

//---------------------------------------------------------------------------
//class TFormEdtPlan;
class VTFiltro;
class VTApl;

//---------------------------------------------------------------------------
class TFormEdtFiltro : public TForm
{
__published:	// IDE-managed Components
   TPageControl *pgCtrl;
   TTabSheet *tsPlaneja;
   TTabSheet *tsFiltro;
   TToolBar *ToolBar2;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
	TLabel *Label1;
   TEdit *EditCodigo;
	TEdit *edtR0;
	TEdit *edtX0;
	TLabel *LabelX0;
	TLabel *LabelR0;
	TEdit *edtVnom;
	TLabel *LabelVnom;
	TLabel *LabelSnom;
	TEdit *edtSnom;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TPanel *Panel5;
   TPanel *Panel6;
   TPanel *Panel7;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);

public:		// métodos
        __fastcall TFormEdtFiltro(TComponent* Owner, VTApl *apl, VTFiltro* filtro);
        __fastcall ~TFormEdtFiltro(void);
   void __fastcall OpcaoRetira(bool enabled);

private:    //métodos
   void __fastcall DefValoresDefaultFiltro(void);
   void __fastcall ExibeDadosFiltro(void);
   void __fastcall FormIniciaPosicao(void);
   void __fastcall IniciaParteFixaTela(void);
   bool __fastcall LeDadosFiltro(void);

private: //objetos externos
   //TFormEdtPlan *FormEdtPlan;
   VTApl     *apl;
   VTFiltro  *filtro;

private: //dados locais
   TList *lisEQP;
};

#endif
//---------------------------------------------------------------------------
//eof
