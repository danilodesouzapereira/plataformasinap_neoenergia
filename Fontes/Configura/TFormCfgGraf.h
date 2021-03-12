//---------------------------------------------------------------------------
#ifndef TFormCfgGrafH
#define TFormCfgGrafH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>

//---------------------------------------------------------------------------
class TPad;
class VTGeral;
class VTBDado;
class TformCfgPadrao;

//---------------------------------------------------------------------------
class TformCfgGraf : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionDesfaz;
   TAction *ActionEdita;
   TImageList *ImageList;
   TToolBar *ToolBar1;
   TToolButton *butDefault;
   TToolButton *butInclui;
   TToolButton *butSair;
   TComboBox *cboPadrao;
   TLabel *Label1;
   TAction *EscolhePadrao;
   TGroupBox *GroupBox1;
   TPanel *pnlParCfg;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionDesfazExecute(TObject *Sender);
   void __fastcall EscolhePadraoExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);

public:		// User declarations
   __fastcall TformCfgGraf(TComponent* Owner, AnsiString dirBase, AnsiString dirRede);
   __fastcall ~TformCfgGraf(void);

private:	// User declarations
   void __fastcall LoadPadrao(void);

private:	// User declarations
   VTGeral        *geral;
   VTBDado        *bdado;
   TList          *lisPad;
   TformCfgPadrao *formCfgPad;
   TPad           *tmpPad;

};
//---------------------------------------------------------------------------
#endif
//eof
