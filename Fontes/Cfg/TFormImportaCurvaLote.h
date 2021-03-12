// ---------------------------------------------------------------------------

#ifndef TFormImportaCurvaLoteH
#define TFormImportaCurvaLoteH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>
#include <VCLTee.Chart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.Series.hpp>
#include <Vcl.Menus.hpp>

class VTApl;
class TAplCfg;
class VTCurva;
class VTCurvas;
// ---------------------------------------------------------------------------
class TFormImportaCurvaLote : public TForm
{
__published: // IDE-managed Components

	TToolBar *ToolBar1;
	TToolButton *butConfirmar;
	TImageList *ImageList1;
	TToolButton *butSelecionar;
	TActionList *ActionList1;
	TAction *ActionFecha;
	TAction *ActionSeleciona;
	TAction *ActionConfirma;
	TAction *ActionAjuda;
	TToolButton *butFechar;
	TToolButton *butAjuda;
	TGroupBox *GroupBoxArquivo;
	TEdit *DiretorioCurvas;
	TGroupBox *GroupBoxPreferencias;
	TCheckBox *ChRedeFut;
	TAction *ActionInsereRede;
	TToolBar *ToolBar2;
	TToolButton *butIncluirArquivos;
	TAction *ActionExcluirRede;
	TToolButton *butExcluirArquivos;
	TGroupBox *GroupBoxRede;
	TListBox *LBoxRedes;
	TSaveDialog *SaveDialog;
	TOpenDialog *OpenDialog;

	void __fastcall ActionFechaExecute(TObject *Sender);
	void __fastcall ActionSelecionaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall ActionAjudaExecute(TObject *Sender);
	void __fastcall AtualizaCurvas(void);
	void __fastcall ExcluiArqRedeEmpresa(bool checked);
	void __fastcall IncluiArqRedeEmpresa(bool checked);
	void __fastcall SalvaCurvas(TList *lisCURVA);
	void __fastcall ActionInsereRedeExecute(TObject *Sender);
	void __fastcall ActionExcluirRedeExecute(TObject *Sender);

public: // User declarations
	__fastcall TFormImportaCurvaLote(TComponent* Owner, TAplCfg *aplcfg);
	__fastcall ~TFormImportaCurvaLote(void);


private: // Métodos
    void __fastcall AtualizaListaRedes(void);

private: // dados locais
	VTApl *apl;
	TAplCfg *aplcfg;
	VTCurvas *curvas;
	TStringList *slisREDESINC;

};


// ---------------------------------------------------------------------------
//extern PACKAGE TFormImportaCurvaLote *FormImportaCurvaLote;
// ---------------------------------------------------------------------------
#endif
