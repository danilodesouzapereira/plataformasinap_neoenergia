//---------------------------------------------------------------------------

#ifndef TFormSelMedH
#define TFormSelMedH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include "EstruturaMed.h"
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TMedidores;
class VTApl;
//---------------------------------------------------------------------------
class TFormSelMed : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TComboBox *cbMedidor;
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TLabel *Label1;
	TEdit *edRelTP;
	TLabel *Label2;
	TEdit *edVcontratada;
	TToolBar *ToolBar1;
	TToolButton *tbutSelArqMed;
	TToolButton *tbutCancela;
	TToolButton *tbutNovoMed;
	TImageList *ImageList;
	TActionList *ActionList1;
	TAction *ActionCancel;
	TAction *ActionSelArq;
	TAction *ActionNewMeter;
	TToolButton *ToolButton1;
	TOpenDialog *OpenDialogMed;
	TRadioGroup *RGroupAng;
	TGroupBox *GroupBox2;
	void __fastcall ActionCancelExecute(TObject *Sender);
	void __fastcall ActionSelArqExecute(TObject *Sender);
	void __fastcall ActionNewMeterExecute(TObject *Sender);

private:	// User declarations
	void __fastcall IniciaForm(void);
	void __fastcall InsereAngDefasagem(void);

private:	//objetos externos
	VTApl *apl;

private: //dados
	TComponent 	*owner;
	TStringList *linhas;
	TMedidores 	*perfilMedidores;
	strMEDIDOR 	*medidorSelected;
	double 		relTP, vContratada;

public:		// User declarations
					__fastcall TFormSelMed(TComponent* Owner, VTApl *apl_owner);
					__fastcall ~TFormSelMed(void);
	AnsiString  __fastcall GetFilepath(void);
	strMEDIDOR* __fastcall GetMedidor(void);
	double 		__fastcall GetRelTP(void);
	double 		__fastcall GetVContratada(void);

public:
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSelMed *FormSelMed;
//---------------------------------------------------------------------------
#endif
