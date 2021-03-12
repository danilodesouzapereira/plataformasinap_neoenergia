//---------------------------------------------------------------------------
#ifndef TFormCfgRelTensaoH
#define TFormCfgRelTensaoH

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
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class VTApl;
class TNivelTensao;
class TRelatorioTensao;

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16082236
class TFormCfgRelTensao : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TImageList *ImageList1;
	TActionList *ActionList1;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TAction *ActionAlteraFaixas;
	TAction *ActionFecha;
	TPanel *Panel1;
	TPanel *Panel2;
	TRadioGroup *RGroupTensao;
	TRadioGroup *RGroupLimite;
	TGroupBox *GroupBox1;
	TComboBox *CBoxPatamar;
	TGroupBox *GroupBox2;
	TTabControl *tabControlNivelV;
	TPanel *pnlV1;
	TLabel *lblV1;
	TPanel *PanCorV1;
	TPanel *pnlV2;
	TLabel *lblV2;
	TPanel *PanCorV2;
	TPanel *pnlV3;
	TLabel *lblV3;
	TPanel *PanCorV3;
	TPanel *pnlV4;
	TLabel *lblV4;
	TPanel *PanCorV4;
	TPanel *pnlV5;
	TLabel *lblV5;
	TPanel *PanCorV5;
	TColorDialog *ColorDialog;
	TAction *ActionAlteraCor;
	void __fastcall ActionAlteraFaixasExecute(TObject *Sender);
	void __fastcall ActionFechaExecute(TObject *Sender);
	void __fastcall tabControlNivelVChange(TObject *Sender);
	void __fastcall ActionAlteraCorExecute(TObject *Sender);
	void __fastcall tabControlNivelVChanging(TObject *Sender, bool &AllowChange);
	void __fastcall FormShow(TObject *Sender);

public:		// User declarations
	__fastcall TFormCfgRelTensao(TComponent* Owner, VTApl *apl, TRelatorioTensao *relatorio_tensao);
	__fastcall ~TFormCfgRelTensao(void);

private:
    void __fastcall AtualizaFaixas(void);
    void __fastcall CBoxPatamarInicia(void);
	void __fastcall Inicia(void);
	void __fastcall TabControlInicia(void);

private: //dados externos
	VTApl *apl;
	TRelatorioTensao *relatorio_tensao;
    TNivelTensao *nivel_ativo;
};

//---------------------------------------------------------------------------
#endif
