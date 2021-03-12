//---------------------------------------------------------------------------
#ifndef TFormAlocaLicencaH
#define TFormAlocaLicencaH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.EngExt.hpp>
#include <Data.Bind.ObjectScope.hpp>
#include <IPPeerClient.hpp>
#include <REST.Client.hpp>
#include <System.Bindings.Outputs.hpp>
#include <System.Rtti.hpp>
#include <Vcl.Bind.DBEngExt.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTGestaoLicenca;
class VTLicenca;
class TLicencaRequest;
class TLicencaBO;

//---------------------------------------------------------------------------
class TFormAlocaLicenca : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar2;
	TToolButton *butConfirma;
	TToolButton *butCancela;
	TGroupBox *GroupBox1;
	TEdit *EditLogin;
	TGroupBox *GroupBox2;
	TEdit *EditPassword;
	TImageList *ImageList1;
	TActionList *ActionList1;
	TAction *ActionCancela;
	TAction *ActionConfirma;
    TButton *butMostrar;
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall EditLoginKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EditPasswordKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall butMostrarMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall butMostrarMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

public:		// User declarations
	__fastcall TFormAlocaLicenca(TComponent *Owner, VTApl *apl_owner);
	__fastcall ~TFormAlocaLicenca(void);
	AnsiString __fastcall GetDominio(void);
	AnsiString __fastcall GetLogin(void);
	AnsiString __fastcall GetPassword(void);
//	bool __fastcall LiberaLicenca(VTLicenca *licenca, UnicodeString &diagnostico);

private:
	bool __fastcall MontaLicencaRequest(void);
	bool __fastcall ValidaCampos(void);

private:	// objeto externo
	VTApl *apl;

private:	// objeto local
	TLicencaBO *licencaBO;

};

//---------------------------------------------------------------------------
#endif
