//---------------------------------------------------------------------------

#ifndef TFormCfgEstudoH
#define TFormCfgEstudoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
//---------------------------------------------------------------------------
class VTApl;
//---------------------------------------------------------------------------
class TFormCfgEstudo : public TForm
{
__published:	// IDE-managed Components
	TImageList *ImageList;
	TActionList *ActionList;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TToolBar *ToolBar;
	TToolButton *ButConfirmar;
	TToolButton *ToolButton1;
	TToolButton *ButCancelar;
	TGroupBox *GBoxAutoTag;
	TCheckBox *checkBoxChavesManobrada;
	TCheckBox *checkBoxEnabled;
	TGroupBox *gboxTipos;
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall checkBoxEnabledClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormCfgEstudo(TComponent* Owner, VTApl *apl);
	__fastcall ~TFormCfgEstudo(void);

private:
	void __fastcall LeCfgAtual(void);
    void __fastcall HabilitaOpcoes(bool habilita);

private:
 VTApl *apl_;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCfgEstudo *FormCfgEstudo;
//---------------------------------------------------------------------------
#endif
