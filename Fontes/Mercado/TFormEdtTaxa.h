//---------------------------------------------------------------------------
#ifndef TFormEdtTaxaH
#define TFormEdtTaxaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Grids.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTCrescimento;
class VTMercado;
class VTSegmento;

//---------------------------------------------------------------------------
class TFormEdtTaxa : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TStringGrid *SGrid;
   TToolButton *ToolButton1;
   TToolButton *ButDefinneTaxa;
   TAction *ActionDefineTaxa;
   TEdit *EditTaxa;
   TComboBox *CBoxCrescimento;
   TComboBox *CBoxAno;
   TToolButton *ToolButton2;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionDefineTaxaExecute(TObject *Sender);

public:      // User declarations
	__fastcall TFormEdtTaxa(TComponent* Owner, VTMercado *mercado, VTSegmento *segmento);
   __fastcall ~TFormEdtTaxa(void);

private:   //métodos
   void __fastcall CBoxAnoInicia(void);
   void __fastcall CBoxCrescimentoInicia(void);
   void __fastcall DefineTaxa(VTCrescimento *crescimento, int ano_ini, int ano_fim, double taxa_perc);
   bool __fastcall LeDados(void);
   bool __fastcall LeTaxa(double &taxa);
   void __fastcall SGridInicia(void);
   bool __fastcall Validadados(void);

private:   //objetos externos
	VTMercado  *mercado;
	VTSegmento *segmento;
};

//---------------------------------------------------------------------------
#endif
//eof
