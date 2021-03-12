//---------------------------------------------------------------------------

#ifndef TFormRelatorioH
#define TFormRelatorioH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;

//---------------------------------------------------------------------------
class TFormRelatorio : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
	TToolButton *ToolButton1;
	TImageList *ImageList;
	TActionList *ActionList1;
	TAction *ActionSalvar;
	TAction *ActionSair;
	TAction *ActionImprimir;
   TListView *LView;
	TToolButton *ToolButton4;
   TCheckBox *CheckBoxCargaAlterada;
   TToolButton *ButSair;
   TSaveDialog *SaveDialog;
   TPanel *Panel;
   TComboBox *CBoxPatamar;
   TLabel *Label1;
   TToolButton *ToolButton2;
	void __fastcall ActionSalvarExecute(TObject *Sender);
	void __fastcall ActionSairExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall CBoxPatamarChange(TObject *Sender);
   void __fastcall CheckBoxCargaAlteradaClick(TObject *Sender);
   void __fastcall LViewColumnClick(TObject *Sender, TListColumn *Column);
   void __fastcall LViewCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
   void __fastcall LViewClick(TObject *Sender);

public:		// User declarations
	     __fastcall  TFormRelatorio(TComponent* Owner, VTApl *apl);
	     __fastcall ~TFormRelatorio(void);

private:
   void     __fastcall CBoxPatamarInicia(void);
   VTCarga* __fastcall ExisteCarga(TList *lisCAR, int carga_id);
   void     __fastcall FormIniciaPosicao(void);
	void     __fastcall LViewInicia(void);
   void     __fastcall Moldura(TList *lisEQP=NULL);
   bool     __fastcall StrToAbsComplex(AnsiString text, double &valor);

private: //objetos externos
   VTApl *apl;

private: //dados locais
   struct{
         int    ColumnIndex;
         bool   OrdemCrescente;
         }ListViewSort;
   TList *lisEQP;
};

//---------------------------------------------------------------------------
#endif
