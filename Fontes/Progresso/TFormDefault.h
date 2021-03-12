// --------------------------------------------------------------------------------------------- bof
#ifndef TFormDefaultH
#define TFormDefaultH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>

// --------------------------------------------------------------------------------------- namespace
using namespace std;

// ------------------------------------------------------------------------------------------- class
class TFormDefault : public TForm
{
__published: // IDE-managed Components
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public: // User declarations
	__fastcall TFormDefault(TComponent* Owner);
	__fastcall ~TFormDefault(void);
	void __fastcall Atualiza(void);

private: // métodos
	void __fastcall DrawCirculoXor(int x, int y, int raio);
	void __fastcall DrawCircunferencia(int x, int y, int raio, TColor color);

	// dados locais
	/*
	 struct   {
	 bool up;
	 int  x, y;
	 int  raio, raio_min, raio_max;
	 }animacao;
	 */
	struct
	{
		// bool   completo;
		int x, y;
		int raio, count, count_max;
	}

	animacao;
	// TCriticalSection *cs;
};

#endif
// --------------------------------------------------------------------------------------------- eof
