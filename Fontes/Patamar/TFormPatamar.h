//---------------------------------------------------------------------------
#ifndef TFormPatamarH
#define TFormPatamarH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <CheckLst.hpp>
#include <Grids.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTPatamar;

//---------------------------------------------------------------------------
class TFormPatamar : public TForm
{
__published:	// IDE-managed Components
   TRadioGroup *RGroupPatamares;
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionEnablePatamar;
   TAction *ActionDisablePatamar;
   TPageControl *PageControl;
   TTabSheet *TabSheetHoraCal;
   TTabSheet *TabSheetHabilita;
   TGroupBox *GroupBoxHabilitar;
   TCheckListBox *CLBoxPatamar;
   TToolBar *ToolBarPatamar;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TStringGrid *StringGridConfig;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall RGroupPatamaresClick(TObject *Sender);
   void __fastcall ActionEnablePatamarExecute(TObject *Sender);
   void __fastcall ActionDisablePatamarExecute(TObject *Sender);
   void __fastcall CLBoxPatamarClickCheck(TObject *Sender);

public:		// User declarations
   __fastcall TFormPatamar(TComponent* Owner, VTApl *apl);
   __fastcall ~TFormPatamar(void);

private:	//métodos
	bool __fastcall AtualizaPatamar(VTPatamar *patamar, TDateTime date_time_ini,TDateTime date_time_fim,TDateTime date_time_cal);
	void __fastcall CLBoxPatamarInicia(void);
	void __fastcall CriaPatamares(void);
	void __fastcall FormIniciaPosicao(void);
   void __fastcall LimpaTStringGrid(TStringGrid *StringGrid);
   void __fastcall RGroupPatamarInicia(void);
   bool __fastcall SalvaPatamares(void);
	bool __fastcall SalvaValores(void);
	bool __fastcall SalvaValoresInfoset(AnsiString nome_pat, TDateTime date_time_ini,TDateTime date_time_fim,TDateTime date_time_cal);
	void __fastcall StringGridConfigInicia(void);
	bool __fastcall ValidaHoraMinutoCalculo(VTPatamar *patamar, TDateTime date_time_cal);
	bool __fastcall ValidaValores(AnsiString *pat1, AnsiString *pat2);
	
private:	//objetos externos
   VTApl *apl;

private:	//dados locais
   TList *lisPAT;
};

//---------------------------------------------------------------------------
#endif
//eof

