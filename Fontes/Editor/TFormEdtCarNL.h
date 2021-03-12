//---------------------------------------------------------------------------

#ifndef TFormEdtCarNLH
#define TFormEdtCarNLH
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
#include <Vcl.Grids.hpp>
//inclde da carga e structures
#include "..\Rede\VTCargaNL.h"
//---------------------------------------------------------------------------
class VTApl;
//---------------------------------------------------------------------------
class TFormEdtCarNL : public TForm
{
__published:	// IDE-managed Components
	TImageList *ImageList1;
	TActionList *ActionList1;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TAction *ActionRetira;
	TToolBar *ToolBar2;
	TToolButton *butConfirma;
	TToolButton *butCancela;
	TToolButton *butRetira;
	TPanel *Panel1;
	TLabel *Label1;
	TEdit *edtCodigo;
	TPanel *Panel2;
	TLabel *Label2;
	TEdit *edtSa;
	TPanel *Panel3;
	TLabel *Label3;
	TEdit *edtFP;
	TPanel *Panel4;
	TLabel *Label4;
	TEdit *edtSc;
	TPanel *Panel5;
	TLabel *Label5;
	TEdit *edtSb;
	TLabel *Label6;
	TEdit *edtAngA;
	TLabel *Label7;
	TEdit *edtAngB;
	TLabel *Label8;
	TEdit *edtAngC;
	TGroupBox *GroupBox1;
	TStringGrid *sGridHarmonicas;
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TAction *ActionAddHarmonica;
	TAction *ActionRemoveHarmonica;
	TLabel *Label9;
	TComboBox *cboxNatureza;
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall ActionRetiraExecute(TObject *Sender);
	void __fastcall ActionAddHarmonicaExecute(TObject *Sender);
	void __fastcall ActionRemoveHarmonicaExecute(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormEdtCarNL(TComponent* Owner, VTApl *apl, VTCargaNL *pcargaAntiga, VTCargaNL *pcargaNova = NULL);

private:
	void     __fastcall ExibeDadosCarga(void);
	void 	 __fastcall IniciaCboxNatureza(void);
	void 	 __fastcall IniciasGridHarmonicas(void);
	void	 __fastcall InsereHarmonicaSGrid(strHRM *harmonica, int index);
	bool	 __fastcall IsDoubleValid(AnsiString numero);
	bool	 __fastcall IsIntValid(AnsiString numero);
	bool 	 __fastcall IsHarmonicaEmpty(int index);
	bool 	 __fastcall IsHarmonicaValid(int index);
	bool	 __fastcall IsDadosCargaValid(void);
	void	 __fastcall RemoveHarmonicaSGrid(int index);
	bool     __fastcall LeDadosCarga(void);

private:    //objetos externos
   VTApl   *apl;
   VTCargaNL *pcarga;
   VTCargaNL *pcargaNova;

private:    //dados locais
	struct strHRMSTRINGS
	{
		AnsiString ordem;
		AnsiString sa_mod_pu;
		AnsiString sa_ang_pu;
		AnsiString sb_mod_pu;
		AnsiString sb_ang_pu;
		AnsiString sc_mod_pu;
		AnsiString sc_ang_pu;
		strHRM *hamonica;
	};
	DynamicArray<strHRMSTRINGS> vetorHarmonicas;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEdtCarNL *FormEdtCarNL;
//---------------------------------------------------------------------------
#endif
