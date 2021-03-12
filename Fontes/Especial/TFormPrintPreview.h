//---------------------------------------------------------------------------
#ifndef TFormPrintPreviewH
#define TFormPrintPreviewH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormPrintPreview : public TForm
{
__published:	// IDE-managed Components

public:		// User declarations
	__fastcall TFormPrintPreview(TComponent* Owner, VTApl *apl_owner);
	__fastcall ~TFormPrintPreview(void);

private:	// User declarations
};

//---------------------------------------------------------------------------
#endif
//eof
