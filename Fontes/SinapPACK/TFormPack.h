//---------------------------------------------------------------------------

#ifndef TFormPackH
#define TFormPackH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

class VTApl;

//---------------------------------------------------------------------------
class TFormPack : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *RichEdit;
	TTimer *Timer;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);

private:	// User declarations

public:		// User declarations
	__fastcall TFormPack(TComponent* Owner);
	__fastcall ~TFormPack(void);
	bool EncerraSinap(void);
	void Executa(void);

private: //local
	VTApl *apl;

};
//---------------------------------------------------------------------------
extern PACKAGE TFormPack *FormPack;
//---------------------------------------------------------------------------
#endif
