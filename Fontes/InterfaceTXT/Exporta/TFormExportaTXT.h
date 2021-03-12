//---------------------------------------------------------------------------
#ifndef TFormExportaTXTH
#define TFormExportaTXTH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTExportaTXT;

//---------------------------------------------------------------------------
class TFormExportaTXT : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList;
   TAction *ActionExporta;
   TSaveDialog *SaveDialog;
   TToolBar *ToolBar;
   TToolButton *ToolButton1;
   TToolButton *ButCancela;
   TAction *ActionCancela;
   TCheckBox *CheckBoxReducoes;
   TGroupBox *GroupBox1;
   TCheckBox *CheckBoxArranjos;
   TCheckBox *CheckBoxCurvasTipicas;
   TCheckBox *CheckBoxTipos;
   TCheckBox *CheckBoxRedes;
   TCheckBox *CheckBoxCript;
   void __fastcall ActionExportaExecute(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);

public:		// User declarations
   __fastcall  TFormExportaTXT(TComponent *Owner, VTApl *apl, AnsiString arq_txt);
   __fastcall ~TFormExportaTXT(void);

private:	//métodos
   void __fastcall ConfiguraTela(void);
   bool __fastcall ExportaTXT(AnsiString arq_txt);
   void __fastcall FormIniciaPosicao(void);

private:	//objetos externos
   VTApl *apl;

private:	//dados locais
   AnsiString   arq_txt;
   //VTCtrlPanel  *ctrlpanel;
   VTExportaTXT *exportaTXT;
};

//---------------------------------------------------------------------------
#endif
//eof
