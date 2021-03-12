//---------------------------------------------------------------------------
#ifndef TFormExportaXMLH
#define TFormExportaXMLH

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
class VTCtrlPanel;
class VTExportaXML;

//---------------------------------------------------------------------------
class TFormExportaXML : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList1;
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
   void __fastcall ActionExportaExecute(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);

public:		// User declarations
   __fastcall TFormExportaXML(TComponent *Owner, VTApl *apl, AnsiString arq_xml);
   __fastcall ~TFormExportaXML(void);

private:	//objetos externos
   void __fastcall ConfiguraTela(void);
   bool __fastcall ExportaXML(AnsiString arq_xml);
   void __fastcall FormIniciaPosicao(void);

private:	//objetos externos
   VTApl *apl;

private:	//dados locais
   AnsiString    arq_xml;
   VTCtrlPanel  *ctrlpanel;
   VTExportaXML *exportaXML;
};

//---------------------------------------------------------------------------
#endif
//eof
