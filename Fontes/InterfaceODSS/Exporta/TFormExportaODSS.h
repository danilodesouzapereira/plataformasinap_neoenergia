//---------------------------------------------------------------------------
#ifndef TFormExportaODSSH
#define TFormExportaODSSH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <System.Actions.hpp>
#include "VTExportaODSS.h"
//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormExportaODSS : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList;
   TAction *ActionExporta;
   TSaveDialog *SaveDialog;
   TToolBar *ToolBar;
   TToolButton *ToolButton1;
   TToolButton *ButCancela;
   TAction *ActionCancela;
   TRadioGroup *RadioGroupModelo;
   TRadioGroup *RadioGroupTrafoDY;
   TRadioGroup *RadioGroupCargaEqv;
	TGroupBox *GBoxAviso;
	TRichEdit *RichEdit1;
   void __fastcall ActionExportaExecute(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);

public:		// User declarations
   __fastcall  TFormExportaODSS(TComponent *Owner, VTApl *apl);
   __fastcall ~TFormExportaODSS(void);

private:	//métodos
   void __fastcall ConfiguraTela(void);
	bool __fastcall ExportaArquivo(AnsiString arq_txt);
   void __fastcall FormIniciaPosicao(void);

private:	//objetos externos
   VTApl *apl;

private:	//dados locais
   AnsiString    arq_txt;
	strOPCAO_ODSS  opcao;
	VTExportaODSS *exportaODSS;
};

//---------------------------------------------------------------------------
#endif
//eof
