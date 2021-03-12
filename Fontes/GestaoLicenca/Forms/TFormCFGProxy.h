//---------------------------------------------------------------------------

#ifndef TFormCFGProxyH
#define TFormCFGProxyH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.ImageList.hpp>
//---------------------------------------------------------------------------
class TFormCFGProxy : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar2;
	TToolButton *butConfirma;
	TToolButton *butCancela;
	TImageList *ImageList1;
	TActionList *ActionList1;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TGroupBox *GroupBox1;
	TEdit *EditServidor;
	TGroupBox *GroupBox2;
	TEdit *EditPorta;
	TGroupBox *GroupBox3;
	TEdit *edtPW;
	TGroupBox *GroupBox4;
	TEdit *edtUser;
	TButton *butMostrar;
	void __fastcall butCancelaClick(TObject *Sender);
	void __fastcall butConfirmaClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall butMostrarMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall butMostrarMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
private:	// User declarations
public:		// User declarations
	__fastcall TFormCFGProxy(TComponent* Owner);
	__fastcall ~TFormCFGProxy(void);

	__property int 			Porta 		= {read=PD.porta};
	__property AnsiString 	Servidor 	= {read=PD.servidor};
	__property AnsiString 	Usuario		= {read=PD.usuario};
	__property AnsiString 	Password 	= {read=PD.password};


private:
	bool __fastcall ValidaServidor(void);
	bool __fastcall ValidaPorta(void);
	bool __fastcall ValidaUsuario(void);
	bool __fastcall ValidaPassword(void);

	struct
	{
		int porta;
		AnsiString servidor;
		AnsiString usuario;
		AnsiString password;
	}PD;

};
//---------------------------------------------------------------------------
extern PACKAGE TFormCFGProxy *FormCFGProxy;
//---------------------------------------------------------------------------
#endif
