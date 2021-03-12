//---------------------------------------------------------------------------

#ifndef TFormMedicaoDetalheH
#define TFormMedicaoDetalheH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
//---------------------------------------------------------------------------
class TMedicao;
class VTApl;
class VTCarga;
//---------------------------------------------------------------------------
class TFormMedicaoDetalhe : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox4;
	TListView *lviewMTFIX;
	TListView *lviewMTVAR;
	TListView *lviewBTFIX;
	TListView *lviewBTVAR;
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	void __fastcall ToolButton1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormMedicaoDetalhe(TComponent* Owner, VTApl *apl, TMedicao *medicao, int ndias);
	void __fastcall Exporta(void);
	void __fastcall PreencheListViews(void);

private:
	TMedicao *mMedicao;
	VTApl *mApl;
	int mNdias;

private:
	void __fastcall PreencheListViews(TListView *lview, TList *lisCARGAS);
	void __fastcall PreencheListItem(TListItem *item, VTCarga *carga);
	void __fastcall ExportaListView(TListView *lview, AnsiString filepath);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormMedicaoDetalhe *FormMedicaoDetalhe;
//---------------------------------------------------------------------------
#endif
