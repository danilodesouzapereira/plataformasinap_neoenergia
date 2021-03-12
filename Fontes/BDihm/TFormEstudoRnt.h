//---------------------------------------------------------------------------
#ifndef TFormEstudoRntH
#define TFormEstudoRntH

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
#include <FileCtrl.hpp>
#include <Menus.hpp>
#include <System.Actions.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAbre;
class VTEstudo;

//---------------------------------------------------------------------------
class TFormEstudoRnt : public TForm
{
__published:   // IDE-managed Components
	TActionList *ActionList;
	TAction *ActionCancela;
   TToolBar *ToolBar;
   TToolButton *butAbrirEstudo;
   TToolButton *butCancela;
   TAction *ActionAbrirEstudo;
   TStatusBar *StatusBar;
   TImageList *ImageList;
	TAction *ActionBDChange;
	TGroupBox *GBoxEstudo;
	TEdit *EditCodigo;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *EditAutor;
	TBitBtn *BitBtn1;
	TPanel *PanelEstudo;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TAction *ActionSelecionaBase;
   TOpenDialog *OpenDialog;
   void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionAbrirEstudoExecute(TObject *Sender);
	void __fastcall DirectoryListBoxChange(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionSelecionaBaseExecute(TObject *Sender);

public:      // User declarations
	__fastcall  TFormEstudoRnt(TComponent* Owner, VTApl *apl_owner, int tipo_estudo);
	__fastcall ~TFormEstudoRnt(void);

private:   //métodos
   void       __fastcall ExibeDadosBaseSelecionada(void);
	void       __fastcall ExibeEstudo(void);
	VTEstudo*  __fastcall ExisteEstudoRNT(void);
	void       __fastcall IniciaConexao(void);
   bool       __fastcall SelecionaBaseRede(void);
   bool       __fastcall ValidaBaseRede(AnsiString sdb_name);
   void       __fastcall VerificaModulos(void);

private:   //objetos externos

private:   //dados locais
	int       tipo_estudo;
	TList     *lisEQP;
	VTAbre    *abre;
	VTApl     *apl;
	VTEstudo  *estudo_rnt;
};

//---------------------------------------------------------------------------
#endif
//eof

