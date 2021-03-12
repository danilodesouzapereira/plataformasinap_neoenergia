//---------------------------------------------------------------------------

#ifndef TFormCfgLimVH
#define TFormCfgLimVH
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
//---------------------------------------------------------------------------
//#include "..\Rede\VTTipoRede.h"
//#include "..\Geral\VTGeral.h"
//---------------------------------------------------------------------------
#define textMENOR "menor do que"
#define textMENORIGUAL "menor e igual a"
#define textMAIOR "maior do que"
#define textMAIORIGUAL "maior e igual a"
//---------------------------------------------------------------------------
class VTApl;
class VTGeral;
//---------------------------------------------------------------------------
class TFormCfgLimV : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TToolButton *butInclui;
	TToolButton *butSair;
	TToolButton *ToolButton1;
	TActionList *ActionList1;
	TImageList *ImageList;
	TAction *ActionConfirmar;
	TAction *ActionDefault;
	TAction *ActionCancelar;
	TToolButton *ToolButton2;
	TGroupBox *gboxLimBT;
	TComboBox *cboxLimInfBT;
	TEdit *edtLimInfBT;
	TComboBox *cboxLimSupBT;
	TEdit *edtLimSupBT;
	TPanel *pnlBTinf;
	TPanel *pnlBTsup;
	TLabel *Label1;
	TGroupBox *gboxLimAT;
	TLabel *Label2;
	TPanel *Panel1;
	TComboBox *cboxLimInfAT;
	TEdit *edtLimInfAT;
	TPanel *Panel2;
	TComboBox *cboxLimSupAT;
	TEdit *edtLimSupAT;
	TGroupBox *gboxLimMT;
	TLabel *Label3;
	TPanel *Panel3;
	TComboBox *cboxLimInfMT;
	TEdit *edtLimInfMT;
	TPanel *Panel4;
	TComboBox *cboxLimSupMT;
	TEdit *edtLimSupMT;
	void __fastcall ActionDefaultExecute(TObject *Sender);
	void __fastcall cboxLimSupBTChange(TObject *Sender);
	void __fastcall cboxLimInfMTChange(TObject *Sender);
	void __fastcall cboxLimSupMTChange(TObject *Sender);
	void __fastcall cboxLimInfATChange(TObject *Sender);
	void __fastcall edtLimSupBTChange(TObject *Sender);
	void __fastcall edtLimInfMTChange(TObject *Sender);
	void __fastcall edtLimSupMTChange(TObject *Sender);
	void __fastcall edtLimInfATChange(TObject *Sender);
	void __fastcall ActionCancelarExecute(TObject *Sender);
	void __fastcall ActionConfirmarExecute(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall  TFormCfgLimV(TComponent* Owner, VTApl *apl_owner);
	__fastcall ~TFormCfgLimV(void);

private:
	void __fastcall AjustaCampoIrmao(TComboBox *combo);
	void __fastcall AjustaCampoIrmao(TEdit *edit);
	bool __fastcall ConvertComboToBoolean(TComboBox *combo);
	void __fastcall ConvertBooleanToCombo(bool booleano, TComboBox *combo);
	void __fastcall IniciaComboBox(void);
	void __fastcall IniciaComboBoxLimSup(TComboBox *combo);
	void __fastcall IniciaComboBoxLimInf(TComboBox *combo);
	void __fastcall LeGeral(void);
	bool __fastcall ValidaValores(void);
	bool __fastcall ValidaValoresDouble(TEdit *edit);
	bool __fastcall VerificaCoerenciaLimites(void);
	void __fastcall SalvaGeral(void);

private: //dados externos
	VTGeral *geral;
	VTApl *apl;

};
//---------------------------------------------------------------------------
extern PACKAGE TFormCfgLimV *FormCfgLimV;
//---------------------------------------------------------------------------
#endif
