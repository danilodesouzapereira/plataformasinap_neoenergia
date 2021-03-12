//---------------------------------------------------------------------------

#ifndef TFormEditaCurvaCargaH
#define TFormEditaCurvaCargaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTCurva;
class TFormEditaCurva;
class TFormEditaCurvaCarga3F;
class TFormExibeCurva;
//---------------------------------------------------------------------------
class TFormEditaCurvaCarga : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl;
	TTabSheet *TabSheetEdicao;
	TToolBar *ToolBar2;
	TToolButton *butConfirma;
	TToolButton *butCancela;
	TImageList *ImageList1;
	TActionList *ActionList1;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TAction *ActionRetira;
	TAction *ActionImporta;
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall PageControlChange(TObject *Sender);

private:	// User declarations
	void 		__fastcall IniciaDadosCargaNova(void);
	VTCurva* __fastcall CriaCurva(void);

public:		// User declarations
				__fastcall TFormEditaCurvaCarga(TComponent* Owner, VTApl* apl, VTCurva* curva);
				__fastcall ~TFormEditaCurvaCarga(void);
	VTCurva* __fastcall Curva(void);

private:	//objetos externos
	VTApl       *apl;
	VTCurva		*curva_orig;

private:    //dados locais
   TFormEditaCurva *FormEditaCurva;
   TFormEditaCurvaCarga3F *FormEditaCurvaCarga3F;
	TFormExibeCurva *FormExibeCurva;
	struct   {
            VTCurva *propria;
				}curva;
};

//---------------------------------------------------------------------------
#endif
