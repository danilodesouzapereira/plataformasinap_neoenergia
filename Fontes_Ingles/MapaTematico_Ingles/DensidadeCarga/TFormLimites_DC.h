//---------------------------------------------------------------------------
#ifndef TFormLimites_DCH
#define TFormLimites_DCH

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
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormLimites_DC : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TToolButton *ButFechar;
	TActionList *ActionList;
	TAction *ActionFecha;
	TImageList *ImageList;
	TToolButton *ButConfirmar;
   TStringGrid *StringGrid;
	TAction *ActionConfirma;
   TToolButton *ButAdic;
   TAction *ActionAdiciona;
   TAction *ActionRetira;
   TToolButton *ButRet;
   TToolButton *ToolButton1;
   TRadioGroup *RadioGroupUnidade;

	void __fastcall ActionFechaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionAdicionaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall RadioGroupUnidadeClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);

public:		// User declarations
	__fastcall  TFormLimites_DC(TComponent* Owner, VTApl *apl, TAction *ActionRemontaMapaPadrao);
	__fastcall ~TFormLimites_DC(void);

private:	// User declarations
	bool __fastcall EditaLimites(void);
   void __fastcall HabilitaInsereRetiraFaixa(void);
   bool __fastcall StringGridCellToDouble(int col, int row, double &valor);
//   void __fastcall StringGridInicia(void);
   void __fastcall StringGridIniciaMVA(void);
   void __fastcall StringGridIniciaKVA(void);
   bool __fastcall StringGridLeValores(bool so_validar);

private:   //objetos externos
	VTApl   *apl;
	TAction *ActionRemontaMapaPadrao;

private: //dados locais
   struct{
         int min;
         int max;
         int enabled;
         }num_faixa;
};

//---------------------------------------------------------------------------
#endif
//eof

