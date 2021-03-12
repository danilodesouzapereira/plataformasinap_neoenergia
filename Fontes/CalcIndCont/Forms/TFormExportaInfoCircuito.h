// ---------------------------------------------------------------------------

#ifndef TFormExportaInfoCircuitoH
#define TFormExportaInfoCircuitoH
// ---------------------------------------------------------------------------
#include <stdio.h>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
// ---------------------------------------------------------------------------
class VTApl;
class VTCalcIndCont;

// ---------------------------------------------------------------------------
class TFormExportaInfoCircuito : public TForm
{
__published: // IDE-managed Components

	TButton *Button1;

	void __fastcall Button1Click(TObject *Sender);

private: // User declarations
		public : // User declarations
	__fastcall TFormExportaInfoCircuito(TComponent* Owner, VTApl* apl_owner, TWinControl *parent);
	__fastcall ~TFormExportaInfoCircuito(void);

	VTApl *apl;
	VTCalcIndCont *calcIndCont;
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormExportaInfoCircuito *FormExportaInfoCircuito;
// ---------------------------------------------------------------------------
#endif
