//---------------------------------------------------------------------------
#ifndef TFormExportaANEELH
#define TFormExportaANEELH

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
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTExportaANEEL;

//---------------------------------------------------------------------------
class TFormExportaANEEL : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList;
   TAction *ActionExporta;
   TSaveDialog *SaveDialog;
   TToolBar *ToolBar;
   TToolButton *ToolButton1;
   TToolButton *ButCancela;
	TAction *ActionCancela;
	TGroupBox *GBoxBaseDado;
	TPanel *PanelData;
	TLabel *LabelData;
	TDateTimePicker *DateTimePicker;
	void __fastcall ActionExportaExecute(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);

public:		// User declarations
   __fastcall  TFormExportaANEEL(TComponent *Owner, VTApl *apl, AnsiString arq_txt);
   __fastcall ~TFormExportaANEEL(void);

private:	//métodos
   void __fastcall ConfiguraTela(void);
	bool __fastcall ExportaArquivo(AnsiString arq_txt, TDateTime data);
   void __fastcall FormIniciaPosicao(void);

private:	//objetos externos
   VTApl *apl;

private:	//dados locais
   AnsiString   arq_txt;
	VTExportaANEEL *exporta;
};

//---------------------------------------------------------------------------
#endif
//eof
