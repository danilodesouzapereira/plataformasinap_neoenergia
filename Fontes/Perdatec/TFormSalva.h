//---------------------------------------------------------------------------
#ifndef TFormSalvaH
#define TFormSalvaH
#include <ActnList.hpp>
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include <Actions.hpp>
//#include <ActnList.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormSalva : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar;
	TToolButton *butConfirma;
	TToolButton *butCancela;
	TActionList *ActionList;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TAction *ActionCriaBaseNova;
	TImageList *ImageList;
	TPanel *PanelData;
	TGroupBox *GroupBox1;
	TComboBox *CBoxData;
	TGroupBox *GroupBox2;
	TComboBox *CBoxBase;
	TBitBtn *BitBtn1;
	TCheckBox *CheckBoxDataExistente;
	TCheckBox *CheckBoxDataNova;
	TDateTimePicker *DateTimePicker;
   TSaveDialog *SaveDialog;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall ActionCriaBaseNovaExecute(TObject *Sender);
	void __fastcall DateTimePickerChange(TObject *Sender);
	void __fastcall CBoxBaseChange(TObject *Sender);
	void __fastcall CheckBoxDataExistenteClick(TObject *Sender);
	void __fastcall CheckBoxDataNovaClick(TObject *Sender);

public:		// User declarations
	__fastcall  TFormSalva(TComponent* Owner, VTApl *apl_owner);
	__fastcall ~TFormSalva(void);

private:	//métodos
	void __fastcall CBoxBaseInicia(AnsiString base_ref="Historico");
	void __fastcall CBoxDataInicia(void);
	bool __fastcall ExisteBaseSelecionada(void);
	bool __fastcall SalvaPerdasNaBase(void);

private:	//objetos externos
	VTApl *apl;

private:	//dados locais
	TList    *lisARV;
	TStrings *files;
};

//---------------------------------------------------------------------------
#endif
//eof

