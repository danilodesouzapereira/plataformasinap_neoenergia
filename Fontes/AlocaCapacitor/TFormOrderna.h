//---------------------------------------------------------------------------

#ifndef TFormOrdernaH
#define TFormOrdernaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
class VTApl;
//---------------------------------------------------------------------------
class TFormOrderna : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBox;
	void __fastcall ListBoxClick(TObject *Sender);
	void __fastcall ListBoxDblClick(TObject *Sender);
public:		// User declarations
	__fastcall TFormOrderna(TComponent* Owner, VTApl *apl, TList *lisEqp);
private:	// User declarations
	void __fastcall IniciaLisEqpto(TList *lisEqp);
	VTApl *apl;
};
//---------------------------------------------------------------------------
#endif
