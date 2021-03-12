//---------------------------------------------------------------------------
#ifndef TFormEdtSup3H
#define TFormEdtSup3H

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
#include <Dialogs.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTSuprimento;
class VTApl;
class TFormEditaCurva3F;
class TFormExibeCurva;
class VTPatamares;
//---------------------------------------------------------------------------
class TFormEdtSup3 : public TForm
{
__published:	// IDE-managed Components
   TPageControl *PageControl;
   TTabSheet *TabSheetEquivalente;
   TTabSheet *TabSheetSuprimento;
   TGroupBox *gboxZeq;
   TGroupBox *GroupBox4;
   TEdit *EditCodigo;
   TGroupBox *GroupBox5;
   TGroupBox *gboxSupSmax;
   TComboBox *cboxTipoSup;
   TEdit *edtSmax;
   TRadioGroup *rgImpedancia;
   TGroupBox *gboxCurto;
   TStringGrid *sgCurto;
	TTabSheet *TabSheetGrafico;
   TTabSheet *TabSheetCurva;
   TGroupBox *GBoxVnom;
   TEdit *edtVnom;
	TImageList *ImageList1;
	TActionList *ActionList1;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TAction *ActionRetira;
	TToolBar *ToolBar2;
	TToolButton *butConfirma;
	TToolButton *butCancela;
	TToolButton *butRetira;
   TRadioGroup *RadioGroupPcc_ft;
   TStringGrid *sgZeq;
   TOpenDialog *OpenDialog;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall cboxTipoSupChange(TObject *Sender);
   void __fastcall rgImpedanciaClick(TObject *Sender);
   void __fastcall PageControlChanging(TObject *Sender, bool &AllowChange);
	void __fastcall PageControlChange(TObject *Sender);
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall RadioGroupPcc_ftClick(TObject *Sender);


public:		// métodos
		  __fastcall  TFormEdtSup3(TComponent* Owner, VTApl *apl, VTSuprimento *psup, bool op_retira);
		  __fastcall ~TFormEdtSup3(void);

private:  //métodos
   bool          __fastcall ConvertePotCurtoToZeqv(void);
   bool          __fastcall ConverteZeqvToPotCurto(void);
   void          __fastcall ExibeDadosPotCC(void);
	void          __fastcall ExibeDadosPQVT(void);
	void          __fastcall ExibeDadosSup(void);
	void          __fastcall ExibeDadosZeq(void);
	VTSuprimento* __fastcall ExisteSuprimento(AnsiString codigo);
   void          __fastcall FormIniciaPosicao(void);
   void          __fastcall IniciaDadosSuprimentoNovo(void);
   void          __fastcall IniciaParteFixaTela(void);
   bool          __fastcall LeDadosCurva(void);
	bool          __fastcall LeDadosPotCC_CalculaZeqv(bool show_erro);
	bool          __fastcall LeDadosZeqv_CalculaPotCC(bool show_erro);
   bool          __fastcall LeDadosSup(void);
   bool          __fastcall ValidaZeqv(bool show_erro);
   bool          __fastcall ValidaPotCC(bool show_erro);

private: //objetos externos
   VTApl        *apl;
   VTSuprimento *psup;

private: 	// Atributos
	#define           Sbase_mva    100.
	TList             *lisTMP;
   VTPatamares       *patamares;
   TFormEditaCurva3F *FormEditaCurva3F;
   TFormExibeCurva   *FormExibeCurva;
};

#endif
//---------------------------------------------------------------------------
//eof

