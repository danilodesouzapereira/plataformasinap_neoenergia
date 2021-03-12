//---------------------------------------------------------------------------
#ifndef TFormCfgTensaoH
#define TFormCfgTensaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTMedicao;
class VTRes505;
class VTApl;

//---------------------------------------------------------------------------
class TFormCfgTensao : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *ToolButton3;
   TImageList *ImageList;
   TToolButton *ToolButton1;
   TActionList *ActionList;
   TAction *ActionCancelar;
   TAction *ActionConfirmar;
   TStatusBar *StatusBar1;
   TGroupBox *GroupBox1;
   TLabel *Label1;
   TLabel *Label7;
   TEdit *editVff;
   TEdit *editVfn;
   TPageControl *PageControl1;
   TTabSheet *TabSheet1;
   TTabSheet *TabSheet2;
   TGroupBox *GroupBox2;
   TLabel *Label2;
   TEdit *editAdeq1;
   TEdit *editAdeq2;
   TGroupBox *GroupBox3;
   TLabel *Label3;
   TLabel *Label6;
   TEdit *editCrit1;
   TEdit *editCrit2;
   TGroupBox *GroupBox4;
   TLabel *Label4;
   TLabel *Label5;
   TEdit *editPrecInf1;
   TEdit *editPrecSup1;
   TEdit *editPrecInf2;
   TEdit *editPrecSup2;
   TGroupBox *GroupBox5;
   TRadioButton *radiobutFN;
   TRadioButton *radiobutFF;
   TComboBox *cboxV1;
   TLabel *Label8;
   TLabel *Label9;
   TLabel *Label10;
   TComboBox *cboxV2;
   TLabel *Label11;
   TComboBox *cboxV3;
	TLabel *Label_adeq1;
	TLabel *Label_adeq2;
	TLabel *Label_crit1;
	TLabel *Label_crit2;
	TLabel *Label_prec_inf1;
	TLabel *Label_prec_inf2;
	TLabel *Label_prec_sup1;
	TLabel *Label_prec_sup2;
   void __fastcall ActionCancelarExecute(TObject *Sender);
   void __fastcall ActionConfirmarExecute(TObject *Sender);
   void __fastcall editAdeq1Exit(TObject *Sender);
   void __fastcall editAdeq2Exit(TObject *Sender);
   void __fastcall editCrit1Exit(TObject *Sender);
   void __fastcall editCrit2Exit(TObject *Sender);
   void __fastcall radiobutFNClick(TObject *Sender);
   void __fastcall radiobutFFClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);

public:      // User declarations
   __fastcall  TFormCfgTensao(TComponent* Owner, VTApl *apl);
	__fastcall ~TFormCfgTensao(void);

private:
   void __fastcall IniciaCboxMedicao(void);
   void __fastcall IniciaValores(void);
//	bool __fastcall LeValores(void);
	bool __fastcall SalvaDados(void);
	bool __fastcall SalvaRes(void);
	bool __fastcall ValidaDados(void);

private:   //objetos externos
	//VTMedicao *medicao;
   VTRes505  *res;
	VTApl     *apl;

private:  //internos
	double adeq1, adeq2, crit1, crit2;
	double vff_v, vfn_v;
	bool   confirma;

};
//---------------------------------------------------------------------------
#endif
