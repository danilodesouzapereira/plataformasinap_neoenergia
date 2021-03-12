//---------------------------------------------------------------------------

#ifndef TFormCfgGrafH
#define TFormCfgGrafH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class VTCfgGrafConfiab;
//---------------------------------------------------------------------------
class TFormCfgGraf : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TShape *Shape1;
	TShape *Shape2;
	TShape *Shape3;
	TEdit *dec1;
	TEdit *dec2;
	TRadioButton *butDec;
	TRadioButton *butFec;
	TEdit *fec1;
	TEdit *fec2;
	TRadioButton *butEnd;
	TEdit *end1;
	TEdit *end2;
	TRadioButton *butPot;
	TEdit *pot1;
	TEdit *pot2;
	TActionList *ActionList;
	TAction *ActionCancelar;
	TAction *ActionConfirmar;
	TImageList *ImageList;
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TToolButton *ToolButton3;
	TLabel *Label1;
	TShape *Shape4;
	TLabel *Label2;
	TShape *Shape5;
	TShape *Shape6;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TShape *Shape7;
	TLabel *Label7;
	void __fastcall ActionCancelarExecute(TObject *Sender);
	void __fastcall ActionConfirmarExecute(TObject *Sender);
public:		// User declarations
	__fastcall TFormCfgGraf(TComponent* Owner, VTCfgGrafConfiab *cfg);
	__fastcall ~TFormCfgGraf(void);

private:
	bool __fastcall LeDados(void);     	
	VTCfgGrafConfiab *cfg;


};
//---------------------------------------------------------------------------
#endif
