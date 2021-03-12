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
#include <System.Actions.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTCfgGraf;
//---------------------------------------------------------------------------
class TFormCfgGraf : public TForm
{
__published:	// IDE-managed Components
	TActionList *ActionList;
	TAction *ActionCancelar;
	TAction *ActionConfirmar;
	TImageList *ImageList;
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TToolButton *ToolButton3;
	TGroupBox *GroupBox2;
	TShape *Shape7;
	TShape *Shape8;
	TShape *Shape9;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TEdit *EditDRC;
	TEdit *EditDRP;
	TGroupBox *GroupBox3;
	TShape *Shape10;
	TShape *Shape11;
	TShape *Shape12;
	TLabel *Label17;
	TLabel *Label18;
	TLabel *Label16;
	TEdit *EditSnom;
	TEdit *EditSmed;
	TRadioGroup *RgView;
	TEdit *EditSadeq;
	TPanel *Panel1;
	void __fastcall ActionCancelarExecute(TObject *Sender);
	void __fastcall ActionConfirmarExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall EditSmedExit(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);

public:		// User declarations
	__fastcall TFormCfgGraf(TComponent* Owner, VTApl *apl);
	__fastcall ~TFormCfgGraf(void);

private:  //métodos
	bool __fastcall SalvaDados(void);
	bool __fastcall SalvaCfg(void);
	bool __fastcall ValidaDados(void);

private: //obj. internos
	bool 		 confirma;
	double 	 valDRC, valDRP, valSmed, valSnom;
	VTCfgGraf *cfg;

private: //obj. externo
	VTApl *apl;

};
//---------------------------------------------------------------------------
#endif
