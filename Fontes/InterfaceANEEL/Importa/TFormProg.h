//---------------------------------------------------------------------------

#ifndef TFormProgH
#define TFormProgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class VTProgresso;
//---------------------------------------------------------------------------
class TFormProg : public TForm
{
__published:	// IDE-managed Components
   TBitBtn *butCancel;
	TPanel *panelFundo;
	TLabel *lblPorc;
	TLabel *lblTitulo1;
	TLabel *lblTitulo2;
	TProgressBar *pbar;
	void __fastcall butCancelClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public:		// User declarations
	__fastcall TFormProg(TComponent* Owner);
	__fastcall ~TFormProg(void);

   bool __fastcall Cancelado(void);
   void __fastcall Hide(void);
   void __fastcall Progresso(void);
	void __fastcall Progresso(AnsiString txt);
   void __fastcall Progresso(int posicao);
   void __fastcall Show(AnsiString titulo, int min, int max);

private:	// User declarations
	  bool         cancelado;
};
//---------------------------------------------------------------------------
#endif
