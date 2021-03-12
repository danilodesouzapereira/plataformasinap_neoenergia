//---------------------------------------------------------------------------

#ifndef TFormRjChavesH
#define TFormRjChavesH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ValEdit.hpp>
//---------------------------------------------------------------------------
class VTArvore;
class TInfoRJChave;
//---------------------------------------------------------------------------
class TFormRjChaves : public TForm
{
__published:	// IDE-managed Components
	TTreeView *treeViewArvore;
	TGroupBox *GroupBox1;
	TValueListEditor *vlistEditor;
	void __fastcall treeViewArvoreClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormRjChaves(TComponent* Owner, VTArvore* arvore);
	void __fastcall ExibeValor(TInfoRJChave *str);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormRjChaves *FormRjChaves;
//---------------------------------------------------------------------------
#endif
