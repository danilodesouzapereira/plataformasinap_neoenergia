//---------------------------------------------------------------------------
#ifndef TFormCronometroH
#define TFormCronometroH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormCronometro : public TForm
{
__published:	// IDE-managed Components
   TListView *lview;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);

public:		// User declarations
        __fastcall TFormCronometro(TComponent* Owner, VTApl *apl);
        __fastcall ~TFormCronometro(void);
   void __fastcall Insere(AnsiString codigo, AnsiString descricao, int acionamentos, int mseg, double perc);

private: //métodos
   void __fastcall FormIniciaPosicao(void);

private: //objstos externos
   VTApl *apl;
   };

//---------------------------------------------------------------------------
#endif
