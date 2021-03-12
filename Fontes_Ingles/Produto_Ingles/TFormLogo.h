//---------------------------------------------------------------------------
#ifndef TFormLogoH
#define TFormLogoH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>

//---------------------------------------------------------------------------
class TFormLogo : public TForm
{
__published:	// IDE-managed Components
   TImage *Image;

public:		// User declarations
   __fastcall  TFormLogo(TComponent* Owner, TWinControl *parent);
   __fastcall ~TFormLogo(void);

private:	// User declarations
};

//---------------------------------------------------------------------------
#endif
//eof

