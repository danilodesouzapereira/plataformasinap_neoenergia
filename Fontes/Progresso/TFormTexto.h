// --------------------------------------------------------------------------------------------- bof
#ifndef TFormTextoH
#define TFormTextoH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>

// --------------------------------------------------------------------------------------- namespace

// ------------------------------------------------------------------------------------------- class

// -------------------------------------------------------------------------------------------------
class TFormTexto : public TForm
{
__published: // IDE-managed Components

	TRichEdit *RichEdit;

	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public: // User declarations
	__fastcall TFormTexto(TComponent* Owner);
	__fastcall ~TFormTexto(void);
	void __fastcall Add(AnsiString texto, bool clear);
	void __fastcall Atualiza(void);
//jss20191003
	void __fastcall NoAutoInc(void);
	bool __fastcall MaxStep(int max);
	void __fastcall Step(void);

	// dados locais
	struct
	{
		int y1, y2;
		int x, dim;
		TColor color_barra;
		TColor color_fundo;
//jss20191003
		int  max_step;
		int  step;
		bool auto_inc;
	}
	animacao;

private: // métodos
	void __fastcall Clear(TColor color);
	void __fastcall DrawLinha(int x1, int y1, int x2, int y2, int dim, TColor color);
	void __fastcall DrawRetangulo(int x1, int y1, int x2, int y2, TColor color);

	// TCriticalSection *cs;

};

#endif
// --------------------------------------------------------------------------------------------- eof
