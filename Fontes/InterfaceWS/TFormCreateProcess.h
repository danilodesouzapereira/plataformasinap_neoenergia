//---------------------------------------------------------------------------
#ifndef TFormCreateProcessH
#define TFormCreateProcessH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class TFormCreateProcess : public TForm
{
__published:	// IDE-managed Components
   TTimer *timer;
   TEdit *EditCodigo;
   TProgressBar *pbar;
   TButton *ButCancela;
   TGroupBox *GBoxCodigo;
   void __fastcall timerTimer(TObject *Sender);
   void __fastcall ButCancelaClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public:		// User declarations
        __fastcall  TFormCreateProcess(TComponent* Owner, AnsiString cmd_line);
        __fastcall ~TFormCreateProcess(void);

private:	//métodos

private:	//dados
	WideString          cmd_line;
   PROCESS_INFORMATION pi;
};

#endif
//---------------------------------------------------------------------------//eof

