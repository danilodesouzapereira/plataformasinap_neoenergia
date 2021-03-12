//---------------------------------------------------------------------------
#ifndef TFormCroquiH
#define TFormCroquiH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "jpeg.hpp"
//class TJPEGImage;

//---------------------------------------------------------------------------
class TFormCroqui : public TForm
{
__published:	// IDE-managed Components
	TImage *ImageCroqui;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// User declarations
	void __fastcall FormIniciaPosicao(void);

public:		// User declarations
	__fastcall TFormCroqui(TComponent* Owner, VTApl *apl);
	__fastcall ~TFormCroqui(void);
	bool __fastcall LeArquivo(AnsiString filepath);
	bool __fastcall SalvaArquivo(AnsiString filepath);

private: //objetos externos
	VTApl *apl;

private: //objetos locais
	TJPEGImage *jpg;

};

//---------------------------------------------------------------------------
#endif
